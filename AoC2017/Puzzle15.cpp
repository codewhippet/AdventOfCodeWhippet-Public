#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2017_Types
{
	struct GeneratorOne
	{
		int64_t Factor;
		int64_t Current;

		int64_t Next()
		{
			Current = (Current * Factor) % 2147483647;
			return Current;
		}
	};

	struct GeneratorTwo
	{
		int64_t Factor;
		int64_t MultiplesOf;
		int64_t Current;

		int64_t Next()
		{
			do
			{
				Current = (Current * Factor) % 2147483647;

			} while ((Current % MultiplesOf) != 0);

			return Current;
		}
	};
}

using namespace Puzzle15_2017_Types;

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t rounds = 40000000;

	vector<tuple<int64_t>> generatorSeeds;
	ranges::copy(ScanfEachLine<int64_t>(input, "Generator %*c starts with %d"), back_inserter(generatorSeeds));

	GeneratorOne a{ 16807, get<0>(generatorSeeds[0]) };
	GeneratorOne b{ 48271, get<0>(generatorSeeds[1]) };

	int64_t answer = 0;
	for (int64_t i = 0; i < rounds; i++)
	{
		int64_t nextA = a.Next();
		int64_t nextB = b.Next();
		if ((nextA & 0xffff) == (nextB & 0xffff))
		{
			answer++;
		}
	}

	printf("[2017] Puzzle15_A: %" PRId64 "\n", answer);
}


static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t rounds = 5000000;

	vector<tuple<int64_t>> generatorSeeds;
	ranges::copy(ScanfEachLine<int64_t>(input, "Generator %*c starts with %d"), back_inserter(generatorSeeds));

	GeneratorTwo a{ 16807, 4, get<0>(generatorSeeds[0]) };
	GeneratorTwo b{ 48271, 8, get<0>(generatorSeeds[1]) };

	int64_t answer = 0;
	for (int64_t i = 0; i < rounds; i++)
	{
		int64_t nextA = a.Next();
		int64_t nextB = b.Next();
		if ((nextA & 0xffff) == (nextB & 0xffff))
		{
			answer++;
		}
	}

	printf("[2017] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2017(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
