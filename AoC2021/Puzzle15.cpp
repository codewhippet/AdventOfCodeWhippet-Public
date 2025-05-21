#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2021_Types
{
	struct Cavern
	{
		vector<vector<int8_t>> Data;

		int8_t operator()(const Point2& p) const
		{
			return Data[p.Y][p.X];
		}

		bool IsIn(const Point2& p) const
		{
			return p.X >= 0
				&& p.X < (int64_t)Data.front().size()
				&& p.Y >= 0
				&& p.Y < (int64_t)Data.size();
		}
	};
}

using namespace Puzzle15_2021_Types;

static Cavern ReadRiskLevel(istream& input, size_t repeatCount)
{
	Cavern cavern;
	for (const string& line : ReadEachLine(input))
	{
		vector<int8_t> row;
		ranges::copy(line | views::transform([](char c) -> int8_t { return c - '0'; }), back_inserter(row));
		cavern.Data.push_back(row);
	}

	auto IncrementWithWrap = [](int8_t v) -> int8_t { return (v % 9) + 1; };

	// Width
	size_t originalWidth = cavern.Data.front().size();
	for (vector<int8_t>& row : cavern.Data)
	{
		for (size_t x = originalWidth; x < (originalWidth * repeatCount); x++)
		{
			row.push_back(IncrementWithWrap(row[x - originalWidth]));
		}
	}

	// Height
	size_t originalHeight = cavern.Data.size();
	for (size_t y = originalHeight; y < (originalHeight * repeatCount); y++)
	{
		cavern.Data.push_back({});
		ranges::copy(cavern.Data[y - originalHeight] | views::transform(IncrementWithWrap), back_inserter(cavern.Data.back()));
	}

	return cavern;
}

static int64_t FindLowestRisk(const Cavern& cavern)
{
	const Point2 start{};
	const Point2 finish{ (int64_t)cavern.Data.front().size() - 1, (int64_t)cavern.Data.size() - 1 };

	multimap<int64_t, Point2> searchQueue{ { 0, start } };
	map<Point2, int64_t> visited;

	while (!searchQueue.empty())
	{
		auto [currentRisk, currentPos] = *searchQueue.begin();
		searchQueue.erase(searchQueue.begin());

		if (currentPos == finish)
		{
			return currentRisk;
		}

		if (visited.insert({ currentPos, currentRisk }).second == false)
			continue;

		for (const Point2& dir : Point2::CardinalDirections())
		{
			const Point2 neighbour = currentPos + dir;
			if (cavern.IsIn(neighbour))
			{
				searchQueue.insert({ currentRisk + cavern(neighbour), neighbour });
			}
		}
	}

	return -1;
}

static void Puzzle15_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Cavern cavern = ReadRiskLevel(input, 1);
	int64_t answer = FindLowestRisk(cavern);

	printf("[2021] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Cavern cavern = ReadRiskLevel(input, 5);
	int64_t answer = FindLowestRisk(cavern);

	printf("[2021] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2021(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
