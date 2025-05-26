#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2024_Types
{
}

using namespace Puzzle08_2024_Types;

static void Puzzle08_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap antennaMap = ReadPointMap(input);

	multimap<char, Point2> antennas;
	set<Point2> antinodes;
	for (map<Point2, char>::const_reference newAntenna : antennaMap.Data)
	{
		auto existingAntennas = antennas.equal_range(newAntenna.second);
		for (const auto& existingAntenna : ranges::subrange(existingAntennas.first, existingAntennas.second))
		{
			Point2 diff = newAntenna.first - existingAntenna.second;
			antinodes.insert(existingAntenna.second - diff);
			antinodes.insert(newAntenna.first + diff);
		}
		antennas.insert(make_pair(newAntenna.second, newAntenna.first));
	}

	int64_t answer = ranges::count_if(antinodes,
		[&antennaMap](set<Point2>::const_reference node)
		{
			return IsInside(node, antennaMap);
		});

	printf("[2024] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap antennaMap = ReadPointMap(input);

	multimap<char, Point2> antennas;
	set<Point2> antinodes;
	for (map<Point2, char>::const_reference newAntenna : antennaMap.Data)
	{
		auto existingAntennas = antennas.equal_range(newAntenna.second);
		for (const auto& existingAntenna : ranges::subrange(existingAntennas.first, existingAntennas.second))
		{
			Point2 diff = newAntenna.first - existingAntenna.second;
			for (int64_t i = 0; i < max(antennaMap.Size.X, antennaMap.Size.Y); i++)
			{
				antinodes.insert(existingAntenna.second - diff * i);
				antinodes.insert(newAntenna.first + diff * i);
			}
		}
		antennas.insert(make_pair(newAntenna.second, newAntenna.first));
	}

	int64_t answer = ranges::count_if(antinodes,
		[&antennaMap](set<Point2>::const_reference node)
		{
			return IsInside(node, antennaMap);
		});

	printf("[2024] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2024(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
