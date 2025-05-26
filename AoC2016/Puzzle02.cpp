#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2016_Types
{
}

using namespace Puzzle02_2016_Types;

static string GetKeyCode(const map<Point2, char>& keypad, const Point2& startingLocation, istream& input)
{
	const map<char, Point2> directions =
	{
		{ 'U', Point2::Up() },
		{ 'D', Point2::Down() },
		{ 'L', Point2::Left() },
		{ 'R', Point2::Right() },
	};

	Point2 fingerLocation = startingLocation;

	string code;
	for (const string& line : ReadEachLine(input))
	{
		for (char dir : line)
		{
			Point2 newFingerLocation = fingerLocation + directions.at(dir);
			if (keypad.contains(newFingerLocation))
			{
				fingerLocation = newFingerLocation;
			}
		}
		code += keypad.at(fingerLocation);
	}
	return code;
}

static void Puzzle02_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const map<Point2, char> keypad =
	{
		{ Point2::Up() + Point2::Left(), '1' },
		{ Point2::Up(), '2' },
		{ Point2::Up() + Point2::Right(), '3' },

		{ Point2::Left(), '4' },
		{ Point2(), '5' },
		{ Point2::Right(), '6' },

		{ Point2::Down() + Point2::Left(), '7' },
		{ Point2::Down(), '8' },
		{ Point2::Down() + Point2::Right(), '9' },
	};

	string answer = GetKeyCode(keypad, {}, input);

	printf("[2016] Puzzle02_A: %s\n", answer.c_str());
}


static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const map<Point2, char> keypad =
	{
		{ Point2::Up() + Point2::Up(), '1' },

		{ Point2::Up() + Point2::Left(), '2' },
		{ Point2::Up(), '3' },
		{ Point2::Up() + Point2::Right(), '4' },

		{ Point2::Left() + Point2::Left(), '5' },
		{ Point2::Left(), '6' },
		{ Point2(), '7' },
		{ Point2::Right(), '8' },
		{ Point2::Right() + Point2::Right(), '9' },

		{ Point2::Down() + Point2::Left(), 'A' },
		{ Point2::Down(), 'B' },
		{ Point2::Down() + Point2::Right(), 'C' },

		{ Point2::Down() + Point2::Down(), 'D' },
	};

	string answer = GetKeyCode(keypad, Point2::Left() + Point2::Left(), input);

	printf("[2016] Puzzle02_B: %s\n", answer.c_str());
}

void Puzzle02_2016(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
