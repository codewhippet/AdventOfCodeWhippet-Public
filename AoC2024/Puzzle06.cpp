#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2024_Types
{
	static map<Point2, Point2> Rotate =
	{
		{ Point2::North(), Point2::East() },
		{ Point2::East(), Point2::South() },
		{ Point2::South(), Point2::West() },
		{ Point2::West(), Point2::North() },
	};
}

using namespace Puzzle06_2024_Types;

static bool DoesLoop(Point2 guardPosition, Point2 guardDirection, const PointMap& lab)
{
	set<pair<Point2, Point2>> visited;

	// Walk the guard until they've left the map
	while (IsInside(guardPosition, lab))
	{
		auto guardState = make_pair(guardPosition, guardDirection);
		if (visited.contains(guardState))
		{
			return true;
		}

		visited.insert(guardState);

		if (lab.Data.contains(guardPosition + guardDirection))
		{
			guardDirection = Rotate[guardDirection];
		}
		else
		{
			guardPosition = guardPosition + guardDirection;
		}
	}

	return false;
}

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap map = ReadPointMap(input);

	// Find the guard's starting location
	Point2 guard;
	for (decltype(PointMap::Data)::iterator locationIt = map.Data.begin(), end = map.Data.end();
		locationIt != end; ++locationIt)
	{
		if (locationIt->second == '^')
		{
			guard = locationIt->first;
			map.Data.erase(locationIt);
			break;
		}
	}

	set<Point2> visited;

	// Walk the guard until they've left the map
	Point2 guardDirection = Point2::North();
	while (IsInside(guard, map))
	{
		visited.insert(guard);
		if (map.Data.contains(guard + guardDirection))
		{
			guardDirection = Rotate[guardDirection];
		}
		else
		{
			guard = guard + guardDirection;
		}
	}

	int64_t answer = visited.size();

	printf("[2024] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap lab = ReadPointMap(input);

	// Find the guard's starting location
	Point2 guardStartingPosition;
	for (decltype(PointMap::Data)::iterator locationIt = lab.Data.begin(), end = lab.Data.end();
		locationIt != end; ++locationIt)
	{
		if (locationIt->second == '^')
		{
			guardStartingPosition = locationIt->first;
			lab.Data.erase(locationIt);
			break;
		}
	}

	assert(DoesLoop(guardStartingPosition, Point2::North(), lab) == false);

	map<pair<Point2, Point2>, int> visited;

	// Walk the guard until they've left the map
	Point2 guardPosition = guardStartingPosition;
	Point2 guardDirection = Point2::North();
	int guardStep = 0;
	while (IsInside(guardPosition, lab))
	{
		auto guardState = make_pair(guardPosition, guardDirection);
		assert(visited.contains(guardState) == false);
		visited.insert(make_pair(guardState, guardStep));
		if (lab.Data.contains(guardPosition + guardDirection))
		{
			guardDirection = Rotate[guardDirection];
		}
		else
		{
			guardPosition = guardPosition + guardDirection;
			guardStep++;
		}
	}

	set<Point2> loopingBlockers;
	set<Point2> tested;

	// Run along the route looking for places to put a blocking piece
	for (const auto& guardLocation : visited)
	{
		Point2 candidateBlockingPiece = guardLocation.first.first + guardLocation.first.second;

		// Can only put pieces in empty spaces
		if (lab.Data.contains(candidateBlockingPiece))
			continue;

		// Don't re-test candidates
		if (tested.contains(candidateBlockingPiece))
			continue;

		lab.Data.insert({ candidateBlockingPiece, '#' });
		if (DoesLoop(guardStartingPosition, Point2::North(), lab))
		{
			loopingBlockers.insert(candidateBlockingPiece);
		}
		lab.Data.erase(candidateBlockingPiece);

		tested.insert(candidateBlockingPiece);
	}

	int64_t answer = loopingBlockers.size();

	printf("[2024] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2024(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
