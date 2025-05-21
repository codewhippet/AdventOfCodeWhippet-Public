#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2020_Types
{
	struct Puzzle
	{
		multimap<string, string> ContainedBy;
		map<string, vector<pair<int, string>>> Contains;
	};
}

using namespace Puzzle07_2020_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p;

	vector<string> rules = ReadAllLines(input);

	regex whatBag(R"((\w+ \w+) bags contain.+)");
	regex contains(R"( (\d+) (\w+ \w+) bags?)");

	multimap<string, string> containedBy;

	for (string rule : rules)
	{
		smatch bagMatch;
		if (regex_match(rule, bagMatch, whatBag))
		{
			string containingBag = bagMatch[1].str();

			smatch containsMatch;
			while (regex_search(rule, containsMatch, contains))
			{
				p.ContainedBy.insert(make_pair(containsMatch[2].str(), containingBag));
				p.Contains[containingBag].push_back(make_pair(atoi(containsMatch[1].str().c_str()), containsMatch[2].str()));

				rule = containsMatch.suffix().str();
			}
		}
	}

	return p;
}

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	set<string> visited;
	vector<string> searchQueue = { "shiny gold" };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		if (visited.contains(searchQueue[i]))
			continue;

		auto canBeContainedBy = p.ContainedBy.equal_range(searchQueue[i]);
		for (multimap<string, string>::const_iterator containedByIt = canBeContainedBy.first;
			containedByIt != canBeContainedBy.second; containedByIt++)
		{
			const string& next = containedByIt->second;
			if (visited.contains(next) == false)
			{
				searchQueue.push_back(next);
			}
		}

		visited.insert(searchQueue[i]);
	}
	visited.erase("shiny gold");

	int64_t answer = visited.size();

	printf("[2020] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	using TBagCache = map<string, int64_t>;

	function<int64_t(const string&, const Puzzle&, TBagCache&)> NumBagsInside;
	NumBagsInside = [&NumBagsInside](const string& bag, const Puzzle& puzzle, TBagCache& cache) -> int64_t
		{
			TBagCache::const_iterator cachedResult = cache.find(bag);
			if (cachedResult != cache.end())
			{
				return cachedResult->second;
			}

			int64_t numBags = 0;

			map<string, vector<pair<int, string>>>::const_iterator bagContents = puzzle.Contains.find(bag);
			if (bagContents == puzzle.Contains.end())
			{
				numBags = 0;
			}
			else
			{
				for (const pair<int, string>& bagsInside : bagContents->second)
				{
					numBags += bagsInside.first;
					numBags += bagsInside.first * NumBagsInside(bagsInside.second, puzzle, cache);
				}
			}

			cache[bag] = numBags;
			return numBags;
		};

	TBagCache bagCountCache;
	int64_t answer = NumBagsInside("shiny gold", p, bagCountCache);

	printf("[2020] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2020(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
