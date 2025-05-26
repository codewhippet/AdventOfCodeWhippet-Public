#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2015_Types
{
}

using namespace Puzzle17_2015_Types;

static int64_t StorageCombinations(const vector<int64_t>& containers,
	size_t containerIndex,
	int64_t targetLitres,
	int64_t containersUsed,
	map<int64_t, int64_t> *solutionCount)
{
	if (targetLitres == 0)
	{
		(*solutionCount)[containersUsed]++;
		return 1;
	}

	if (containerIndex == containers.size() - 1)
	{
		if (containers[containerIndex] == targetLitres)
		{
			(*solutionCount)[containersUsed + 1]++;
			return 1;
		}
		return 0;
	}

	int64_t combinations = 0;
	if (containers[containerIndex] <= targetLitres)
	{
		combinations += StorageCombinations(containers, containerIndex + 1, targetLitres - containers[containerIndex], containersUsed + 1, solutionCount);
	}
	combinations += StorageCombinations(containers, containerIndex + 1, targetLitres, containersUsed, solutionCount);

	return combinations;
}

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t targetLitres = 150;

	vector<int64_t> containers;
	for (const string& line : ReadEachLine(input))
	{
		containers.push_back(atoi(line.c_str()));
	}

	map<int64_t, int64_t> solutionCount;
	int64_t answer = StorageCombinations(containers, 0, targetLitres, 0, &solutionCount);

	printf("[2015] Puzzle17_A: %" PRId64 "\n", answer);
}


static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t targetLitres = 150;

	vector<int64_t> containers;
	for (const string& line : ReadEachLine(input))
	{
		containers.push_back(atoi(line.c_str()));
	}

	map<int64_t, int64_t> solutionCount;
	StorageCombinations(containers, 0, targetLitres, 0, &solutionCount);

	int64_t answer = solutionCount.begin()->second;

	printf("[2015] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2015(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
