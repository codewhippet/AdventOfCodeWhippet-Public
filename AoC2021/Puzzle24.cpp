#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2021_Types
{
	struct Stage
	{
		int64_t Divisor;
		int64_t TestOffset;
		int64_t Increment;
	};
}

using namespace Puzzle24_2021_Types;

static vector<Stage> ExtractStages(istream& input)
{
	const size_t numStages = 14;
	const int64_t linesPerStage = 18;

	vector<Stage> stages(numStages);
	for (const auto &line : ReadEachLineWithNumber(input))
	{
		int64_t stage = line.second / linesPerStage;
		switch (line.second % linesPerStage)
		{
		case 4:
			{
				int scanned = sscanf(line.first.c_str(), "div z %lld", &stages[stage].Divisor);
				assert(scanned == 1);
				(void)scanned;
			}
			break;
		case 5:
			{
				int scanned = sscanf(line.first.c_str(), "add x %lld", &stages[stage].TestOffset);
				assert(scanned == 1);
				(void)scanned;
			}
			break;
		case 15:
			{
				int scanned = sscanf(line.first.c_str(), "add y %lld", &stages[stage].Increment);
				assert(scanned == 1);
				(void)scanned;
			}
			break;
		}
	}
	return stages;
}

static int64_t ExecuteStage(const Stage& s, int64_t input, int64_t z)
{
	int64_t w = input;
	int64_t x = z % 26;
	z = z / s.Divisor;

	bool mismatch = w != (x + s.TestOffset);
	if (mismatch)
	{
		z = (z * 26) + w + s.Increment;
	}

	return z;
}

static bool IsPossibleToFindValid(const vector<Stage>& stages, const vector<int64_t>& fixedInputs)
{
	map<int, set<int64_t>> stageOutputs;

	// Z always starts off as 0
	stageOutputs[-1].insert(0);

	// For all input slots
	for (int i = 0; i < 14; i++)
	{
		set<int64_t>& currentStage = stageOutputs[i];
		set<int64_t>& previousStage = stageOutputs[i - 1];

		// For all possible values of Z
		for (int64_t prevZ : previousStage)
		{
			int64_t inputMin = 1;
			int64_t inputMax = 10;

			// Fixed input
			if (i < fixedInputs.size())
			{
				inputMin = fixedInputs[i];
				inputMax = inputMin + 1;
			}

			// For all possible inputs into this stage
			for (int64_t input = inputMin; input < inputMax; input++)
			{
				int64_t newZ = ExecuteStage(stages[i], input, prevZ);
				currentStage.insert(newZ);
			}
		}
	}
	return stageOutputs[13].find(0) != stageOutputs[13].end();
}

static void Puzzle24_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Stage> stages = ExtractStages(input);

	vector<int64_t> fixedInputs;
	while (fixedInputs.size() < 14)
	{
		for (int64_t i = 9; i > 0; i--)
		{
			fixedInputs.push_back(i);

			bool possible = IsPossibleToFindValid(stages, fixedInputs);
			if (possible)
			{
				break;
			}
			fixedInputs.pop_back();
		}
	}

	int64_t answer = accumulate(fixedInputs.begin(), fixedInputs.end(), 0ll, [](int64_t a, int64_t b) { return (a * 10) + b; });

	printf("[2021] Puzzle24_A: %" PRId64 "\n", answer);
}

static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Stage> stages = ExtractStages(input);

	vector<int64_t> fixedInputs;
	while (fixedInputs.size() < 14)
	{
		for (int64_t i = 1; i < 10; i++)
		{
			fixedInputs.push_back(i);

			bool possible = IsPossibleToFindValid(stages, fixedInputs);
			if (possible)
			{
				break;
			}
			fixedInputs.pop_back();
		}
	}

	int64_t answer = accumulate(fixedInputs.begin(), fixedInputs.end(), 0ll, [](int64_t a, int64_t b) { return (a * 10) + b; });

	printf("[2021] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2021(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
