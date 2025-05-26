#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2025_Types
{
}

using namespace Puzzle13_2025_Types;

static void Puzzle13_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2025(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
