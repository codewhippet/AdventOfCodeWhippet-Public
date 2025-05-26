#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2022_Types
{
}

using namespace Puzzle06_2022_Types;

static int64_t FindMarker(const string &message, int64_t requiredCount)
{
	return Enumerable::Range(0, (int)message.size())
		->Select<pair<int64_t, string>>([&](const int &i) -> pair<int64_t, string>
			{
				return { i, message.substr(i, requiredCount) };
			})
		->Where([&](const pair<int64_t, string> &p)
			{
				return MakeEnumerator(p.second)->Distinct()->Count() == requiredCount;
			})
		->First().first + requiredCount;
}

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = FindMarker(ReadSingleLine(input), 4);

	printf("[2022] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = FindMarker(ReadSingleLine(input), 14);

	printf("[2022] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2022(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
