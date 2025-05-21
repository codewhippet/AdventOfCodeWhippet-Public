#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2020_Types
{
}

using namespace Puzzle05_2020_Types;


static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> passes = MakeEnumerator(ReadAllLines(input))
		->Select<int>([](const string& s)
			{
				int v = 0;
				for (char c : s)
				{
					v <<= 1;
					int bit = (c == 'F' || c == 'L' ? 0 : 1);
					v |= bit;
				}
				return v;
			})
		->ToVector();

	sort(passes.begin(), passes.end());

	int64_t answer = passes.back();

	printf("[2020] Puzzle05_A: %" PRId64 "\n", answer);
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> passes = MakeEnumerator(ReadAllLines(input))
		->Select<int>([](const string& s)
			{
				int v = 0;
				for (char c : s)
				{
					v <<= 1;
					int bit = (c == 'F' || c == 'L' ? 0 : 1);
					v |= bit;
				}
				return v;
			})
		->ToVector();

	sort(passes.begin(), passes.end());

	int64_t answer = 0;
	for (size_t i = 0; i + 1 < passes.size(); i++)
	{
		if (passes[i + 1] != (passes[i] + 1))
		{
			answer = passes[i] + 1;
			break;
		}
	}

	printf("[2020] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2020(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
