#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2022_Types
{
}

using namespace Puzzle17_2022_Types;

static vector<Point2> Spawn(int64_t rockIndex, int64_t height)
{
	static const vector<vector<Point2>> shapes
	{
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
		{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
		{ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }
	};

	return shapes[rockIndex % shapes.size()] | views::transform([&](const auto &p) { return p + Point2{ 3, height }; }) | ranges::to<vector>();
}

static vector<Point2> MoveWithWind(const vector<Point2>& shape, char wind)
{
	Point2 move = (wind == '>' ? Point2{ 1, 0 } : Point2{ -1, 0 });
	vector<Point2> newShape = shape | views::transform([&](const auto& p) { return p + move; }) | ranges::to<vector>();

	// Don't move beyond walls
	auto [minX, maxX] = ranges::minmax(newShape | views::transform(&Point2::GetX));
	if ((minX <= 0) || (maxX >= 8))
	{
		return shape;
	}

	return newShape;
}

static vector<Point2> MoveWithGravity(const vector<Point2>& shape)
{
	return shape | views::transform([&](const auto& p) { return p - Point2{ 0, 1 }; }) | ranges::to<vector>();
}

static void DropRock(int64_t rockIndex,
	const string& windCurrents,
	int64_t* maxHeight,
	int64_t* windCurrent,
	set<Point2>* settledRocks)
{
	vector<Point2> movingRock = Spawn(rockIndex, *maxHeight + 4);
	while (true)
	{
		char windThisFrame = windCurrents[*windCurrent];
		*windCurrent = (*windCurrent + 1) % windCurrents.size();

		vector<Point2> rockAfterWind = MoveWithWind(movingRock, windThisFrame);
		if (ranges::any_of(rockAfterWind, [&](const auto& p) { return settledRocks->contains(p); }))
		{
			// Rock doesn't move if we hit another rock
			rockAfterWind = movingRock;
		}

		vector<Point2> rockAfterGravity = MoveWithGravity(rockAfterWind);
		if (ranges::any_of(rockAfterGravity, [&](const auto& p) { return settledRocks->contains(p); }) ||
			ranges::any_of(rockAfterGravity, [&](const auto& p) { return p.Y == 0; }))
		{
			// Rock finishes moving if we hit another rock or the floor
			ranges::copy(rockAfterWind, inserter(*settledRocks, settledRocks->end()));
			*maxHeight = max(*maxHeight, ranges::max(rockAfterWind | views::transform(&Point2::GetY)));
			return;
		}

		movingRock = rockAfterGravity;
	}
}

static void Puzzle17_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string windCurrents = ReadSingleLine(input);

	set<Point2> settledRocks;

	int64_t maxHeight = 0;

	int64_t windCurrent = 0;
	for (int64_t rockIndex = 0; rockIndex < 2022; rockIndex++)
	{
		DropRock(rockIndex, windCurrents, &maxHeight, &windCurrent, &settledRocks);
	}

	int64_t answer = maxHeight;

	printf("[2022] Puzzle17_A: %" PRId64 "\n", answer);
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string windCurrents = ReadSingleLine(input);

	vector<int64_t> tallRockLandsOnTop;
	{
		set<Point2> settledRocks;

		int64_t maxHeight = 0;
		int64_t windCurrent = 0;

		for (int64_t rockIndex = 0; rockIndex < 10000; rockIndex++)
		{
			int64_t oldMax = maxHeight;
			DropRock(rockIndex, windCurrents, &maxHeight, &windCurrent, &settledRocks);

			// Record the times we get the biggest possible increments
			if ((maxHeight - oldMax) == 4)
			{
				tallRockLandsOnTop.push_back(rockIndex);
			}
		}
	}

	vector<int64_t> rockIndexDiffs = tallRockLandsOnTop | views::slide(2) | views::transform([](const auto& s) { return s.back() - s.front(); }) | ranges::to<vector>();
	ranges::reverse(rockIndexDiffs);

	// Look for a repeat length
	int64_t repeatLength = 0;
	for (int64_t i = 10; i < (int64_t)rockIndexDiffs.size() / 2; i++)
	{
		bool repeatFound = ranges::equal(
			rockIndexDiffs | views::take(i),
			rockIndexDiffs | views::drop(i) | views::take(i));
		if (repeatFound)
		{
			repeatLength = ranges::fold_left(rockIndexDiffs | views::take(i), 0ll, plus{});
			break;
		}
	}
	assert(repeatLength > 0);

	// Resimulate to get the height increment per repeat and a starting height offset
	set<Point2> settledRocks;

	int64_t maxHeight = 0;
	int64_t windCurrent = 0;

	// Repeat a few times to let it settle
	int64_t idealCycles = 1000000000000 - (repeatLength * 3);
	int64_t sectionRepeats = idealCycles / repeatLength;
	int64_t simulationCycles = 1000000000000 - (repeatLength * sectionRepeats);

	vector<int64_t> heights;
	for (int64_t rockIndex = 0; rockIndex < simulationCycles; rockIndex++)
	{
		DropRock(rockIndex, windCurrents, &maxHeight, &windCurrent, &settledRocks);
		if ((rockIndex % repeatLength) == 0)
		{
			heights.push_back(maxHeight);
		}
	}

	vector<int64_t> heightDiffs = heights | views::slide(2) | views::transform([](const auto& s) { return s.back() - s.front(); }) | ranges::to<vector>();

	int64_t answer = maxHeight + (sectionRepeats * heightDiffs.back());

	printf("[2022] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2022(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
