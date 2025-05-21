#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2022_Types
{
}

using namespace Puzzle10_2022_Types;

static vector<int64_t> GetCycleValues(istream& input)
{
	vector<int64_t> cycleValues;

	int64_t xRegister = 1;
	for (const auto& line : ReadEachLine(input))
	{
		if (line == "noop"sv)
		{
			cycleValues.push_back(xRegister);
		}
		else
		{
			int64_t value = 0;
			int scanned = sscanf(line.c_str(), "addx %lld", &value);
			assert(scanned == 1);
			(void)scanned;

			cycleValues.push_back(xRegister);
			cycleValues.push_back(xRegister);

			xRegister += value;
		}
	}

	return cycleValues;
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> cycleValues = GetCycleValues(input);

	int64_t answer =
		cycleValues[20 - 1] * 20 +
		cycleValues[60 - 1] * 60 +
		cycleValues[100 - 1] * 100 +
		cycleValues[140 - 1] * 140 +
		cycleValues[180 - 1] * 180 +
		cycleValues[220 - 1] * 220;

	printf("[2022] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> cycleValues = GetCycleValues(input);

	string answer;
	for (size_t i = 0; i < cycleValues.size(); i++)
	{
		if ((i % 40) == 0)
			answer += '\n';

		size_t vOffset = i % 40;
		int64_t delta = cycleValues[i] - vOffset;
		bool on = (delta == -1) || (delta == 0) || (delta == 1);
		answer += on ? '#' : '.';
	}

	printf("[2022] Puzzle10_B:%s\n", answer.c_str());
}

void Puzzle10_2022(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
