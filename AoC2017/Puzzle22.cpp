#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2017_Types
{
}

using namespace Puzzle22_2017_Types;

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap network = ReadPointMap(input);

	Point2 virusPos{ (network.Size.X / 2), (network.Size.Y / 2) };
	Point2 virusDir = Point2::Up();

	int64_t answer = 0;
	for (int64_t i = 0; i < 10000; i++)
	{
		if (network.Data.contains(virusPos))
		{
			virusDir = Point2::RotateClockwise(virusDir);
			network.Data.erase(virusPos);
		}
		else
		{
			virusDir = Point2::RotateAnticlockwise(virusDir);
			network.Data.insert({ virusPos, '#' });
			answer++;
		}
		virusPos = virusPos + virusDir;
	}

	printf("[2017] Puzzle22_A: %" PRId64 "\n", answer);
}


static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap network = ReadPointMap(input);

	Point2 virusPos{ (network.Size.X / 2), (network.Size.Y / 2) };
	Point2 virusDir = Point2::Up();

	int64_t answer = 0;
	for (int64_t i = 0; i < 10000000; i++)
	{
		if (network.Data.contains(virusPos))
		{
			switch (network.Data.at(virusPos))
			{
			case 'W':
				network.Data[virusPos] = '#';
				answer++;
				break;
			case '#':
				virusDir = Point2::RotateClockwise(virusDir);
				network.Data[virusPos] = 'F';
				break;
			case 'F':
				virusDir = Point2::RotateClockwise(Point2::RotateClockwise(virusDir));
				network.Data.erase(virusPos);
				break;
			default:
				assert(false);
			}
		}
		else
		{
			virusDir = Point2::RotateAnticlockwise(virusDir);
			network.Data.insert({ virusPos, 'W' });
		}
		virusPos = virusPos + virusDir;
	}

	printf("[2017] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2017(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
