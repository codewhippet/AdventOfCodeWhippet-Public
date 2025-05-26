#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2025_Types
{
}

using namespace Puzzle14_2025_Types;

static void Puzzle14_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle14_A: %" PRId64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2025(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
