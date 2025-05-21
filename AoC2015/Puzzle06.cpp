#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2015_Types
{
}

using namespace Puzzle06_2015_Types;

// Grid update logic derived from a Karnaugh Map:
//
//      toggle  0  0  1  1
//     turnOff  0  1  0  1
// in | turnOn
//  0     0     x  0  1  x
//  0     1     1  x  x  x
//  1     0     x  0  0  x
//  1     1     1  x  x  x
//
// -->
// 
//      toggle  0  0  1  1
//     turnOff  0  1  0  1
// in | turnOn
//  0     0     A  0  B  B
//  0     1     A  0  B  B
//  1     0     A  0  0  0
//  1     1     A  0  0  0
//
// A = !toggle & !turnOff
// B = toggle & !in

static void UpdateGridOnOff(
	uint8_t turnOn,
	uint8_t toggle,
	uint8_t turnOff,
	int64_t gridWidth,
	int64_t gridHeight,
	const Point2& from,
	const Point2& to,
	vector<uint8_t>* grid)
{
	assert(((int)turnOn + (int)toggle + (int)turnOff) == 0xff);
	(void)gridHeight;
	(void)turnOn;

	for (int64_t y = from.Y; y <= to.Y; y++)
	{
		int64_t rowOffset = y * gridWidth;
		for (int64_t x = from.X; x <= to.X; x++)
		{
			uint8_t in = (*grid)[rowOffset + x];
			(*grid)[rowOffset + x] = (~toggle & ~turnOff) | (toggle & ~in);
		}
	}
}

static void UpdateGridBrightness(
	int32_t change,
	int64_t gridWidth,
	int64_t gridHeight,
	const Point2& from,
	const Point2& to,
	vector<int32_t>* grid)
{
	(void)gridHeight;

	for (int64_t y = from.Y; y <= to.Y; y++)
	{
		int64_t rowOffset = y * gridWidth;
		for (int64_t x = from.X; x <= to.X; x++)
		{
			int32_t in = (*grid)[rowOffset + x];
			(*grid)[rowOffset + x] = max(in + change, 0);
		}
	}
}

static void Puzzle06_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t gridWidth = 1000;
	const int64_t gridHeight = 1000;

	vector<uint8_t> grid;
	grid.resize(gridWidth * gridHeight);
	for (const string& line : ReadEachLine(input))
	{
		Point2 from, to;
		int scanned = sscanf(line.c_str(), "%*[^0-9]%" SCNd64 ",%" SCNd64 " through %" SCNd64 ",%" SCNd64,
			&from.X, &from.Y,
			&to.X, &to.Y);
		assert(scanned == 4);
		(void)scanned;

		uint8_t turnOn = (line.starts_with("turn on") ? 0xff : 0x00);
		uint8_t toggle = (line.starts_with("toggle") ? 0xff : 0x00);
		uint8_t turnOff = (line.starts_with("turn off") ? 0xff : 0x00);

		UpdateGridOnOff(turnOn, toggle, turnOff, gridWidth, gridHeight, from, to, &grid);
	}

	int64_t answer = ranges::count(grid, 0xff);

	printf("[2015] Puzzle06_A: %" PRId64 "\n", answer);
}


static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t gridWidth = 1000;
	const int64_t gridHeight = 1000;

	vector<int32_t> grid;
	grid.resize(gridWidth * gridHeight);
	for (const string& line : ReadEachLine(input))
	{
		Point2 from, to;
		int scanned = sscanf(line.c_str(), "%*[^0-9]%" SCNd64 ",%" SCNd64 " through %" SCNd64 ",%" SCNd64,
			&from.X, &from.Y,
			&to.X, &to.Y);
		assert(scanned == 4);
		(void)scanned;

		int32_t brightnessChange = 0;
		if (line.starts_with("turn on"))
		{
			brightnessChange = 1;
		}
		else if (line.starts_with("toggle"))
		{
			brightnessChange = 2;
		}
		else
		{
			assert(line.starts_with("turn off"));
			brightnessChange = -1;
		}

		UpdateGridBrightness(brightnessChange, gridWidth, gridHeight, from, to, &grid);
	}

	int64_t answer = accumulate(grid.begin(), grid.end(), 0, plus{});

	printf("[2015] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2015(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
