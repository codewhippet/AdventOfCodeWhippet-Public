#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2016_Types
{
}

using namespace Puzzle06_2016_Types;

static string Decode(istream& input, int64_t weighting)
{
	map<size_t, map<char, int64_t>> columnCounts;
	for (const auto& line : ReadEachLine(input))
	{
		for (size_t column = 0; column < line.size(); column++)
		{
			columnCounts[column][line[column]] += weighting;
		}
	}

	string decoded;
	for (const auto& counts : columnCounts)
	{
		set<pair<int64_t, char>> commonCounts;
		ranges::copy(counts.second
			| views::transform([](const auto& kvp)
				{
					return make_pair(kvp.second, kvp.first);
				}),
			inserter(commonCounts, commonCounts.end()));
		decoded += commonCounts.begin()->second;
	}

	return decoded;
}

static void Puzzle06_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string answer = Decode(input, -1);

	printf("[2016] Puzzle06_A: %s\n", answer.c_str());
}


static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string answer = Decode(input, 1);

	printf("[2016] Puzzle06_B: %s\n", answer.c_str());
}

void Puzzle06_2016(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
