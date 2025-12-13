#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2025_Types
{
	enum class PointType
	{
		// Ordering is important here!
		OpenRange,
		Item,
		CloseRange,
	};
}

using namespace Puzzle05_2025_Types;

static void Puzzle05_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<tuple<uint64_t, PointType, int64_t>> inventory;

	int64_t rangeId = 0;

	string line;
	while (getline(input, line))
	{
		uint64_t a, b;
		if (sscanf(line.c_str(), "%llu-%llu", &a, &b) == 2)
		{
			inventory.push_back({ a, PointType::OpenRange, rangeId });
			inventory.push_back({ b, PointType::CloseRange, rangeId });
			rangeId++;
		}
		else if (sscanf(line.c_str(), "%llu", &a) == 1)
		{
			inventory.push_back({ a, PointType::Item, -1 });
		}
	}

	ranges::sort(inventory);

	set<int64_t> openRanges;

	int64_t answer = 0;
	for (const auto& item : inventory)
	{
		switch (get<1>(item))
		{
		case PointType::OpenRange:
			openRanges.insert(get<2>(item));
			break;
		case PointType::Item:
			if (!openRanges.empty())
			{
				answer++;
			}
			break;
		case PointType::CloseRange:
			openRanges.erase(get<2>(item));
			break;
		}
	}

	printf("[2025] Puzzle05_A: %" PRId64 "\n", answer);
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<tuple<uint64_t, PointType, int64_t>> inventory;

	int64_t rangeId = 0;

	string line;
	while (getline(input, line))
	{
		uint64_t a, b;
		if (sscanf(line.c_str(), "%llu-%llu", &a, &b) == 2)
		{
			inventory.push_back({ a, PointType::OpenRange, rangeId });
			inventory.push_back({ b, PointType::CloseRange, rangeId });
			rangeId++;
		}
	}

	ranges::sort(inventory);

	set<int64_t> openRanges;

	int64_t freshStart = -1;

	int64_t answer = 0;
	for (const auto& item : inventory)
	{
		switch (get<1>(item))
		{
		case PointType::OpenRange:
			if (openRanges.empty())
			{
				freshStart = get<0>(item);
			}
			openRanges.insert(get<2>(item));
			break;

		case PointType::CloseRange:
			openRanges.erase(get<2>(item));
			if (openRanges.empty())
			{
				answer += get<0>(item) - freshStart + 1;
			}
			break;
		}
	}

	printf("[2025] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2025(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
