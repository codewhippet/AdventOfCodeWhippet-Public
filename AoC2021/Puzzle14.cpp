#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2021_Types
{
	struct Puzzle
	{
		string Polymer;
		map<string, char> Template;
	};

	struct CharCounts
	{
		CharCounts()
		{
			memset(&Count[0], 0, sizeof(Count));
		}

		int64_t Count[26];
	};

	void operator+=(CharCounts& lhs, const CharCounts& rhs)
	{
		for (int i = 0; i < 26; i++)
			lhs.Count[i] += rhs.Count[i];
	}
}

using namespace Puzzle14_2021_Types;

static Puzzle ReadPuzzle(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	Puzzle p;
	p.Polymer = lines[0];

	for (size_t i = 2; i < lines.size(); i++)
	{
		const string& line = lines[i];
		p.Template[line.substr(0, 2)] = line[6];
	}

	return p;
}

static string TransformPolymer(const string& polymer, const map<string, char>& templates)
{
	string newPolymer;
	for (size_t i = 0; i + 1 < polymer.size(); i++)
	{
		const string component = polymer.substr(i, 2);
		newPolymer += component[0];

		map<string, char>::const_iterator newComponent = templates.find(component);
		if (newComponent != templates.end())
		{
			newPolymer += newComponent->second;
		}
	}
	newPolymer += *polymer.rbegin();

	return newPolymer;
}

static string Transform10(const string& pair, const map<string, char>& templates)
{
	string polymer = pair;
	for (int i = 0; i < 10; i++)
	{
		polymer = TransformPolymer(polymer, templates);
	}
	return polymer;
}

static CharCounts CountElements(const string& polymerPair,
	const map<string, char>& templates,
	int rounds,
	map<pair<string, int>, CharCounts>* memoizedCounts)
{
	if (rounds < 40)
	{
		map<pair<string, int>, CharCounts>::const_iterator cachedAnswer = memoizedCounts->find(make_pair(polymerPair, rounds));
		if (cachedAnswer != memoizedCounts->end())
		{
			return cachedAnswer->second;
		}
	}

	CharCounts answer;

	// Perform 10 rounds
	string newPolymer = Transform10(polymerPair, templates);
	if (rounds == 10)
	{
		// Don't count the last polymer element
		for (size_t i = 0; i < newPolymer.size() - 1; i++)
		{
			answer.Count[newPolymer[i] - 'A']++;
		}
	}
	else
	{
		for (size_t i = 0; i + 1 < newPolymer.size(); i++)
		{
			answer += CountElements(newPolymer.substr(i, 2), templates, rounds - 10, memoizedCounts);
		}
	}

	(*memoizedCounts)[make_pair(polymerPair, rounds)] = answer;
	return answer;
}

static void Puzzle14_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	string polymer = p.Polymer;

	for (int i = 0; i < 10; i++)
	{
		polymer = TransformPolymer(polymer, p.Template);
	}

	map<char, int> charCounts;
	for (char c : polymer)
	{
		charCounts[c]++;
	}

	int mostCommon = MakeEnumerator<char, int>(charCounts)
		->Select<int>([](map<char, int>::const_reference kvp) { return kvp.second; })
		->Max();

	int leastCommon = MakeEnumerator<char, int>(charCounts)
		->Select<int>([](map<char, int>::const_reference kvp) { return kvp.second; })
		->Min();

	int64_t answer = mostCommon - leastCommon;

	printf("[2021] Puzzle14_A: %" PRId64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	string polymer = p.Polymer;

	CharCounts counts;

	map<pair<string, int>, CharCounts> memoizedCounts;
	for (size_t i = 0; i + 1 < polymer.size(); i++)
	{
		counts += CountElements(polymer.substr(i, 2), p.Template, 40, &memoizedCounts);
	}

	counts.Count[*polymer.rbegin() - 'A']++;

	set<int64_t> sortedCounts;
	for (int i = 0; i < 26; i++)
	{
		if (counts.Count[i] != 0)
		{
			sortedCounts.insert(counts.Count[i]);
		}
	}

	int64_t answer = *sortedCounts.crbegin() - *sortedCounts.cbegin();

	printf("[2021] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2021(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
