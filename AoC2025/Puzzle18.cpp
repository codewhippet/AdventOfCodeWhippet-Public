#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2025_Types
{
}

using namespace Puzzle18_2025_Types;

static void Puzzle18_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2025(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
