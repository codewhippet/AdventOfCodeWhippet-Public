#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2025_Types
{
}

using namespace Puzzle17_2025_Types;

static void Puzzle17_A(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle17_A: %" PRId64 "\n", answer);
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	int64_t answer = 0;

	printf("[2025] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2025(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
