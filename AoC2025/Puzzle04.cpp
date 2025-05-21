#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2025_Types
{
}

using namespace Puzzle04_2025_Types;

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2025(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
