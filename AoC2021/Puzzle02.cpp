#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2021_Types
{
}

using namespace Puzzle02_2021_Types;

static void Puzzle02_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<string, int>> instructions;
	ranges::copy(ReadEachLine(input, regex{ R"((\w+) (\d+))" })
		| views::transform([](const auto& m) -> pair<string, int>
			{
				return { m[1].str(), atoi(m[2].str().c_str()) };
			}),
		back_inserter(instructions));

	int horizontal = 0;
	int depth = 0;

	for (vector<pair<string, int>>::const_reference instruction : instructions)
	{
		if (instruction.first == "forward")
			horizontal += instruction.second;
		if (instruction.first == "up")
			depth -= instruction.second;
		if (instruction.first == "down")
			depth += instruction.second;
	}

	int64_t answer = horizontal * depth;

	printf("[2021] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<string, int>> instructions;
	ranges::copy(ReadEachLine(input, regex{ R"((\w+) (\d+))" })
		| views::transform([](const auto& m) -> pair<string, int>
			{
				return { m[1].str(), atoi(m[2].str().c_str()) };
			}),
		back_inserter(instructions));

	int horizontal = 0;
	int depth = 0;
	int aim = 0;

	for (vector<pair<string, int>>::const_reference instruction : instructions)
	{
		if (instruction.first == "forward")
		{
			horizontal += instruction.second;
			depth += aim * instruction.second;
		}
		if (instruction.first == "up")
		{
			aim -= instruction.second;
		}
		if (instruction.first == "down")
		{
			aim += instruction.second;
		}
	}

	int64_t answer = horizontal * depth;

	printf("[2021] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2021(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
