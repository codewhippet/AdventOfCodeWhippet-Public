#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2016_Types
{
}

using namespace Puzzle20_2016_Types;

static vector<pair<int64_t, int64_t>> ReadRanges(istream& input)
{
	vector<pair<int64_t, int64_t>> ranges;
	for (const auto& m : ReadEachLine(input, regex{ R"((\d+)-(\d+))" }))
	{
		ranges.push_back({ atoll(m[1].str().c_str()), atoll(m[2].str().c_str()) });
	}
	return ranges;
}

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, int64_t>> ranges = ReadRanges(input);
	int64_t lowestIp = 0;
	while (true)
	{
		bool moved = false;
		for (const auto& range : ranges)
		{
			if ((lowestIp >= range.first) && (lowestIp <= range.second))
			{
				lowestIp = range.second + 1;
				moved = true;
			}
		}
		if (moved == false)
		{
			break;
		}
	}

	int64_t answer = lowestIp;

	printf("[2016] Puzzle20_A: %" PRIu64 "\n", answer);
}


static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, int64_t>> ranges = ReadRanges(input);

	vector<int64_t> boundaries;
	for (const auto& range : ranges)
	{
		boundaries.push_back(range.first - 1);
		boundaries.push_back(range.first);
		boundaries.push_back(range.second);
		boundaries.push_back(range.second + 1);
	}

	{
		ranges::sort(boundaries);
		const auto [first, last] = ranges::unique(boundaries);
		boundaries.erase(first, last);
	}

	map<int64_t, size_t> boundaryToIndex;
	for (size_t i = 0; i < boundaries.size(); i++)
	{
		boundaryToIndex[boundaries[i]] = i;
	}

	vector<bool> blockedRange(boundaries.size());
	for (const auto& range : ranges)
	{
		size_t rangeStart = boundaryToIndex.at(range.first);
		size_t rangeEnd = boundaryToIndex.at(range.second);
		for (size_t i = rangeStart; i <= rangeEnd; i++)
		{
			blockedRange[i] = true;
		}
	}

	assert(blockedRange.front() == false);
	assert(blockedRange.back() == false);

	int64_t allowedIps = 0;
	for (size_t i = 1; i + 1 < blockedRange.size(); i++)
	{
		if (blockedRange[i] == false)
		{
			int64_t allowedCount = boundaries[i + 1] - boundaries[i];
			allowedIps += allowedCount;
		}
	}

	int64_t answer = allowedIps;

	printf("[2016] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2016(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
