#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2025_Types
{
}

using namespace Puzzle21_2025_Types;

static void Puzzle21_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2025(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
