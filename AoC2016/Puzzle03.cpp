#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2016_Types
{
}

using namespace Puzzle03_2016_Types;

static void Puzzle03_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	vector<vector<int64_t>> triangles;
	ranges::copy(lines
		| views::transform([](const string& s)
			{
				return ReadAsVectorOfNumbers(s);
			}),
		back_inserter(triangles));

	int64_t answer = ranges::count_if(triangles,
		[](const vector<int64_t>& t)
		{
			assert(t.size() == 3);
			return (
				(t[0] + t[1] > t[2]) &&
				(t[1] + t[2] > t[0]) &&
				(t[2] + t[0] > t[1])
				);
		});

	printf("[2016] Puzzle03_A: %" PRId64 "\n", answer);
}


static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	vector<vector<int64_t>> triangles;
	ranges::copy(lines
		| views::transform([](const string& s)
			{
				return ReadAsVectorOfNumbers(s);
			}),
		back_inserter(triangles));

	assert(triangles.size() % 3 == 0);
	for (size_t block = 0; block + 2 < triangles.size(); block += 3)
	{
		swap(triangles[block + 1][0], triangles[block + 0][0 + 1]);
		swap(triangles[block + 2][0], triangles[block + 0][0 + 2]);
		swap(triangles[block + 2][1], triangles[block + 1][0 + 2]);
	}

	int64_t answer = ranges::count_if(triangles,
		[](const vector<int64_t>& t)
		{
			assert(t.size() == 3);
			return (
				(t[0] + t[1] > t[2]) &&
				(t[1] + t[2] > t[0]) &&
				(t[2] + t[0] > t[1])
				);
		});

	printf("[2016] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2016(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
