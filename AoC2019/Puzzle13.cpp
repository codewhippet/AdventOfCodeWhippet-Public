#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2019_Types
{
}

using namespace Puzzle13_2019_Types;

static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	auto exec = puter.Execute();
	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;

	map<Point2, int8_t> screen;
	for (size_t i = 0; (i + 2) < out.size(); i += 3)
	{
		screen[Point2{ out[i + 0], out[i + 1] }] = (int8_t)out[i + 2];
	}

	int64_t answer = ranges::count(screen | views::values, 2);

	printf("[2019] Puzzle13_A: %" PRId64 "\n", answer);
}


static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> program = ReadAsVectorOfNumbers(ReadSingleLine(input));
	program[0] = 2;

	Intputer puter;
	puter.CopyProgram(program);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	int64_t answer = 0;

	map<Point2, int8_t> blocks;
	while (true)
	{
		puter.Execute();
		assert(in.empty());
		assert(!out.empty());

		Point2 ballLocation;
		Point2 paddleLocation;
		for (size_t i = 0; (i + 2) < out.size(); i += 3)
		{
			Point2 screenLocation{ out[i + 0], out[i + 1] };
			if (screenLocation == Point2{ -1, 0 })
			{
				answer = out[i + 2];
			}
			else
			{
				switch (out[i + 2])
				{
				case 0:
					blocks.erase(screenLocation);
					break;

				case 2:
					blocks[screenLocation] = (int8_t)out[i + 2];
					break;

				case 3:
					assert(paddleLocation == Point2{});
					paddleLocation = screenLocation;
					break;

				case 4:
					ballLocation = screenLocation;
					break;
				}
			}
		}
		out.clear();

		if (blocks.empty())
		{
			break;
		}

		if (ballLocation.X < paddleLocation.X)
		{
			in.push_back(-1);
		}
		else if (ballLocation.X > paddleLocation.X)
		{
			in.push_back(1);
		}
		else
		{
			in.push_back(0);
		}
	}

	printf("[2019] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2019(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
