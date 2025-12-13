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
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	const int64_t dialSize = 100;
	int64_t dialPosition = 50;
	for (const auto& line : ScanfEachLine<char, int64_t>(input, "%c%lld"))
	{
		const int64_t rotateBy = get<1>(line) % dialSize;
		dialPosition = (dialPosition + (get<0>(line) == 'R' ? rotateBy : dialSize - rotateBy)) % dialSize;
		if (dialPosition == 0)
			answer++;
	}

	printf("[2025] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	const int64_t dialSize = 100;
	int64_t dialPosition = 50;
	for (const auto& line : ScanfEachLine<char, int64_t>(input, "%c%lld"))
	{
		const int64_t rotateBy = get<1>(line);
		for (int64_t i = 0; i < rotateBy; i++)
		{
			dialPosition = (dialPosition + (get<0>(line) == 'R' ? 1 : dialSize - 1)) % dialSize;
			if (dialPosition == 0)
				answer++;
		}
	}

	printf("[2025] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2025(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
