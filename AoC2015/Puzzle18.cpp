#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2015_Types
{
}

using namespace Puzzle18_2015_Types;

static void SetCorners(ArrayMap2D* board)
{
	(*board)({ 0, 0 }) = '#';
	(*board)({ board->GetWidth() - 1, 0 }) = '#';
	(*board)({ 0, board->GetHeight() - 1 }) = '#';
	(*board)({ board->GetWidth() - 1, board->GetHeight() - 1 }) = '#';
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int iterations = 100;

	vector<ArrayMap2D> boards;
	boards.emplace_back(ReadArrayMap(input));
	boards.emplace_back(ArrayMap2D{ ArrayMap2DOptions::CloneAsUninitialised, boards[0] });

	for (int i = 0; i < iterations; i++)
	{
		ArrayMap2D& activeBoard = boards[i % 2];
		ArrayMap2D& nextBoard = boards[1 - (i % 2)];

		for (const auto kvp : activeBoard.Grid())
		{
			int neighboursAlive = 0;
			for (const Point2 d : Point2::CardinalAndDiagonalDirections())
			{
				if (activeBoard(kvp.first + d) == '#')
				{
					neighboursAlive++;
				}
			}

			if (kvp.second == '#')
			{
				// A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
				nextBoard(kvp.first) = (neighboursAlive == 2 || neighboursAlive == 3) ? '#' : '.';
			}
			else
			{
				// A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.
				nextBoard(kvp.first) = (neighboursAlive == 3) ? '#' : '.';
			}
		}
	}

	int64_t answer = 0;
	for (const auto kvp : boards[iterations % 2].Grid())
	{
		if (kvp.second == '#')
		{
			answer++;
		}
	}

	printf("[2015] Puzzle18_A: %" PRId64 "\n", answer);
}


static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int iterations = 100;

	vector<ArrayMap2D> boards;
	boards.emplace_back(ReadArrayMap(input));
	boards.emplace_back(ArrayMap2D{ ArrayMap2DOptions::CloneAsUninitialised, boards[0] });

	SetCorners(&boards[0]);

	for (int i = 0; i < iterations; i++)
	{
		ArrayMap2D& activeBoard = boards[i % 2];
		ArrayMap2D& nextBoard = boards[1 - (i % 2)];

		for (const auto kvp : activeBoard.Grid())
		{
			int neighboursAlive = 0;
			for (const Point2 d : Point2::CardinalAndDiagonalDirections())
			{
				if (activeBoard(kvp.first + d) == '#')
				{
					neighboursAlive++;
				}
			}

			if (kvp.second == '#')
			{
				// A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
				nextBoard(kvp.first) = (neighboursAlive == 2 || neighboursAlive == 3) ? '#' : '.';
			}
			else
			{
				// A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.
				nextBoard(kvp.first) = (neighboursAlive == 3) ? '#' : '.';
			}
		}

		SetCorners(&nextBoard);
	}

	int64_t answer = 0;
	for (const auto kvp : boards[iterations % 2].Grid())
	{
		if (kvp.second == '#')
		{
			answer++;
		}
	}

	printf("[2015] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2015(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
