#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2022_Types
{
}

using namespace Puzzle09_2022_Types;

static int64_t SimulateRope(istream &input, size_t elementCount)
{
	vector<Point2> rope{ elementCount };

	const map<char, Point2> directions =
	{
		{ 'U', Point2::Up() },
		{ 'D', Point2::Down() },
		{ 'L', Point2::Left() },
		{ 'R', Point2::Right() },
	};

	set<Point2> uniquePositions{ {} };
	for (const auto &line : ScanfEachLine<char, int64_t>(input, "%c %lld"))
	{
		for (int64_t count = 0; count < get<1>(line); count++)
		{
			rope[0] += directions.at(get<0>(line));
			for (size_t i = 1; i < rope.size(); i++)
			{
				Point2 delta = rope[i - 1] - rope[i];

				// Pure axis movement
				bool moveU = (delta.Y == -2);
				bool moveD = (delta.Y == 2);
				bool moveL = (delta.X == -2);
				bool moveR = (delta.X == 2);

				// Extra movement for diagonals
				bool moveU_LR = (moveL || moveR) && (delta.Y == -1);
				bool moveD_LR = (moveL || moveR) && (delta.Y == 1);
				bool moveL_UD = (moveU || moveD) && (delta.X == -1);
				bool moveR_UD = (moveU || moveD) && (delta.X == 1);

				if (moveR || moveR_UD)
				{
					rope[i] += Point2::Right();
				}
				if (moveL || moveL_UD)
				{
					rope[i] += Point2::Left();
				}
				if (moveU || moveU_LR)
				{
					rope[i] += Point2::Up();
				}
				if (moveD || moveD_LR)
				{
					rope[i] += Point2::Down();
				}
			}

			uniquePositions.insert(rope.back());
		}
	}

	return (int64_t) uniquePositions.size();
}

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = SimulateRope(input, 2);

	printf("[2022] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = SimulateRope(input, 10);;

	printf("[2022] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2022(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
