#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2021_Types
{
	struct Puzzle
	{
		string IEAString;
		set<Point2> LightPixels;
		set<Point2> DarkPixels;
	};
}

using namespace Puzzle20_2021_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);

	p.IEAString = lines[0];
	for (int64_t y = 0; y + 2 < (int64_t)lines.size(); y++)
	{
		const string& line = lines[y + 2];
		for (int64_t x = 0; x < (int64_t)line.size(); x++)
		{
			if (line[x] == '#')
			{
				p.LightPixels.insert(Point2{ x, y });
			}
		}
	}

	return p;
}

static int PatternToIndex(const bool pattern[9])
{
	int index = 0;
	for (int i = 0; i < 9; i++)
	{
		index <<= 1;
		index |= pattern[i] ? 1 : 0;
	}
	return index;
}

static bool IsIn(const Point2& p, const set<Point2>& pixels)
{
	return pixels.find(p) != pixels.end();
}

static void EnhanceLightToDark(Puzzle* puzzle)
{
	assert(puzzle->DarkPixels.empty());

	Point2 offsets[] = {
		Point2{ -1, -1 }, Point2{  0, -1 }, Point2{  1, -1 },
		Point2{ -1,  0 }, Point2{  0,  0 }, Point2{  1,  0 },
		Point2{ -1,  1 }, Point2{  0,  1 }, Point2{  1,  1 },
	};

	set<Point2> pixelsToCheck;
	for (const Point2& p : puzzle->LightPixels)
	{
		for (int i = 0; i < 9; i++)
		{
			pixelsToCheck.insert(p + offsets[i]);
		}
	}

	set<Point2> newDarkPixels;
	for (const Point2& pixel : pixelsToCheck)
	{
		bool pattern[9];
		for (int i = 0; i < 9; i++)
		{
			Point2 testPixel = pixel + offsets[i];
			pattern[i] = IsIn(testPixel, puzzle->LightPixels) == true;
		}

		int index = PatternToIndex(pattern);
		if (puzzle->IEAString[index] == '.')
		{
			newDarkPixels.insert(pixel);
		}
	}

	puzzle->LightPixels.clear();
	puzzle->DarkPixels = newDarkPixels;
}

static void EnhanceDarkToLight(Puzzle* puzzle)
{
	assert(puzzle->LightPixels.empty());

	Point2 offsets[] = {
		Point2{ -1, -1 }, Point2{  0, -1 }, Point2{  1, -1 },
		Point2{ -1,  0 }, Point2{  0,  0 }, Point2{  1,  0 },
		Point2{ -1,  1 }, Point2{  0,  1 }, Point2{  1,  1 },
	};

	set<Point2> pixelsToCheck;
	for (const Point2& p : puzzle->DarkPixels)
	{
		for (int i = 0; i < 9; i++)
		{
			pixelsToCheck.insert(p + offsets[i]);
		}
	}

	set<Point2> newLightPixels;
	for (const Point2& pixel : pixelsToCheck)
	{
		bool pattern[9];
		for (int i = 0; i < 9; i++)
		{
			Point2 testPixel = pixel + offsets[i];
			pattern[i] = IsIn(testPixel, puzzle->DarkPixels) == false;
		}

		int index = PatternToIndex(pattern);
		if (puzzle->IEAString[index] == '#')
		{
			newLightPixels.insert(pixel);
		}
	}

	puzzle->LightPixels = newLightPixels;
	puzzle->DarkPixels.clear();
}

static void Puzzle20_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	// The *actual* puzzle alternates infinite fields of light/dark
	assert(p.IEAString[0] == '#');
	assert(p.IEAString[511] == '.');

	for (int i = 0; i < 2; i += 2)
	{
		EnhanceLightToDark(&p);
		EnhanceDarkToLight(&p);
	}

	int64_t answer = (int64_t)p.LightPixels.size();

	printf("[2021] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	// The *actual* puzzle alternates infinite fields of light/dark
	assert(p.IEAString[0] == '#');
	assert(p.IEAString[511] == '.');

	for (int i = 0; i < 50; i += 2)
	{
		EnhanceLightToDark(&p);
		EnhanceDarkToLight(&p);
	}

	int64_t answer = (int64_t)p.LightPixels.size();

	printf("[2021] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2021(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
