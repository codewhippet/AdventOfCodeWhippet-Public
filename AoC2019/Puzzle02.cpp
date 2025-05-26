#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2019_Types
{
}

using namespace Puzzle02_2019_Types;

static void Execute(vector<int64_t>* program)
{
	vector<int64_t>& p = *program;

	int64_t pc = 0;
	while (true)
	{
		int64_t instruction = p[pc];
		switch (instruction)
		{
		case 99:
			return;

		case 1:
			{
				int64_t a = p[p[pc + 1]];
				int64_t b = p[p[pc + 2]];
				p[p[pc + 3]] = a + b;
				pc += 4;
			}
			break;

		case 2:
			{
				int64_t a = p[p[pc + 1]];
				int64_t b = p[p[pc + 2]];
				p[p[pc + 3]] = a * b;
				pc += 4;
			}
			break;
		}
	}
}

static int64_t GravityAssist(vector<int64_t> program, int64_t a, int64_t b)
{
	program[1] = a;
	program[2] = b;
	Execute(&program);
	return program[0];
}

static int64_t FindGravityAssist(const vector<int64_t> program)
{
	for (int64_t noun : views::iota(0, 100))
	{
		for (int64_t verb : views::iota(0, 100))
		{
			if (GravityAssist(program, noun, verb) == 19690720)
			{
				return 100 * noun + verb;
			}
		}
	}
	return -1;
}

static void Puzzle02_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto program = ReadAsVectorOfNumbers(ReadSingleLine(input));
	program[1] = 12;
	program[2] = 2;

	Execute(&program);

	int64_t answer = program[0];

	printf("[2019] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto program = ReadAsVectorOfNumbers(ReadSingleLine(input));
	int64_t answer = FindGravityAssist(program);

	printf("[2019] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2019(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
