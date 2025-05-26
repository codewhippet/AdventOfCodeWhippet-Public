#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2021_Types
{
}

using namespace Puzzle05_2021_Types;

static void Puzzle05_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> coords = ReadAllLines(input);

	map<pair<int, int>, int> oceanFloor;
	for (vector<string>::const_reference coord : coords)
	{
		pair<int, int> start;
		pair<int, int> stop;
		int parsed = sscanf(coord.c_str(), "%d,%d -> %d,%d",
			&start.first, &start.second,
			&stop.first, &stop.second);
		if (parsed != 4)
		{
			continue;
		}

		vector<pair<int, int>> lineElements;
		if (start.first != stop.first && start.second == stop.second)
		{
			// Horizontal
			int first = min(start.first, stop.first);
			int last = max(start.first, stop.first);
			lineElements = Enumerable::Range(first, last - first + 1)
				->Select<pair<int, int>>([column = start.second](const int& row) { return make_pair(row, column); })
				->ToVector();
		}

		if (start.first == stop.first && start.second != stop.second)
		{
			// Vertical
			int first = min(start.second, stop.second);
			int last = max(start.second, stop.second);
			lineElements = Enumerable::Range(first, last - first + 1)
				->Select<pair<int, int>>([row = start.first](const int& column) { return make_pair(row, column); })
				->ToVector();
		}

		for (vector<pair<int, int>>::const_reference element : lineElements)
		{
			oceanFloor[element]++;
		}
	}

	int64_t answer = (int)count_if(oceanFloor.begin(), oceanFloor.end(),
		[](map<pair<int, int>, int>::const_reference floorElement)
		{
			return floorElement.second > 1;
		});

	printf("[2021] Puzzle05_A: %" PRId64 "\n", answer);
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> coords = ReadAllLines(input);

	map<Point2, int> oceanFloor;
	for (vector<string>::const_reference coord : coords)
	{
		Point2 start;
		Point2 stop;
		int parsed = sscanf(coord.c_str(), "%" SCNd64 ",%" SCNd64 " -> %" SCNd64 ",%" SCNd64,
			&start.X, &start.Y,
			&stop.X, &stop.Y);
		if (parsed != 4)
		{
			continue;
		}

		vector<Point2> lineElements = Enumerable::Line(start, stop)->ToVector();
		for (vector<Point2>::const_reference element : lineElements)
		{
			oceanFloor[element]++;
		}
	}

	int64_t answer = (int)count_if(oceanFloor.begin(), oceanFloor.end(),
		[](map<Point2, int>::const_reference floorElement)
		{
			return floorElement.second > 1;
		});

	printf("[2021] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2021(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
