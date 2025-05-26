#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2023_Types
{
	struct Space
	{
		int64_t Width;
		int64_t Height;

		vector<Point2> OriginalStars;

		set<int64_t> EmptyRows;
		set<int64_t> EmptyColumns;

		vector<Point2> ExpandedStars;
	};
}

using namespace Puzzle11_2023_Types;

static Space ReadSpace(istream& input, int64_t expandBy)
{
	Space s;

	vector<string> lines = ReadAllLines(input);
	s.Width = lines[0].size();
	s.Height = lines.size();

	set<int64_t> xCoords;
	set<int64_t> yCoords;
	for (int64_t y = 0; y < s.Height; y++)
	{
		for (int64_t x = 0; x < s.Width; x++)
		{
			if (lines[y][x] == '#')
			{
				s.OriginalStars.push_back(Point2{ x, y });
				xCoords.insert(x);
				yCoords.insert(y);
			}
		}
	}

	s.EmptyRows = Enumerable::Range(0ll, s.Height)
		->Where([&yCoords](int64_t y) { return yCoords.contains(y) == false; })
		->ToSet();

	s.EmptyColumns = Enumerable::Range(0ll, s.Width)
		->Where([&xCoords](int64_t x) { return xCoords.contains(x) == false; })
		->ToSet();

	s.ExpandedStars = MakeEnumerator(s.OriginalStars)
		->Select<Point2>([&s, expandBy](const Point2& p)
			{
				int64_t xExpansion = distance(
					s.EmptyColumns.begin(),
					s.EmptyColumns.upper_bound(p.X));

				int64_t yExpansion = distance(
					s.EmptyRows.begin(),
					s.EmptyRows.upper_bound(p.Y));

				return Point2{ p.X + (xExpansion * expandBy), p.Y + (yExpansion * expandBy) };
			})
		->ToVector();

	return s;
}

static void Puzzle11_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Space s = ReadSpace(input, 1);

	int64_t answer = 0;
	for (size_t i = 0; i < s.ExpandedStars.size(); i++)
	{
		for (size_t j = i + 1; j < s.ExpandedStars.size(); j++)
		{
			Point2 diff = s.ExpandedStars[j] - s.ExpandedStars[i];
			int64_t distance = abs(diff.X) + abs(diff.Y);
			answer += distance;
		}
	}

	printf("[2023] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Space s = ReadSpace(input, 1000000 - 1);

	int64_t answer = 0;
	for (size_t i = 0; i < s.ExpandedStars.size(); i++)
	{
		for (size_t j = i + 1; j < s.ExpandedStars.size(); j++)
		{
			Point2 diff = s.ExpandedStars[j] - s.ExpandedStars[i];
			int64_t distance = abs(diff.X) + abs(diff.Y);
			answer += distance;
		}
	}

	printf("[2023] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2023(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
