#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2023_Types
{
	struct Mirror
	{
		Point2 Size;
		map<Point2, char> Rocks;
	};
}

using namespace Puzzle14_2023_Types;

static Mirror ReadMirror(istream& input)
{
	Mirror m;

	vector<string> lines = ReadAllLines(input);
	m.Size = Point2(lines[0].size(), lines.size());

	for (int64_t y = 0; y < (int64_t)lines.size(); y++)
	{
		for (int64_t x = 0; x < (int64_t)lines[0].size(); x++)
		{
			if (lines[y][x] != '.')
			{
				m.Rocks[Point2{ x, y }] = lines[y][x];
			}
		}
	}

	return m;
}

static bool NorthSort(const Point2& a, const Point2& b)
{
	if (a.Y != b.Y)
		return a.Y < b.Y;
	return a.X < b.X;
}

static bool WestSort(const Point2& a, const Point2& b)
{
	if (a.X != b.X)
		return a.X < b.X;
	return a.Y < b.Y;
}

static bool SouthSort(const Point2& a, const Point2& b)
{
	if (a.Y != b.Y)
		return a.Y > b.Y;
	return a.X > b.X;
}

static bool EastSort(const Point2& a, const Point2& b)
{
	if (a.X != b.X)
		return a.X > b.X;
	return a.Y > b.Y;
}

static int64_t NorthLoad(const Point2& rock, const Point2& field)
{
	return field.Y - rock.Y;
}

bool IsConstrained(const Point2& rock, const Point2& constraint)
{
	return (rock.X < 0) || (rock.Y < 0) || (rock.X >= constraint.X) || (rock.Y >= constraint.Y);
}

static Mirror Tilt(Mirror mirror, bool (* const Sort)(const Point2&, const Point2&), const Point2& direction, const Point2& constraint)
{
	Mirror newMirror;
	newMirror.Size = mirror.Size;

	vector<Point2> mobileRocks;
	for (map<Point2, char>::const_reference rock : mirror.Rocks)
	{
		if (rock.second == '#')
		{
			newMirror.Rocks.insert(rock);
		}
		else
		{
			mobileRocks.push_back(rock.first);
		}
	}

	sort(mobileRocks.begin(), mobileRocks.end(), Sort);

	for (Point2 p : mobileRocks)
	{
		while (true)
		{
			Point2 newP = p + direction;
			if (IsConstrained(newP, constraint) || newMirror.Rocks.contains(newP))
			{
				newMirror.Rocks[p] = 'O';
				break;
			}
			p = newP;
		}
	}

	assert(newMirror.Rocks.size() == mirror.Rocks.size());

	return newMirror;
}

static int64_t CalculateLoad(Mirror mirror, int64_t(* const LoadCalc)(const Point2&, const Point2&), const Point2& field)
{
	int64_t load = 0;
	for (map<Point2, char>::const_reference rock : mirror.Rocks)
	{
		if (rock.second == 'O')
		{
			int64_t rockLoad = LoadCalc(rock.first, field);
			load += rockLoad;
		}
	}
	return load;
}

static void Puzzle14_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Mirror mirror = ReadMirror(input);

	Mirror tiltedMirror = Tilt(mirror, &NorthSort, Point2{ 0, -1 }, mirror.Size);

	int64_t answer = CalculateLoad(tiltedMirror, &NorthLoad, tiltedMirror.Size);

	printf("[2023] Puzzle14_A: %" PRId64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Mirror startingMirror = ReadMirror(input);

	vector<pair<bool (* const)(const Point2&, const Point2&), Point2>> cycle =
	{
		{ &NorthSort, Point2{ 0, -1 } },
		{ &WestSort, Point2{ -1, 0 } },
		{ &SouthSort, Point2{ 0, 1 } },
		{ &EastSort, Point2{ 1, 0 } },
	};

	map<map<Point2, char>, int64_t> allStates;

	int64_t loopStart = 0;
	int64_t loopEnd = 0;

	Mirror mirror = startingMirror;
	for (int64_t i = 0; i < 500; i++)
	{
		map<map<Point2, char>, int64_t>::const_iterator prev = allStates.find(mirror.Rocks);
		if (prev != allStates.end())
		{
			loopStart = prev->second;
			loopEnd = i;
			break;
		}

		allStates[mirror.Rocks] = i;

		for (auto& tilt : cycle)
		{
			mirror = Tilt(mirror, tilt.first, tilt.second, mirror.Size);
		}
	}

	int64_t loopSize = loopEnd - loopStart;
	int64_t loopOffset = (1000000000 - loopStart) % loopSize;

	mirror = startingMirror;
	for (int64_t i = 0; i < loopStart + loopOffset; i++)
	{
		for (auto& tilt : cycle)
		{
			mirror = Tilt(mirror, tilt.first, tilt.second, mirror.Size);
		}
	}

	int64_t answer = CalculateLoad(mirror, &NorthLoad, mirror.Size);

	printf("[2023] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2023(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
