#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2016_Types
{
}

using namespace Puzzle01_2016_Types;

static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Point2 location{ 0, 0 };
	Point2 direction = Point2::North();

	string instruction;
	while (input >> instruction)
	{
		if (instruction[0] == 'L')
		{
			direction = Point2::RotateAnticlockwise(direction);
		}
		else if (instruction[0] == 'R')
		{
			direction = Point2::RotateClockwise(direction);
		}
		else
		{
			assert(false);
		}

		location = location + direction * atoi(&instruction[1]);
	}

	int64_t answer = abs(location.X) + abs(location.Y);

	printf("[2016] Puzzle01_A: %" PRId64 "\n", answer);
}


static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Point2 location{ 0, 0 };
	Point2 direction = Point2::North();

	set<Point2> visited{ location };

	string instruction;
	while (input >> instruction)
	{
		if (instruction[0] == 'L')
		{
			direction = Point2::RotateAnticlockwise(direction);
		}
		else if (instruction[0] == 'R')
		{
			direction = Point2::RotateClockwise(direction);
		}
		else
		{
			assert(false);
		}

		bool foundHq = false;

		int64_t blocks = atoi(&instruction[1]);
		for (int64_t i = 0; i < blocks; i++)
		{
			location = location + direction;
			if (visited.contains(location))
			{
				foundHq = true;
				break;
			}
			visited.insert(location);
		}

		if (foundHq)
		{
			break;
		}
	}

	int64_t answer = abs(location.X) + abs(location.Y);

	printf("[2016] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2016(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
