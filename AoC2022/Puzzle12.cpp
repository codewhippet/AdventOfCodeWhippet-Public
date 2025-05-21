#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2022_Types
{
}

using namespace Puzzle12_2022_Types;

static bool CanClimb(char from, char to)
{
	return (to == 'E' ? from >= 'y' : (to - from) <= 1);
}

static bool CanDescend(char from, char to)
{
	return (to - from >= -1);
}

static int64_t ShortestPath(const ArrayMap2D &heights, const Point2 &startAt, char lookingFor, bool (*heightCheck)(char, char))
{
	vector<pair<Point2, int64_t>> searchQueue{ { startAt, 0 } };
	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		const auto [currentPos, currentSteps] = searchQueue[i];
		if (visited.insert(currentPos).second == false)
			continue;

		if (heights(currentPos) == lookingFor)
			return currentSteps;

		for (const Point2 &dir : Point2::CardinalDirections())
		{
			const Point2 neighbour = currentPos + dir;
			if (heights.IsInside(neighbour) && heightCheck(heights(currentPos), heights(neighbour)))
			{
				searchQueue.push_back({ neighbour, currentSteps + 1 });
			}
		}
	}
	return -1;
}

static void Puzzle12_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D heights = ReadArrayMap(input);
	Point2 start = (heights.Grid() | views::filter([](const auto &p) { return p.second == 'S'; }) | ranges::to<vector>()).front().first;
	Point2 end = (heights.Grid() | views::filter([](const auto &p) { return p.second == 'E'; }) | ranges::to<vector>()).front().first;

	heights(start) = 'a';

	int64_t answer = ShortestPath(heights, start, 'E', &CanClimb);

	printf("[2022] Puzzle12_A: %" PRId64 "\n", answer);
}

static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D heights = ReadArrayMap(input);
	Point2 start = (heights.Grid() | views::filter([](const auto &p) { return p.second == 'S'; }) | ranges::to<vector>()).front().first;
	Point2 end = (heights.Grid() | views::filter([](const auto &p) { return p.second == 'E'; }) | ranges::to<vector>()).front().first;

	heights(start) = 'a';
	heights(end) = 'z';

	int64_t answer = ShortestPath(heights, end, 'a', &CanDescend);

	printf("[2022] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2022(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
