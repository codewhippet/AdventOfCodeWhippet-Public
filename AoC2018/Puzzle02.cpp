#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2018_Types
{
}

using namespace Puzzle02_2018_Types;

static string RemoveDifferences(const string& a, const string& b)
{
	assert(a.size() == b.size());

	string same;
	for (size_t i = 0; i < a.size(); i++)
	{
		if (a[i] == b[i])
		{
			same += a[i];
		}
	}
	return same;
}

static void Puzzle02_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t twoCounts = 0;
	int64_t threeCounts = 0;
	for (const string& line : ReadEachLine(input))
	{
		map<char, int64_t> charCounts;
		for (char c : line)
		{
			charCounts[c]++;
		}
		twoCounts += ranges::count_if(charCounts, [](const auto& kvp) { return kvp.second == 2; }) > 0 ? 1 : 0;
		threeCounts += ranges::count_if(charCounts, [](const auto& kvp) { return kvp.second == 3; }) > 0 ? 1 : 0;
	}

	int64_t answer = twoCounts * threeCounts;

	printf("[2018] Puzzle02_A: %" PRId64 "\n", answer);
}


static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string answer;

	vector<string> ids = ReadAllLines(input);
	for (const auto& p : AllUnorderedPairs(ids.size()))
	{
		string same = RemoveDifferences(ids[p.first], ids[p.second]);
		if (same.size() == ids[0].size() - 1)
		{
			answer = same;
			break;
		}
	}

	printf("[2018] Puzzle02_B: %s\n", answer.c_str());
}

void Puzzle02_2018(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
