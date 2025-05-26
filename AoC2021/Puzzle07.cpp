#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2021_Types
{
}

using namespace Puzzle07_2021_Types;

static void Puzzle07_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> positions = ReadAsVectorOfNumbers(ReadSingleLine(input));
	int64_t maxPosition = MakeEnumerator(positions)->Max();

	vector<int64_t> crabCounts(maxPosition + 1);
	for (int64_t crab : positions)
	{
		crabCounts[crab]++;
	}

	vector<int64_t> increaseBy(maxPosition + 1);
	{
		increaseBy[0] = crabCounts[0];
		for (size_t pos = 1; pos < crabCounts.size(); pos++)
		{
			increaseBy[pos] = increaseBy[pos - 1] + crabCounts[pos];
		}
	}

	vector<int64_t> reduceBy(maxPosition + 1);
	{
		reduceBy.back() = 0;
		for (size_t pos = crabCounts.size() - 1; pos > 0; pos--)
		{
			reduceBy[pos - 1] = reduceBy[pos] + crabCounts[pos];
		}
	}

	vector<int64_t> fuelUsed(maxPosition + 1);
	{
		int64_t fuel = 0;
		for (size_t pos = 0; pos < crabCounts.size(); pos++)
		{
			fuelUsed[pos] = fuel;
			fuel = fuel + increaseBy[pos] - reduceBy[pos];
		}
	}

	int64_t pos = 0;
	pair<int64_t, int64_t> targetPosition = MakeEnumerator(fuelUsed)
		->Select<pair<int64_t, int64_t>>([&pos](const int64_t& delta) { return make_pair(delta, pos++); })
		->Min();

	int64_t answer = 0;
	for (int64_t i = 0; i < (int64_t)crabCounts.size(); i++)
	{
		answer += abs(i - targetPosition.second) * crabCounts[i];
	}

	printf("[2021] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> positions = ReadAsVectorOfNumbers(ReadSingleLine(input));
	int64_t maxPosition = MakeEnumerator(positions)->Max();

	vector<int64_t> crabCounts(maxPosition + 1);
	for (int64_t crab : positions)
	{
		crabCounts[crab]++;
	}

	vector<int64_t> fuelUsed(maxPosition + 1);
	{
		for (int64_t crabPos = 0; crabPos < (int64_t)crabCounts.size(); crabPos++)
		{
			for (int64_t fuelUsedPos = 0; fuelUsedPos < (int64_t)fuelUsed.size(); fuelUsedPos++)
			{
				int64_t diff = abs(crabPos - fuelUsedPos);
				int64_t triangle = (diff * (diff + 1)) / 2;
				fuelUsed[fuelUsedPos] += triangle * crabCounts[crabPos];
			}
		}
	}

	int pos = 0;
	pair<int64_t, int64_t> targetPosition = MakeEnumerator(fuelUsed)
		->Select<pair<int64_t, int64_t>>([&pos](const int64_t& delta) { return make_pair(delta, pos++); })
		->Min();

	int64_t answer = targetPosition.first;

	printf("[2021] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2021(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
