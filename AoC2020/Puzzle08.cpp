#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2020_Types
{
	const int kNOP = 0;
	const int kACC = 1;
	const int kJMP = 2;
}

using namespace Puzzle08_2020_Types;

static vector<pair<int, int>> ReadProgram(istream& input)
{
	return MakeEnumerator(ReadAllLines(input))
		->Select<pair<int, int>>([](const string& line)
			{
				char operation[4];
				pair<int, int> opcode;
				sscanf(line.c_str(), "%s %d", operation, &opcode.second);
				if (strcmp(operation, "nop") == 0)
				{
					opcode.first = kNOP;
				}
				else if (strcmp(operation, "acc") == 0)
				{
					opcode.first = kACC;
				}
				else
				{
					assert(strcmp(operation, "jmp") == 0);
					opcode.first = kJMP;
				}
				return opcode;
			})
		->ToVector();
}

static bool RunsToCompletion(const vector<pair<int, int>>& program, int64_t* accOut)
{
	set<size_t> executed;

	int64_t acc = 0;
	size_t pc = 0;

	while (true)
	{
		if (executed.contains(pc))
		{
			return false;
		}

		if (pc == program.size())
		{
			*accOut = acc;
			return true;
		}

		assert(pc < program.size());

		executed.insert(pc);

		switch (program[pc].first)
		{
		case kNOP:
			pc++;
			break;

		case kACC:
			acc += program[pc].second;
			pc++;
			break;

		case kJMP:
			pc += program[pc].second;
			break;
		}
	};
}

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int, int>> program = ReadProgram(input);

	set<size_t> executed;

	int64_t acc = 0;
	size_t pc = 0;

	int64_t answer = 0;
	while (true)
	{
		if (executed.contains(pc))
		{
			answer = acc;
			break;
		}

		executed.insert(pc);

		switch (program[pc].first)
		{
		case kNOP:
			pc++;
			break;

		case kACC:
			acc += program[pc].second;
			pc++;
			break;

		case kJMP:
			pc += program[pc].second;
			break;
		}
	};

	printf("[2020] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int, int>> program = ReadProgram(input);

	int64_t answer = 0;

	for (size_t i = 0; i < program.size(); i++)
	{
		if (program[i].first == kACC)
			continue;

		int stash;
		if (program[i].first == kNOP)
		{
			program[i].first = kJMP;
			stash = kNOP;
		}
		else
		{
			assert(program[i].first == kJMP);
			program[i].first = kNOP;
			stash = kJMP;
		}

		int64_t acc;
		if (RunsToCompletion(program, &acc))
		{
			answer = acc;
			break;
		}

		program[i].first = stash;
	}

	printf("[2020] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2020(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
