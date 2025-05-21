#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2019_Types
{
}

using namespace Puzzle07_2019_Types;

static int64_t SignalFromPhasesPart1(const vector<int64_t>& program, const vector<int64_t>& phases)
{
	vector<Intputer> amplifiers(5);
	vector<deque<int64_t>> ioQueues(6);
	for (size_t i = 0; i < amplifiers.size(); i++)
	{
		amplifiers[i].CopyProgram(program);
		amplifiers[i].SetReadWriteQueues(&ioQueues[i], &ioQueues[i + 1]);
	}

	// Starting phases
	for (size_t i = 0; i < phases.size(); i++)
	{
		ioQueues[i].push_back(phases[i]);
	}

	// Starting signal
	ioQueues[0].push_back(0);

	for (size_t amp = 0; amp < amplifiers.size(); amp++)
	{
		auto exec = amplifiers[amp].Execute();
		assert(exec == Intputer::ExecutionResult::Finished);
		(void)exec;
	}

	return ioQueues.back().back();
}

static int64_t SignalFromPhasesPart2(const vector<int64_t>& program, const vector<int64_t>& phases)
{
	vector<Intputer> amplifiers(5);
	vector<deque<int64_t>> ioQueues(5);
	for (size_t i = 0; i < amplifiers.size(); i++)
	{
		amplifiers[i].CopyProgram(program);
		amplifiers[i].SetReadWriteQueues(&ioQueues[i], &ioQueues[(i + 1) % ioQueues.size()]);
	}

	// Starting phases
	for (size_t i = 0; i < phases.size(); i++)
	{
		ioQueues[i].push_back(phases[i]);
	}

	// Starting signal
	ioQueues[0].push_back(0);

	size_t finished = 0;
	do
	{
		for (size_t amp = 0; amp < amplifiers.size(); amp++)
		{
			auto exec = amplifiers[amp].Execute();
			finished += exec == Intputer::ExecutionResult::Finished ? 1 : 0;
		}

		assert((finished == 0) || (finished == amplifiers.size()));

	} while (finished < amplifiers.size());

	return ioQueues.front().back();
}

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const auto startingProgram = ReadAsVectorOfNumbers(ReadSingleLine(input));

	int64_t maximumSignal = numeric_limits<int64_t>::min();

	vector<int64_t> phases{ 0, 1, 2, 3, 4 };
	do
	{
		maximumSignal = max(maximumSignal, SignalFromPhasesPart1(startingProgram, phases));

	} while (next_permutation(phases.begin(), phases.end()));

	int64_t answer = maximumSignal;

	printf("[2019] Puzzle07_A: %" PRId64 "\n", answer);
}


static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const auto startingProgram = ReadAsVectorOfNumbers(ReadSingleLine(input));

	int64_t maximumSignal = numeric_limits<int64_t>::min();

	vector<int64_t> phases{ 5, 6, 7, 8, 9 };
	do
	{
		maximumSignal = max(maximumSignal, SignalFromPhasesPart2(startingProgram, phases));

	} while (next_permutation(phases.begin(), phases.end()));

	int64_t answer = maximumSignal;

	printf("[2019] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2019(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
