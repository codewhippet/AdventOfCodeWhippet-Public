#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2015_Types
{
}

using namespace Puzzle01_2015_Types;

static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string instructions;
	getline(input, instructions);

	map<char, int64_t> movements{ { '(', 1 }, { ')', -1 } };

	int64_t answer = 0;
	for (char c : instructions)
	{
		answer += movements.at(c);
	}

	printf("[2015] Puzzle01_A: %" PRId64 "\n", answer);
}


static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string instructions;
	getline(input, instructions);

	map<char, int64_t> movements{ { '(', 1 }, { ')', -1 } };

	int64_t answer = 0;
	int64_t currentFloor = 0;
	for (size_t i = 0; i < instructions.size(); i++)
	{
		currentFloor += movements.at(instructions[i]);
		if (currentFloor < 0)
		{
			answer = i + 1;
			break;
		}
	}

	printf("[2015] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2015(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
