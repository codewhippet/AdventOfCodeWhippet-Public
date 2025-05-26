#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2025_Types
{
}

using namespace Puzzle20_2025_Types;

static void Puzzle20_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2025(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
