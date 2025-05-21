#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2015_Types
{
}

using namespace Puzzle16_2015_Types;

static map<int, map<string, int>> ParseSues(istream& input)
{
	map<int, map<string, int>> sues;

	for (const string& line : ReadEachLine(input))
	{
		int sueIndex = 0;
		char aType[64] = { 0 };
		int aCount = 0;
		char bType[64] = { 0 };
		int bCount = 0;
		char cType[64] = { 0 };
		int cCount = 0;
		int scanned = sscanf(line.c_str(), "Sue %d: %[a-z]: %d, %[a-z]: %d, %[a-z]: %d",
			&sueIndex,
			aType,
			&aCount,
			bType,
			&bCount,
			cType,
			&cCount);
		assert(scanned == 7);
		(void)scanned;

		sues.insert({ sueIndex, { { aType, aCount }, { bType, bCount }, { cType, cCount } } });
	}

	return sues;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int, map<string, int>> sues = ParseSues(input);

	map<string, int> detectedValues =
	{
		{ "children", 3 },
		{ "cats", 7 },
		{ "samoyeds", 2 },
		{ "pomeranians", 3 },
		{ "akitas", 0 },
		{ "vizslas", 0 },
		{ "goldfish", 5 },
		{ "trees", 3 },
		{ "cars", 2 },
		{ "perfumes", 1 }
	};

	erase_if(sues,
		[&](const auto& sueCounts)
		{
			for (const auto& targetCounts : detectedValues)
			{
				if (sueCounts.second.contains(targetCounts.first) &&
					sueCounts.second.at(targetCounts.first) != targetCounts.second)
				{
					return true;
				}
			}

			return false;
		});
	assert(sues.size() == 1);

	int64_t answer = sues.begin()->first;

	printf("[2015] Puzzle16_A: %" PRId64 "\n", answer);
}


static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int, map<string, int>> sues = ParseSues(input);

	map<string, int> needExact =
	{
		{ "children", 3 },
		{ "samoyeds", 2 },
		{ "akitas", 0 },
		{ "vizslas", 0 },
		{ "cars", 2 },
		{ "perfumes", 1 }
	};

	map<string, int> needGreaterThan =
	{
		{ "cats", 7 },
		{ "trees", 3 },
	};

	map<string, int> needLessThan =
	{
		{ "pomeranians", 3 },
		{ "goldfish", 5 },
	};

	erase_if(sues,
		[&](const auto& sueCounts)
		{
			for (const auto& targetCounts : needExact)
			{
				if (sueCounts.second.contains(targetCounts.first) &&
					sueCounts.second.at(targetCounts.first) != targetCounts.second)
				{
					return true;
				}
			}

			for (const auto& targetCounts : needGreaterThan)
			{
				if (sueCounts.second.contains(targetCounts.first) &&
					sueCounts.second.at(targetCounts.first) <= targetCounts.second)
				{
					return true;
				}
			}

			for (const auto& targetCounts : needLessThan)
			{
				if (sueCounts.second.contains(targetCounts.first) &&
					sueCounts.second.at(targetCounts.first) >= targetCounts.second)
				{
					return true;
				}
			}

			return false;
		});
	assert(sues.size() == 1);

	int64_t answer = sues.begin()->first;

	printf("[2015] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2015(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
