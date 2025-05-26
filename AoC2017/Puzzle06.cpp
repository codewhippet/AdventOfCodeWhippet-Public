#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2017_Types
{
}

using namespace Puzzle06_2017_Types;

static size_t FindLargestBucketIndex(const vector<int64_t>& buckets)
{
	size_t largestElementIndex = 0;
	for (size_t i = 1; i < buckets.size(); i++)
	{
		if (buckets[i] > buckets[largestElementIndex])
		{
			largestElementIndex = i;
		}
	}
	return largestElementIndex;
}

static void DistributeLargestBucket(vector<int64_t>* buckets)
{
	size_t largestBucket = FindLargestBucketIndex(*buckets);
	int64_t distribute = (*buckets)[largestBucket];
	(*buckets)[largestBucket] = 0;
	for (int64_t sprinkle = 0; sprinkle < distribute; sprinkle++)
	{
		(*buckets)[(largestBucket + sprinkle + 1) % buckets->size()]++;
	}
}

static void Puzzle06_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> buckets = ReadAsVectorOfNumbers(ReadSingleLine(input));

	int64_t answer = 0;

	set<vector<int64_t>> states{ buckets };
	while (true)
	{
		answer++;

		DistributeLargestBucket(&buckets);
		
		if (states.contains(buckets))
		{
			break;
		}

		states.insert(buckets);
	}

	printf("[2017] Puzzle06_A: %" PRId64 "\n", answer);
}


static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> buckets = ReadAsVectorOfNumbers(ReadSingleLine(input));

	int64_t answer = 0;

	int64_t steps = 0;
	map<vector<int64_t>, int64_t> states{ { buckets, 0 } };
	while (true)
	{
		steps++;

		DistributeLargestBucket(&buckets);

		if (states.contains(buckets))
		{
			answer = steps - states.at(buckets);
			break;
		}

		states.insert({ buckets, steps });
	}

	printf("[2017] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2017(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
