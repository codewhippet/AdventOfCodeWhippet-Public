#include "stdafx.h"
#include "Intputer.h"
#include <iostream>

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2019_Types
{
}

using namespace Puzzle21_2019_Types;

#if 0
static void InteractiveMode(Intputer* puter)
{
	for (int lineIndex = 0; lineIndex < 20; lineIndex++)
	{
		puter->GetWriteQueue()->clear();

		auto exec = puter->Execute();
		assert(exec != Intputer::ExecutionResult::Exception);

		ranges::for_each(*puter->GetWriteQueue() | views::filter([](int64_t c) { return c < 256; }),
			[](int64_t c) { putc((int)c, stdout); });

		printf("[% 2d] $: ", lineIndex);
		string line;
		getline(cin, line);

		if (exec == Intputer::ExecutionResult::Finished)
			break;

		ranges::copy(line, back_inserter(*puter->GetReadQueue()));
		puter->GetReadQueue()->push_back('\n');
	}
}
#endif

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	IntputerIO io;
	puter.SetReadWriteQueues(&io);

	string springbotProgram =
		R"(OR A T
		AND B T
		AND C T
		NOT T J
		AND D J
		WALK
		)";

	ranges::copy(springbotProgram | views::filter([](char c) { return c != '\t'; }), back_inserter(io.Read));

	auto exec = puter.Execute();
	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;

	int64_t answer = io.Write.back();

	printf("[2019] Puzzle21_A: %" PRId64 "\n", answer);
}


static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	IntputerIO io;
	puter.SetReadWriteQueues(&io);

	string springbotProgram =
		R"(OR A T
		AND B T
		AND C T
		NOT T J
		AND D J
		NOT E T
		NOT T T
		OR H T
		AND T J
		RUN
		)";

	ranges::copy(springbotProgram | views::filter([](char c) { return c != '\t'; }), back_inserter(io.Read));

	puter.Execute();

	int64_t answer = io.Write.back();

	printf("[2019] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2019(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
