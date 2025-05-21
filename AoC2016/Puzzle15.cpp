#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2016_Types
{
}

using namespace Puzzle15_2016_Types;

static vector<pair<int64_t, int64_t>> ParseDiscs(istream& input)
{
	vector<pair<int64_t, int64_t>> discs;
	for (const auto& m : ReadEachLine(input, regex{ R"(Disc #\d+ has (\d+) positions; at time=0, it is at position (\d+).)" }))
	{
		discs.push_back({ atoll(m[1].str().c_str()), atoll(m[2].str().c_str()) });
	}
	return discs;
}

static vector<int64_t> CalculateSyncPoint(const vector<pair<int64_t, int64_t>>& discs)
{
	vector<int64_t> needToBeAt;
	for (int64_t discIndex = 0; discIndex < (int64_t)discs.size(); discIndex++)
	{
		const pair<int64_t, int64_t>& disc = discs[discIndex];
		int64_t needsToBe = (disc.first - ((discIndex + 1) % disc.first)) % disc.first;
		needToBeAt.push_back(needsToBe);
	}
	return needToBeAt;
}

static int64_t FindTimeForAlignment(const vector<pair<int64_t, int64_t>>& discs, const vector<int64_t>& needToBeAt)
{
	assert(discs.size() == needToBeAt.size());

	int64_t step = 1;
	int64_t time = 0;
	for (size_t discIndex = 0; discIndex < discs.size(); discIndex++)
	{
		const pair<int64_t, int64_t>& disc = discs[discIndex];
		while (((disc.second + time) % disc.first) != needToBeAt[discIndex])
		{
			time += step;
		}
		step *= disc.first;
	}

	return time;
}

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, int64_t>> discs = ParseDiscs(input);
	vector<int64_t> needToBeAt = CalculateSyncPoint(discs);

	int64_t answer = FindTimeForAlignment(discs, needToBeAt);

	printf("[2016] Puzzle15_A: %" PRId64 "\n", answer);
}


static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, int64_t>> discs = ParseDiscs(input);
	discs.push_back({ 11, 0 });
	vector<int64_t> needToBeAt = CalculateSyncPoint(discs);

	int64_t answer = FindTimeForAlignment(discs, needToBeAt);

	printf("[2016] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2016(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
