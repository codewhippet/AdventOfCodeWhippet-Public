#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2025_Types
{
	enum class Direction
	{
		North = 1,
		East = 2,
		South = 3,
		West = 4,

		Error = -1
	};
}

using namespace Puzzle09_2025_Types;

static int64_t SquareSize(const Point2& a, const Point2& b)
{
	return (abs(b.X - a.X) + 1) * (abs(b.Y - a.Y) + 1);
}

static Direction GetDirection(const Point2& a, const Point2& b)
{
	assert(a != b);

	if (b.Y < a.Y)
		return Direction::North;

	if (b.X > a.X)
		return Direction::East;

	if (b.Y > a.Y)
		return Direction::South;

	if (b.X < a.X)
		return Direction::West;

	assert(false);
	return Direction::Error;
}

static bool IsConvexCorner(Direction a, Direction b)
{
	return
		((a == Direction::North) && (b == Direction::East)) ||
		((a == Direction::East) && (b == Direction::South)) ||
		((a == Direction::South) && (b == Direction::West)) ||
		((a == Direction::West) && (b == Direction::North));
}

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Point2> points;
	for (const auto& line : ScanfEachLine<int64_t, int64_t>(input, "%lld,%lld"))
	{
		points.push_back({ get<0>(line), get<1>(line) });
	}

	int64_t answer = ranges::max(AllUnorderedPairs(points.size())
		| views::transform([&](const auto& p) { return SquareSize(points[p.first], points[p.second]); }));

	printf("[2025] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Point2> points;
	for (const auto& line : ScanfEachLine<int64_t, int64_t>(input, "%lld,%lld"))
	{
		Point2 pos{ get<0>(line), get<1>(line) };
		points.push_back(pos);
	}

	//**NOTE: Hard-coded assumption that the input is clockwise!

	//
	// Trace the outline as walls
	//

	map<int64_t, set<int64_t>> northWalls;
	map<int64_t, set<int64_t>> eastWalls;
	map<int64_t, set<int64_t>> southWalls;
	map<int64_t, set<int64_t>> westWalls;

	for (int64_t i = 0; i < (int64_t)points.size(); i++)
	{
		const Point2& a = points[(i - 1 + points.size()) % points.size()];
		const Point2& b = points[i];
		const Point2& c = points[(i + 1) % points.size()];

		Direction abDirection = GetDirection(a, b);
		Direction bcDirection = GetDirection(b, c);
		assert(abDirection != bcDirection); // Assume no colinear segments

		const bool isConvex = IsConvexCorner(abDirection, bcDirection);

		switch (bcDirection)
		{
		case Direction::North:
			for (const Point2& p : points)
			{
				if ((p.Y < b.Y) && (p.Y > c.Y))
				{
					westWalls[p.Y].insert(-b.X);
				}
			}
			if (isConvex)
			{
				southWalls[b.X].insert(b.Y);
				westWalls[b.Y].insert(-b.X);
			}
			break;

		case Direction::East:
			for (const Point2& p : points)
			{
				if ((p.X > b.X) && (p.X < c.X))
				{
					northWalls[p.X].insert(-b.Y);
				}
			}
			if (isConvex)
			{
				westWalls[b.Y].insert(-b.X);
				northWalls[b.X].insert(-b.Y);
			}
			break;

		case Direction::South:
			for (const Point2& p : points)
			{
				if ((p.Y > b.Y) && (p.Y < c.Y))
				{
					eastWalls[p.Y].insert(b.X);
				}
			}
			if (isConvex)
			{
				northWalls[b.X].insert(-b.Y);
				eastWalls[b.Y].insert(b.X);
			}
			break;

		case Direction::West:
			for (const Point2& p : points)
			{
				if ((p.X < b.X) && (p.X > c.X))
				{
					southWalls[p.X].insert(b.Y);
				}
			}
			if (isConvex)
			{
				eastWalls[b.Y].insert(b.X);
				southWalls[b.X].insert(b.Y);
			}
			break;

		default:
			assert(false);
		}
	}

	//
	// Find the nearest wall for each point
	//

	vector<int64_t> pointDistanceToNorthWall(points.size(), -1);
	vector<int64_t> pointDistanceToEastWall(points.size(), -1);
	vector<int64_t> pointDistanceToSouthWall(points.size(), -1);
	vector<int64_t> pointDistanceToWestWall(points.size(), -1);

	for (int64_t i = 0; i < (int64_t)points.size(); i++)
	{
		const Point2& p = points[i];

		{
			const auto it = northWalls[p.X].lower_bound(-p.Y);
			pointDistanceToNorthWall[i] = (it == northWalls[p.X].end()) ? 0 : (*it + p.Y) + 1;
		}
		{
			const auto it = eastWalls[p.Y].lower_bound(p.X);
			pointDistanceToEastWall[i] = (it == eastWalls[p.Y].end()) ? 0 : (*it - p.X) + 1;
		}
		{
			const auto it = southWalls[p.X].lower_bound(p.Y);
			pointDistanceToSouthWall[i] = (it == southWalls[p.X].end()) ? 0 : (*it - p.Y) + 1;
		}
		{
			const auto it = westWalls[p.Y].lower_bound(-p.X);
			pointDistanceToWestWall[i] = (it == westWalls[p.Y].end()) ? 0 : (*it + p.X) + 1;
		}
	}

	//
	// Find the largest valid rectangle
	//

	int64_t answer = 0;
	for (const auto& p : AllUnorderedPairs(points.size()))
	{
		const int64_t aIndex = p.first;
		const int64_t bIndex = p.second;

		// A -> B
		const Point2& a = points[aIndex];
		const Point2& b = points[bIndex];

		const int64_t potentialWidth = abs(b.X - a.X) + 1;
		const int64_t potentialHeight = abs(b.Y - a.Y) + 1;

		if (b.Y < a.Y)
		{
			if (min(pointDistanceToNorthWall[aIndex], pointDistanceToSouthWall[bIndex]) < potentialHeight)
				continue;
		}
		else if (b.Y > a.Y)
		{
			if (min(pointDistanceToSouthWall[aIndex], pointDistanceToNorthWall[bIndex]) < potentialHeight)
				continue;
		}

		if (b.X > a.X)
		{
			if (min(pointDistanceToEastWall[aIndex], pointDistanceToWestWall[bIndex]) < potentialWidth)
				continue;
		}
		else if (b.X < a.X)
		{
			if (min(pointDistanceToWestWall[aIndex], pointDistanceToEastWall[bIndex]) < potentialWidth)
				continue;
		}

		answer = max(answer, potentialWidth * potentialHeight);
	}

	printf("[2025] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2025(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
