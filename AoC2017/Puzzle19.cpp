#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2017_Types
{
}

using namespace Puzzle19_2017_Types;

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D grid = ReadArrayMap(input, ' ');

	// Find the starting location
	Point2 robotPos;
	Point2 robotDir = Point2::South();
	robotPos.Y = grid.GetOrigin().Y;
	for (int64_t x : grid.AxisRangeX())
	{
		if (grid(x, robotPos.Y) == '|')
		{
			robotPos.X = x;
			break;
		}
	}

	// Follow the line
	string answer;
	while (true)
	{
		robotPos = robotPos + robotDir;
		char currentSquare = grid(robotPos);

		if (isalpha(currentSquare))
		{
			answer += currentSquare;
		}
		else if (currentSquare == '+')
		{
			char turnRightSquare = grid(robotPos + Point2::RotateClockwise(robotDir));
			char turnLeftSquare = grid(robotPos + Point2::RotateAnticlockwise(robotDir));
			assert(((turnRightSquare == ' ') && (turnLeftSquare != ' '))
				|| ((turnRightSquare != ' ') && (turnLeftSquare == ' ')));
			(void)turnLeftSquare;
			if (turnRightSquare != ' ')
			{
				robotDir = Point2::RotateClockwise(robotDir);
			}
			else
			{
				robotDir = Point2::RotateAnticlockwise(robotDir);
			}
		}
		else if (currentSquare == ' ')
		{
			break;
		}
	}

	printf("[2017] Puzzle19_A: %s\n", answer.c_str());
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D grid = ReadArrayMap(input, ' ');

	// Find the starting location
	Point2 robotPos;
	Point2 robotDir = Point2::South();
	robotPos.Y = grid.GetOrigin().Y;
	for (int64_t x : grid.AxisRangeX())
	{
		if (grid(x, robotPos.Y) == '|')
		{
			robotPos.X = x;
			break;
		}
	}

	// Follow the line
	int64_t answer = 1;
	while (true)
	{
		robotPos = robotPos + robotDir;
		char currentSquare = grid(robotPos);
		answer++;

		if (currentSquare == '+')
		{
			char turnRightSquare = grid(robotPos + Point2::RotateClockwise(robotDir));
			char turnLeftSquare = grid(robotPos + Point2::RotateAnticlockwise(robotDir));
			assert(((turnRightSquare == ' ') && (turnLeftSquare != ' '))
				|| ((turnRightSquare != ' ') && (turnLeftSquare == ' ')));
			(void)turnLeftSquare;
			if (turnRightSquare != ' ')
			{
				robotDir = Point2::RotateClockwise(robotDir);
			}
			else
			{
				robotDir = Point2::RotateAnticlockwise(robotDir);
			}
		}
		else if (currentSquare == ' ')
		{
			answer--;
			break;
		}
	}

	printf("[2017] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2017(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
