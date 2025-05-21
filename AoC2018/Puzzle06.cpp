#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2018_Types
{
}

using namespace Puzzle06_2018_Types;

static map<size_t, int64_t> GetRegionCounts(const Point2& topLeft, const Point2& bottomRight, const vector<Point2> coords)
{
	map<size_t, int64_t> regionCounts;
	for (const auto& p : GridRange{ topLeft, bottomRight + Point2{ 1, 1 } })
	{
		map<int64_t, set<int64_t>> distances;
		for (size_t i = 0; i < coords.size(); i++)
		{
			distances[ManhattanDistance(p, coords[i])].insert(i);
		}
		int64_t closestUnique = distances.begin()->second.size() == 1 ? *distances.begin()->second.begin() : -1;
		if (closestUnique != -1)
		{
			regionCounts[closestUnique]++;
		}
	}
	return regionCounts;
}

static void Puzzle06_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Point2> coords;
	Point2 topLeft{ numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max() };
	Point2 bottomRight{ numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min() };
	for (const auto& line : ScanfEachLine<int64_t, int64_t>(input, "%lld, %lld"))
	{
		Point2 coord{ get<0>(line), get<1>(line) };

		topLeft.X = min(topLeft.X, coord.X);
		topLeft.Y = min(topLeft.Y, coord.Y);

		bottomRight.X = max(bottomRight.X, coord.X);
		bottomRight.Y = max(bottomRight.Y, coord.Y);

		coords.push_back(coord);
	}

	map<size_t, int64_t> regions1 = GetRegionCounts(topLeft, bottomRight, coords);
	map<size_t, int64_t> regions2 = GetRegionCounts(topLeft - Point2{ 1, 1 }, bottomRight + Point2{ 1, 1 }, coords);

	MostCounter<int64_t> finiteRegionSizes;
	ranges::for_each(regions1 | views::filter([&](const auto& kvp) { return regions2.at(kvp.first) == kvp.second; }),
		[&](const auto& regionSize)
		{
			finiteRegionSizes.AddCount(regionSize.first, regionSize.second);
		});

	int64_t answer = finiteRegionSizes.GetMostCount();

	printf("[2018] Puzzle06_A: %" PRId64 "\n", answer);
}


static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Point2> coords;
	Point2 topLeft{ numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max() };
	Point2 bottomRight{ numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min() };
	for (const auto& line : ScanfEachLine<int64_t, int64_t>(input, "%lld, %lld"))
	{
		Point2 coord{ get<0>(line), get<1>(line) };

		topLeft.X = min(topLeft.X, coord.X);
		topLeft.Y = min(topLeft.Y, coord.Y);

		bottomRight.X = max(bottomRight.X, coord.X);
		bottomRight.Y = max(bottomRight.Y, coord.Y);

		coords.push_back(coord);
	}

	int64_t fieldSize = 0;
	for (const auto& p : GridRange{ Point2{ -1000, -1000 }, Point2{ 1000, 1000 } })
	{
		int64_t distance = 0;
		for (size_t i = 0; i < coords.size(); i++)
		{
			distance += ManhattanDistance(p, coords[i]);
		}
		if (distance < 10000)
		{
			fieldSize++;
		}
	}

	int64_t answer = fieldSize;

	printf("[2018] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2018(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
