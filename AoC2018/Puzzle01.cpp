#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2018_Types
{
}

using namespace Puzzle01_2018_Types;

static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto changes = views::elements<0>(ScanfEachLine<int64_t>(input, "%lld"));
	int64_t answer = accumulate(changes.begin(), changes.end(), 0ll);

	printf("[2018] Puzzle01_A: %" PRId64 "\n", answer);
}


static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> changes;
	ranges::copy(ScanfEachLine<int64_t>(input, "%lld") | views::elements<0>, back_inserter(changes));

	size_t changeIndex = 0;

	int64_t answer = 0;
	set<int64_t> frequencies{ answer };
	while (true)
	{
		answer += changes[changeIndex++ % changes.size()];
		if (frequencies.contains(answer))
		{
			break;
		}
		frequencies.insert(answer);
	}

	printf("[2018] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2018(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
