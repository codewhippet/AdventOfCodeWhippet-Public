#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2025_Types
{
}

using namespace Puzzle07_2025_Types;

static void Puzzle07_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D tree = ReadArrayMap(input);

	for (const auto& p : tree.Grid())
	{
		char charAbove = tree(p.first + Point2::North());
		bool laserAbove = (charAbove == '|') || (charAbove == 'S');
		switch (p.second)
		{
		case '.':
			tree(p.first) = laserAbove ? '|' : '.';
			break;

		case '^':
			if (laserAbove)
			{
				tree(p.first + Point2::Left()) = '|';
				tree(p.first) = 'O';
				tree(p.first + Point2::Right()) = '|';
			}
			break;
		}
	}

	int64_t answer = tree.Count('O');

	printf("[2025] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D tree = ReadArrayMap(input);

	vector<vector<int64_t>> particleCounts(tree.GetHeight() + 1, vector<int64_t>(tree.GetWidth() + 2)); // Plus padding
	for (const auto& p : tree.Grid())
	{
		const Point2 here = p.first + Point2::Right(); // Offset for padding

		const int64_t particlesHere = particleCounts[here.Y][here.X];
		switch (p.second)
		{
		case 'S':
			particleCounts[here.Y + 1][here.X] = 1;
			break;

		case '^':
			particleCounts[here.Y + 1][here.X - 1] += particlesHere;
			particleCounts[here.Y + 1][here.X + 1] += particlesHere;
			break;

		default:
			particleCounts[here.Y + 1][here.X] += particlesHere;
			break;
		}
	}

	const int64_t lastLine = tree.GetHeight() - 1;
	int64_t answer = ranges::fold_left(particleCounts[lastLine], 0, plus{});

	printf("[2025] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2025(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
