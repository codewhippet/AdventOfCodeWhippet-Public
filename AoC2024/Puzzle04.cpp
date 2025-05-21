#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2024_Types
{
}

using namespace Puzzle04_2024_Types;

static int64_t FindXmas(const PointMap& searchGrid, char startingLetter, const vector<pair<char, Point2>>& offsets)
{
	int64_t locations = 0;

	for (decltype(PointMap::Data)::const_reference p : searchGrid.Data)
	{
		if (p.second == startingLetter)
		{
			int64_t matches = ranges::count_if(offsets,
				[start = p.first, &searchGrid](const pair<char, Point2>& offset) -> bool
				{
					decltype(PointMap::Data)::const_iterator candidate = searchGrid.Data.find(start + offset.second);
					return (candidate != searchGrid.Data.end()) && (candidate->second == offset.first);
				});
			locations += (matches == (int64_t)offsets.size() ? 1 : 0);
		}
	}

	return locations;
}

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	PointMap searchGrid = ReadPointMap(input);

	const vector<pair<char, Point2>> horizontalOffsets
	{
		{ 'M', Point2::Right() * 1 },
		{ 'A', Point2::Right() * 2 },
		{ 'S', Point2::Right() * 3 },
	};

	const vector<pair<char, Point2>> diagonalOffsets
	{
		{ 'M', (Point2::Down() + Point2::Right()) * 1 },
		{ 'A', (Point2::Down() + Point2::Right()) * 2 },
		{ 'S', (Point2::Down() + Point2::Right()) * 3 },
	};

	for (int i = 0; i < 4; i++)
	{
		answer += FindXmas(searchGrid, 'X', horizontalOffsets);
		answer += FindXmas(searchGrid, 'X', diagonalOffsets);

		searchGrid = RotateClockwise(searchGrid);
	}

	printf("[2024] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	PointMap searchGrid = ReadPointMap(input);

	const vector<pair<char, Point2>> offsets
	{
		{ 'M', Point2::Up() + Point2::Left() },
		{ 'M', Point2::Up() + Point2::Right() },
		{ 'S', Point2::Down() + Point2::Left() },
		{ 'S', Point2::Down() + Point2::Right() },
	};

	for (int i = 0; i < 4; i++)
	{
		answer += FindXmas(searchGrid, 'A', offsets);

		searchGrid = RotateClockwise(searchGrid);
	}

	printf("[2024] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2024(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
