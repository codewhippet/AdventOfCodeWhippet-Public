#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2025_Types
{
}

using namespace Puzzle09_2025_Types;

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2025(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
