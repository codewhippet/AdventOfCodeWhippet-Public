#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2018_Types
{
}

using namespace Puzzle17_2018_Types;

static vector<pair<Point2, Point2>> ParseScan(istream& input)
{
	vector<pair<Point2, Point2>> scan;
	for (const auto& slice : ScanfEachLine<char, int64_t, char, int64_t, int64_t>(input, "%c=%lld, %c=%lld..%lld"))
	{
		if (get<0>(slice) == 'x')
		{
			scan.push_back({ { get<1>(slice), get<3>(slice) }, { get<1>(slice), get<4>(slice) } });
		}
		else
		{
			scan.push_back({ { get<3>(slice), get<1>(slice) }, { get<4>(slice), get<1>(slice) } });
		}
	}
	return scan;
}

static ArrayMap2D BuildMap(const vector<pair<Point2, Point2>>& scan)
{
	auto minMaxPairs = scan | views::transform([](const auto& p) -> pair<Point2, Point2> { return { Point2::MinElements(p.first, p.second), Point2::MaxElements(p.first, p.second) }; });
	auto [topLeft, bottomRight] = accumulate(minMaxPairs.begin(), minMaxPairs.end(), scan[0],
		[](const auto& a, const auto& b) -> pair<Point2, Point2>
		{
			return { Point2::MinElements(a.first, b.first), Point2::MaxElements(a.second, b.second) };
		});

	topLeft.X -= 1;
	bottomRight.X += 1;

	Point2 dimensions = bottomRight - topLeft + Point2{ 1, 1 };

	ArrayMap2D ground(topLeft, dimensions.X, dimensions.Y, '.');
	for (const auto& s : scan)
	{
		for (const Point2& p : LineInclusiveRange{ s.first, s.second })
		{
			ground(p) = '#';
		}
	}

	return ground;
}

static void FillWithWater(const Point2& water, ArrayMap2D* ground)
{
	if (ground->IsInside(water) == false)
	{
		return;
	}

	Point2 below = water + Point2::Down();
	if (ground->IsInside(below) == false)
	{
		(*ground)(water) = '|';
		return;
	}

	// First allow the water to fill up underneath us
	char groundBelow = (*ground)(below);
	if (groundBelow == '.')
	{
		FillWithWater(below, ground);
	}

	groundBelow = (*ground)(below);
	if (groundBelow == '|')
	{
		(*ground)(water) = '|';
		return;
	}

	const array<Point2, 2> directions{ Point2::Left(), Point2::Right() };
	array<Point2, 2> extents{ water, water };
	array<bool, 2> isBlocked{ false, false };

	for (size_t i = 0; i < directions.size(); i++)
	{
		while (true)
		{
			if ((*ground)(extents[i] + Point2::Down()) == '.')
			{
				// Another space to fill
				FillWithWater(extents[i] + Point2::Down(), ground);

				if ((*ground)(extents[i] + Point2::Down()) == '|')
				{
					// Waterfall
					isBlocked[i] = false;
					break;
				}
			}

			if ((*ground)(extents[i] + directions[i]) == '|')
			{
				// Hit an existing water stream
				isBlocked[i] = false;
				break;
			}

			if ((*ground)(extents[i] + directions[i]) == '#')
			{
				// Hit a wall
				isBlocked[i] = true;
				break;
			}

			extents[i] = extents[i] + directions[i];
		}
	}

	if (ranges::all_of(isBlocked, [](bool b) { return b; }))
	{
		for (const Point2& p : LineInclusiveRange{ extents[0], extents[1] })
		{
			(*ground)(p) = '~';
		}
	}
	else
	{
		for (const Point2& p : LineInclusiveRange{ extents[0], extents[1] })
		{
			(*ground)(p) = '|';
		}
	}
}

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Point2, Point2>> scan = ParseScan(input);
	ArrayMap2D ground = BuildMap(scan);
	FillWithWater({ 500, ground.GetOrigin().Y }, &ground);

	int64_t answer = ground.Count('~') + ground.Count('|');

	printf("[2018] Puzzle17_A: %" PRId64 "\n", answer);
}


static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Point2, Point2>> scan = ParseScan(input);
	ArrayMap2D ground = BuildMap(scan);
	FillWithWater({ 500, ground.GetOrigin().Y }, &ground);

	int64_t answer = ground.Count('~');

	printf("[2018] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2018(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
