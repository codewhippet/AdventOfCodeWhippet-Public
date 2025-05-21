#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2019_Types
{
}

using namespace Puzzle05_2019_Types;

static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	deque<int64_t> in{ 1 };
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);
	
	auto exec = puter.Execute();
	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;

	int64_t answer = out.back();

	printf("[2019] Puzzle05_A: %" PRId64 "\n", answer);
}


static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	deque<int64_t> in{ 5 };
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	auto exec = puter.Execute();
	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;

	int64_t answer = out.back();

	printf("[2019] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2019(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
