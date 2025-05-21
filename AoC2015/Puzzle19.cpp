#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2015_Types
{
	struct Reaction
	{
		vector<pair<string, string>> Substitutions;
		string StartingChemical;
	};

	struct AtomicStructure
	{
		set<string> Atoms;
		multimap<string, vector<string>> Substitutions;
		map<string, set<string>> FrontSets;
		map<string, set<string>> BackSets;
		vector<string> TargetMolecule;
	};

	struct CacheKey
	{
		size_t Begin;
		size_t End;
		string Element;

		auto operator<=>(const CacheKey&) const = default;
	};
}

using namespace Puzzle19_2015_Types;

static Reaction ParseReaction(istream& input)
{
	Reaction r;

	vector<string> lines = ReadAllLines(input);
	assert(lines.size() >= 2);
	for (size_t i = 0; i < lines.size() - 2; i++)
	{
		char from[64] = { 0 };
		char to[64] = { 0 };
		int scanned = sscanf(lines[i].c_str(), "%s => %s", from, to);
		assert(scanned == 2);
		(void)scanned;

		r.Substitutions.push_back({ from, to });
	}

	r.StartingChemical = lines.back();

	return r;
}

static AtomicStructure ParseAtomicStructure(istream& input)
{
	AtomicStructure structure;

	regex atomPattern(R"([A-Z][a-z]?)");

	vector<string> lines = ReadAllLines(input);
	assert(lines.size() >= 2);
	for (size_t i = 0; i < lines.size() - 2; i++)
	{
		char from[64] = { 0 };
		char to[64] = { 0 };
		int scanned = sscanf(lines[i].c_str(), "%s => %s", from, to);
		assert(scanned == 2);
		(void)scanned;

		structure.Atoms.insert(from);

		string toAsString = to;
		vector<string> toAtoms;
		for (sregex_iterator it{ toAsString.begin(), toAsString.end(), atomPattern }; it != sregex_iterator{}; ++it)
		{
			structure.Atoms.insert(it->str());
			toAtoms.push_back(it->str());
		}

		structure.Substitutions.insert({ from, toAtoms });
	}

	for (sregex_iterator it{ lines.back().begin(), lines.back().end(), atomPattern }; it != sregex_iterator{}; ++it)
	{
		structure.Atoms.insert(it->str());
		structure.TargetMolecule.push_back(it->str());
	}

	return structure;
}

static void ExecuteSingleReplacement(const Reaction& reaction, const string& startFrom, set<string>* results)
{
	for (size_t currentPos = 0; currentPos < startFrom.size(); currentPos++)
	{
		for (const auto& sub : reaction.Substitutions)
		{
			if (strncmp(&startFrom[currentPos], sub.first.c_str(), sub.first.size()) == 0)
			{
				results->insert(startFrom.substr(0, currentPos) + sub.second + startFrom.substr(currentPos + sub.first.size()));
			}
		}
	}
}

static void BuildFrontSet(const AtomicStructure& structure, const string& atom, set<string> *frontSet)
{
	frontSet->insert(atom);

	auto substRange = structure.Substitutions.equal_range(atom);
	for (auto it = substRange.first; it != substRange.second; ++it)
	{
		const string& firstAtom = it->second.front();
		if (frontSet->insert(firstAtom).second)
		{
			BuildFrontSet(structure, firstAtom, frontSet);
		}
	}
}

static void BuildBackSet(const AtomicStructure& structure, const string& atom, set<string>* backSet)
{
	backSet->insert(atom);

	auto substRange = structure.Substitutions.equal_range(atom);
	for (auto it = substRange.first; it != substRange.second; ++it)
	{
		const string& lastAtom = it->second.back();
		if (backSet->insert(lastAtom).second)
		{
			BuildBackSet(structure, lastAtom, backSet);
		}
	}
}

static set<pair<string, string>> AllJoinsForElements(const AtomicStructure& structure, const string& firstElement, const string& secondElement)
{
	set<pair<string, string>> joins;
	for (const auto& backSetElement : structure.BackSets.at(firstElement))
	{
		for (const auto& frontSetElement : structure.FrontSets.at(secondElement))
		{
			joins.insert({ backSetElement, frontSetElement });
		}
	}
	return joins;
}

