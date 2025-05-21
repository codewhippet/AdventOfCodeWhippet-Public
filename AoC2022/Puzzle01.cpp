#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2022_Types
{
}

using namespace Puzzle01_2022_Types;

static void Puzzle01_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	int64_t runningTotal = 0;
	for (const string& line : ReadEachLine(input))
	{
		if (line.empty())
		{
			answer = max(runningTotal, answer);
			runningTotal = 0;
		}
		else
		{
			runningTotal += atoll(line.c_str());
		}
	}

	answer = max(runningTotal, answer);

	printf("[2022] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> elfTotals;

	int64_t runningTotal = 0;
	for (const string& line : ReadEachLine(input))
	{
		if (line.empty())
		{
			elfTotals.push_back(runningTotal);
			runningTotal = 0;
		}
		else
		{
			runningTotal += atoll(line.c_str());
		}
	}

	elfTotals.push_back(runningTotal);
	ranges::sort(elfTotals);
	ranges::reverse(elfTotals);

	auto topThree = elfTotals | views::take(3);
	int64_t answer = accumulate(topThree.begin(), topThree.end(), 0ll);

	printf("[2022] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2022(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
