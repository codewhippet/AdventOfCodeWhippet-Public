#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2017_Types
{
}

using namespace Puzzle02_2017_Types;

static void Puzzle02_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		vector<int64_t> row = ReadAsVectorOfNumbers(line);
		ranges::sort(row);
		answer += row.back() - row.front();
	}

	printf("[2017] Puzzle02_A: %" PRId64 "\n", answer);
}


static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		vector<int64_t> row = ReadAsVectorOfNumbers(line);
		for (const auto& p : AllUnorderedPairs(row.size()))
		{
			int64_t a = row[p.first];
			int64_t b = row[p.second];

			if ((a > b) && ((a % b) == 0))
			{
				answer += a / b;
			}
			else if ((b > a) && ((b % a) == 0))
			{
				answer += b / a;
			}
		}
	}

	printf("[2017] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2017(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
