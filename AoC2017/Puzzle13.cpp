#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2017_Types
{

}

using namespace Puzzle13_2017_Types;

static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<tuple<int64_t, int64_t>> layers;
	ranges::copy(ScanfEachLine<int64_t, int64_t>(input, "%d: %d"), back_inserter(layers));

	auto severity = layers | views::transform(
		[&](const auto& layer)
		{
			int64_t offset = get<0>(layer);
			int64_t depth = get<1>(layer);
			int64_t period = (depth - 1) * 2;
			return (offset % period == 0 ? offset * depth : 0);
		});

	int64_t answer = accumulate(severity.begin(), severity.end(), 0ll);

	printf("[2017] Puzzle13_A: %" PRId64 "\n", answer);
}


static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<tuple<int64_t, int64_t>> layers;
	ranges::copy(ScanfEachLine<int64_t, int64_t>(input, "%d: %d"), back_inserter(layers));

	int64_t answer = 0;
	for (size_t i = 0; i < 100000000; i++)
	{
		auto severity = layers | views::transform(
			[&](const auto& layer)
			{
				int64_t offset = get<0>(layer) + i;
				int64_t depth = get<1>(layer);
				int64_t period = (depth - 1) * 2;
				return (offset % period == 0 ? offset * depth : 0);
			});

		int64_t caught = accumulate(severity.begin(), severity.end(), 0ll);
		if (caught == 0)
		{
			answer = i;
			break;
		}
	}

	printf("[2017] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2017(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
