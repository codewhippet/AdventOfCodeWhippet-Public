#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2025_Types
{
}

using namespace Puzzle06_2025_Types;

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2025(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
