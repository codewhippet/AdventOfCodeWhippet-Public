#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2025_Types
{
}

using namespace Puzzle19_2025_Types;

static void Puzzle19_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle19_A: %" PRId64 "\n", answer);
}

static void Puzzle19_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2025(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
