#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2021_Types
{
}

using namespace Puzzle01_2021_Types;

static void Puzzle01_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int numIncreases = 0;

	int currentDepth;
	input >> currentDepth;

	while (input.eof() == false)
	{
		int nextDepth;
		input >> nextDepth;
		if (nextDepth > currentDepth)
		{
			numIncreases++;
		}
		currentDepth = nextDepth;
	}

	int64_t answer = numIncreases;

	printf("[2021] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> depths;
	ranges::copy(ScanfEachLine<int>(input, "%d") | views::elements<0>, back_inserter(depths));

	int numIncreases = 0;

	for (size_t i = 0; i + 3 < depths.size(); i++)
	{
		int currentDepth = depths[i + 0] + depths[i + 1] + depths[i + 2];
		int nextDepth = depths[i + 1] + depths[i + 2] + depths[i + 3];
		if (nextDepth > currentDepth)
		{
			numIncreases++;
		}
	}

	int64_t answer = numIncreases;

	printf("[2021] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2021(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
