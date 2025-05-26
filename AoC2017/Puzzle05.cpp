#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2017_Types
{
}

using namespace Puzzle05_2017_Types;

static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> instructions;
	ranges::copy(views::elements<0>(ScanfEachLine<int64_t>(input, "%lld")), back_inserter(instructions));

	size_t pc = 0;

	int64_t answer = 0;
	while (true)
	{
		pc = pc + instructions[pc]++;
		answer++;

		if (pc >= instructions.size())
			break;
	}

	printf("[2017] Puzzle05_A: %" PRId64 "\n", answer);
}


static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> instructions;
	ranges::copy(views::elements<0>(ScanfEachLine<int64_t>(input, "%lld")), back_inserter(instructions));

	size_t pc = 0;

	int64_t answer = 0;
	while (true)
	{
		int64_t jump = instructions[pc];
		int64_t nextPc = pc + jump;
		instructions[pc] += (jump >= 3 ? -1 : 1);
		pc = nextPc;
		answer++;

		if (pc >= instructions.size())
			break;
	}

	printf("[2017] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2017(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
