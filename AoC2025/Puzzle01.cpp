#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2025_Types
{
}

using namespace Puzzle01_2025_Types;

static void Puzzle01_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2025(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
