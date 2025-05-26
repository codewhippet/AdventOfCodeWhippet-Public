#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2020_Types
{
}

using namespace Puzzle03_2020_Types;


static void Puzzle03_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> trees = ReadAllLines(input);
	size_t toboggan = 0;
	for (size_t row = 0; row < trees.size(); row++)
	{
		answer += (trees[row][toboggan] == '#' ? 1 : 0);
		toboggan = (toboggan + 3) % trees[row].size();
	}

	printf("[2020] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> trees = ReadAllLines(input);

	const vector<Point2> slopes =
	{
		Point2{ 1, 1 },
		Point2{ 3, 1 },
		Point2{ 5, 1 },
		Point2{ 7, 1 },
		Point2{ 1, 2 },
	};

	vector<int64_t> counts;
	for (const Point2& slope : slopes)
	{
		int64_t treesEncountered = 0;

		size_t toboggan = 0;
		for (size_t row = 0; row < trees.size(); row += slope.Y)
		{
			treesEncountered += (trees[row][toboggan] == '#' ? 1 : 0);
			toboggan = (toboggan + slope.X) % trees[row].size();
		}

		counts.push_back(treesEncountered);
	}

	int64_t answer = 1;
	for (int64_t c : counts)
	{
		answer *= c;
	}

	printf("[2020] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2020(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
