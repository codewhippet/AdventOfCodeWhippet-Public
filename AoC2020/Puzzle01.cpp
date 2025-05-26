#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2020_Types
{
}

using namespace Puzzle01_2020_Types;


static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<int> numbers = MakeEnumerator(ReadAllLines(input))
		->Select<int>([](const string& line)
			{
				int number;
				int parsed = sscanf(line.c_str(), "%d", &number);
				assert(parsed == 1);
				(void)parsed;
				return number;
			})
		->ToSet();

	int64_t answer = 0;

	for (int a : numbers)
	{
		int opposite = 2020 - a;
		if (numbers.find(opposite) != numbers.end())
		{
			answer = a * opposite;
			break;
		}
	}

	printf("[2020] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> numbers = MakeEnumerator(ReadAllLines(input))
		->Select<int>([](const string& line)
			{
				int number;
				int parsed = sscanf(line.c_str(), "%d", &number);
				(void)parsed;
				assert(parsed == 1);
				return number;
			})
		->ToVector();

	int64_t answer = 0;

	set<int> numberSet = MakeEnumerator(numbers)->ToSet();
	for (size_t i = 0; i < numbers.size() && answer == 0; i++)
	{
		for (size_t j = i + 1; j < numbers.size(); j++)
		{
			int remaining = 2020 - numbers[i] - numbers[j];
			if (numberSet.find(remaining) != numberSet.end())
			{
				answer = numbers[i] * numbers[j] * remaining;
				break;
			}
		}
	}

	printf("[2020] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2020(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
