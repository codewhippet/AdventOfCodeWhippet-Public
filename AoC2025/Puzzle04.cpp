#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2025_Types
{
}

using namespace Puzzle04_2025_Types;

static int64_t CountNeighbours(const Point2& p, const ArrayMap2D& floor)
{
	return ranges::count_if(Point2::CardinalAndDiagonalDirections() | views::transform([&](const auto& dir) { return p + dir; }),
		[&](const auto& neighbour)
		{
			return floor(neighbour) == '@';
		});
}

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D floor = ReadArrayMap(input);

	int64_t answer = ranges::distance(floor.Grid()
		| views::filter([](const auto& p) { return p.second == '@'; })
		| views::filter([&](const auto& p) { return CountNeighbours(p.first, floor) < 4; }));

	printf("[2025] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D floor = ReadArrayMap(input);
	vector<Point2> rolls = floor.Grid()
		| views::filter([](const auto& p) { return p.second == '@'; })
		| views::keys
		| ranges::to<vector>();

	int64_t answer = 0;
	while (true)
	{
		const auto rollsToRemove = partition(rolls.begin(), rolls.end(),
			[&](const Point2& p)
			{
				return CountNeighbours(p, floor) >= 4;
			});
		if (rollsToRemove == rolls.end())
			break;

		answer += distance(rollsToRemove, rolls.end());

		for (auto it = rollsToRemove; it != rolls.end(); it++)
		{
			floor(*it) = '.';
		}
		rolls.erase(rollsToRemove, rolls.end());
	}

	printf("[2025] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2025(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