static int64_t ShortestReactionForElement(const AtomicStructure& structure, size_t begin, size_t end, const string& element, map<CacheKey, int64_t>* cache);
static int64_t ShortestReactionForSequence(const AtomicStructure& structure, size_t begin, size_t end, const vector<string> &sequence, size_t sequenceIndex, map<CacheKey, int64_t>* cache);

static int64_t ShortestReactionForElement(const AtomicStructure& structure, size_t begin, size_t end, const string& element, map<CacheKey, int64_t> *cache)
{
	CacheKey answerKey{ begin, end, element };
	auto existingAnswer = cache->find(answerKey);
	if (existingAnswer != cache->end())
	{
		return existingAnswer->second;
	}

	assert(end > begin);
	if ((end - begin) == 1)
	{
		int64_t cost = (structure.TargetMolecule[begin] == element ? 0 : numeric_limits<int64_t>::max());
		cache->insert({ answerKey, cost });
		return cost;
	}

	int64_t shortestReaction = numeric_limits<int64_t>::max();

	auto substRange = structure.Substitutions.equal_range(element);
	for (auto it = substRange.first; it != substRange.second; ++it)
	{
		int64_t candidateReaction = ShortestReactionForSequence(structure, begin, end, it->second, 0, cache);
		if (candidateReaction != numeric_limits<int64_t>::max())
		{
			shortestReaction = min(shortestReaction, candidateReaction + 1);
		}
	}

	cache->insert({ answerKey, shortestReaction });
	return shortestReaction;
}

static int64_t ShortestReactionForSequence(const AtomicStructure& structure, size_t begin, size_t end, const vector<string>& sequence, size_t sequenceIndex, map<CacheKey, int64_t>* cache)
{
	assert(end > begin);
	assert(sequenceIndex < sequence.size());

	if (sequenceIndex == sequence.size() - 1)
	{
		return ShortestReactionForElement(structure, begin, end, sequence.back(), cache);
	}

	if (structure.FrontSets.at(sequence[sequenceIndex]).contains(structure.TargetMolecule[begin]) == false)
	{
		return numeric_limits<int64_t>::max();
	}

	int64_t shortestReaction = numeric_limits<int64_t>::max();

	// Find where we can put the separating join
	set<pair<string, string>> joins = AllJoinsForElements(structure, sequence[sequenceIndex], sequence[sequenceIndex + 1]);
	for (const pair<string, string> &join : joins)
	{
		for (size_t joinIndex = begin; joinIndex + 1 < end; joinIndex++)
		{
			if ((structure.TargetMolecule[joinIndex] == join.first) &&
				(structure.TargetMolecule[joinIndex + 1] == join.second))
			{
				int64_t candidateElementCost = ShortestReactionForElement(structure, begin, joinIndex + 1, sequence[sequenceIndex], cache);
				if (candidateElementCost != numeric_limits<int64_t>::max())
				{
					int64_t candidateSubsequenceCost = ShortestReactionForSequence(structure, joinIndex + 1, end, sequence, sequenceIndex + 1, cache);
					if (candidateSubsequenceCost != numeric_limits<int64_t>::max())
					{
						shortestReaction = min(shortestReaction, candidateElementCost + candidateSubsequenceCost);
					}
				}
			}
		}
	}

	return shortestReaction;
}

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Reaction reaction = ParseReaction(input);

	set<string> derivatives;
	ExecuteSingleReplacement(reaction, reaction.StartingChemical, &derivatives);

	int64_t answer = derivatives.size();

	printf("[2015] Puzzle19_A: %" PRId64 "\n", answer);
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	AtomicStructure structure = ParseAtomicStructure(input);

	for (const string& atom : structure.Atoms)
	{
		BuildFrontSet(structure, atom, &structure.FrontSets[atom]);
		BuildBackSet(structure, atom, &structure.BackSets[atom]);
	}

	map<CacheKey, int64_t> cache;
	int64_t answer = ShortestReactionForElement(structure, 0, structure.TargetMolecule.size(), "e", &cache);

	printf("[2015] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2015(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
