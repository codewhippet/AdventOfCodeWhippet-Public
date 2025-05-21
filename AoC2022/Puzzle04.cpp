#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2022_Types
{
}

using namespace Puzzle04_2022_Types;

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const auto &line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t>(input, "%lld-%lld,%lld-%lld"))
	{
		int64_t firstBegin = get<0>(line);
		int64_t firstEnd = get<1>(line);

		int64_t secondBegin = get<2>(line);
		int64_t secondEnd = get<3>(line);

		bool firstContainsSecond = (firstBegin <= secondBegin) && (firstEnd >= secondEnd);
		bool secondContainsFirst = (secondBegin <= firstBegin) && (secondEnd >= firstEnd);

		if (firstContainsSecond || secondContainsFirst)
		{
			answer++;
		}
	}

	printf("[2022] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const auto &line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t>(input, "%lld-%lld,%lld-%lld"))
	{
		int64_t firstBegin = get<0>(line);
		int64_t firstEnd = get<1>(line);

		int64_t secondBegin = get<2>(line);
		int64_t secondEnd = get<3>(line);

		bool firstLess = firstEnd < secondBegin;
		bool secondLess = secondEnd < firstBegin;
		bool isDisjoint = firstLess || secondLess;

		if (isDisjoint == false)
		{
			answer++;
		}
	}

	printf("[2022] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2022(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
