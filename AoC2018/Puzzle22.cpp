#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2018_Types
{
	static const char Rocky = '.';
	static const char Wet = '=';
	static const char Narrow = '|';

	static const int64_t Neither = 0;
	static const int64_t Torch = 1;
	static const int64_t ClimbingGear = 2;
}

using namespace Puzzle22_2018_Types;

static pair<int64_t, Point2> ParsePuzzle(istream& input)
{
	int64_t depth = -1;
	int scanned = sscanf(ReadSingleLine(input).c_str(), "depth: %lld", &depth);
	assert(scanned == 1);
	(void)scanned;

	Point2 target;
	scanned = sscanf(ReadSingleLine(input).c_str(), "target: %lld,%lld", &target.X, &target.Y);
	assert(scanned == 2);
	(void)scanned;

	return { depth, target };
}

static ArrayMap2D CreateCave(int64_t depth, const Point2 &target, const Point2&padding)
{
	Point2 caveSize = target + padding;

	ArrayMap2D cave({}, caveSize.X + 1, caveSize.Y + 1, ' ');
	vector<vector<int64_t>> erosionLevels(caveSize.Y + 1);
	ranges::fill(erosionLevels, (vector<int64_t>(caveSize.X + 1)));

	const array<char,3> terrain = { Rocky, Wet, Narrow };

	for (const Point2& region : GridRange{ {}, caveSize + Point2{ 1, 1 } })
	{
		int64_t geologicIndex = -1;
		if (region == Point2{ 0, 0 })
		{
			geologicIndex = 0;
		}
		else if (region == target)
		{
			geologicIndex = 0;
		}
		else if (region.Y == 0)
		{
			geologicIndex = region.X * 16807;
		}
		else if (region.X == 0)
		{
			geologicIndex = region.Y * 48271;
		}
		else
		{
			geologicIndex = erosionLevels[region.Y][region.X - 1] * erosionLevels[region.Y - 1][region.X];
		}

		int64_t erosionLevel = (geologicIndex + depth) % 20183;
		erosionLevels[region.Y][region.X] = erosionLevel;

		cave(region) = terrain[erosionLevel % terrain.size()];
	}

	return cave;
}

static pair<bool, int64_t> CheckMove(const ArrayMap2D& cave, const Vector3& from, const Vector3& to)
{
	assert(abs(from.X - to.X) <= 1);
	assert(abs(from.Y - to.Y) <= 1);

	if (cave.IsInside({ to.X, to.Y }) == false)
	{
		// Can't move off the map
		return { false, -1 };
	}

	if (to == from)
	{
		// No point doing nothing
		return { false, -1 };
	}

	char currentRegion = cave({ from.X, from.Y });
	char nextRegion = cave({ to.X, to.Y });

	// Check that any equipment is valid for both regions
	static array<char, 3> incompatibleRegion =
	{
		Rocky,	// Neither
		Wet,	// Torch
		Narrow,	// ClimbingGear
	};
	bool incompatibleEquipment = ranges::any_of(array<char, 2>{ currentRegion, nextRegion },
		[&](const char &region) -> bool
		{
			return (region == incompatibleRegion[from.Z]) || (region == incompatibleRegion[to.Z]);
		});
	if (incompatibleEquipment)
	{
		return { false, -1 };
	}

	// Move is within bounds and has compatible equipment
	return { true, from.Z == to.Z ? 1 : 7 };
}

static int64_t FindQuickestPath(const ArrayMap2D& cave, const Point2& startingLocation, const Point2& target)
{
	set<pair<int64_t, Vector3>> searchQueue{ { 0, { startingLocation.X, startingLocation.Y, Torch } } };
	const Vector3 targetState{ target.X, target.Y, Torch };

	set<Vector3> visited;
	while (searchQueue.empty() == false)
	{
		auto [currentTime, currentState] = *searchQueue.begin();
		searchQueue.erase(searchQueue.begin());

		if (currentState == targetState)
		{
			return currentTime;
		}

		if (visited.contains(currentState))
		{
			continue;
		}

		visited.insert(currentState);

		for (const Point2 dir : Point2::CardinalDirections())
		{
			Vector3 nextState{ currentState.X + dir.X, currentState.Y + dir.Y, currentState.Z };
			auto [possible, nextCost] = CheckMove(cave, currentState, nextState);
			if (possible)
			{
				searchQueue.insert({ currentTime + nextCost, nextState });
			}
		}
		for (int64_t equipment : array<int64_t, 3>{ Neither, Torch, ClimbingGear })
		{
			Vector3 nextState{ currentState.X, currentState.Y, equipment };
			auto [possible, nextCost] = CheckMove(cave, currentState, nextState);
			if (possible)
			{
				searchQueue.insert({ currentTime + nextCost, nextState });
			}
		}
	}

	return -1;
}

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [depth, target] = ParsePuzzle(input);
	ArrayMap2D cave = CreateCave(depth, target, Point2{});

	int64_t answer =
		cave.Count(Rocky) * 0 +
		cave.Count(Wet) * 1 +
		cave.Count(Narrow) * 2;

	printf("[2018] Puzzle22_A: %" PRId64 "\n", answer);
}


static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [depth, target] = ParsePuzzle(input);
	ArrayMap2D cave = CreateCave(depth, target, Point2{ 50, 50 });

	int64_t answer = FindQuickestPath(cave, {}, target);

	printf("[2018] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2018(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
