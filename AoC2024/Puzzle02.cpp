#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2024_Types
{
}

using namespace Puzzle02_2024_Types;

static bool IsPositive(int a)
{
	return a >= 0;
}

static bool IsSafe(const vector<int>& levels)
{
	bool ascending = IsPositive(levels[1] - levels[0]);
	for (size_t i = 0; i + 1 < levels.size(); i++)
	{
		int diff = levels[i + 1] - levels[i];
		if (abs(diff) < 1 || abs(diff) > 3)
		{
			return false;
		}

		if (IsPositive(diff) != ascending)
		{
			return false;
		}
	}
	return true;
}

static bool CanBeMadeSafe(const vector<int>& levels)
{
	for (size_t i = 0; i < levels.size(); i++)
	{
		vector<int> newLevels = levels;
		newLevels.erase(newLevels.begin() + i);
		if (IsSafe(newLevels))
		{
			return true;
		}
	}
	return false;
}

static void Puzzle02_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	for (const string& line : ReadAllLines(input))
	{
		vector<int> list = Enumerable::Tokens(line, " ")
			->Convert<int>()
			->ToVector();
		answer += (IsSafe(list) ? 1 : 0);
	}

	printf("[2024] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	for (const string& line : ReadAllLines(input))
	{
		vector<int> list = Enumerable::Tokens(line, " ")
			->Convert<int>()
			->ToVector();
		answer += (IsSafe(list) || CanBeMadeSafe(list) ? 1 : 0);
	}

	printf("[2024] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2024(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
