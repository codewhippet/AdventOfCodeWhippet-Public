#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2019_Types
{
}

using namespace Puzzle09_2019_Types;

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter;
	puter.ReadProgram(input);

	deque<int64_t> in{ 1 };
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	auto result = puter.Execute();
	assert(result == Intputer::ExecutionResult::Finished);
	(void)result;

	int64_t answer = out.front();

	printf("[2019] Puzzle09_A: %" PRId64 "\n", answer);
}


static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter;
	puter.ReadProgram(input);

	deque<int64_t> in{ 2 };
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	auto result = puter.Execute();
	assert(result == Intputer::ExecutionResult::Finished);
	(void)result;

	int64_t answer = out.front();

	printf("[2019] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2019(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
