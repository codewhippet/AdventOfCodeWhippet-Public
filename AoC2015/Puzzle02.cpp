#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2015_Types
{
}

using namespace Puzzle02_2015_Types;

static void Puzzle02_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		int64_t length = 0;
		int64_t width = 0;
		int64_t height = 0;
		int parsed = sscanf(line.c_str(), "%" SCNd64 "x%" SCNd64 "x%" SCNd64 "", &length, &width, &height);
		assert(parsed == 3);
		(void)parsed;

		int64_t lw = length * width;
		int64_t wh = width * height;
		int64_t hl = height * length;

		answer += (2 * lw) + (2 * wh) + (2 * hl) + min(min(lw, wh), hl);
	}

	printf("[2015] Puzzle02_A: %" PRId64 "\n", answer);
}


static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		vector<int64_t> dimensions(3);
		int parsed = sscanf(line.c_str(), "%" SCNd64 "x%" SCNd64 "x%" SCNd64 "", &dimensions[0], &dimensions[1], &dimensions[2]);
		assert(parsed == 3);
		(void)parsed;

		ranges::sort(dimensions);

		int64_t around = (2 * dimensions[0]) + (2 * dimensions[1]);
		int64_t bow = accumulate(dimensions.begin(), dimensions.end(), 1ll, multiplies{});

		answer += around + bow;
	}

	printf("[2015] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2015(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
