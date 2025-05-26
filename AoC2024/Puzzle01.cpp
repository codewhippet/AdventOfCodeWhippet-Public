#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2024_Types
{
}

using namespace Puzzle01_2024_Types;

static void Puzzle01_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<vector<int>> lists;
	lists.resize(2);

	for (const auto& line : ScanfEachLine<int, int>(input, "%d %d"))
	{
		lists[0].push_back(get<0>(line));
		lists[1].push_back(get<1>(line));
	}

	ranges::sort(lists[0]);
	ranges::sort(lists[1]);

	int64_t answer = 0;

	for (size_t i = 0; i < lists[0].size(); i++)
	{
		answer += abs(lists[0][i] - lists[1][i]);
	}

	printf("[2024] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> leftList;
	map<int, int> rightCounts;

	for (const auto& line : ScanfEachLine<int, int>(input, "%d %d"))
	{
		leftList.push_back(get<0>(line));
		rightCounts[get<1>(line)]++;
	}

	auto counts = leftList | ranges::views::transform([&rightCounts](int i)
		{
			return i * rightCounts[i];
		});

	int64_t answer = accumulate(counts.begin(), counts.end(), 0);

	printf("[2024] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2024(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
