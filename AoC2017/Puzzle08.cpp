#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2017_Types
{
	using Registers = map<string, int64_t>;

	struct Instruction
	{
		string DestinationRegister;
		function<int64_t()> Amount;

		string TestRegister;
		function<bool(int64_t)> TestCondition;
	};
}

using namespace Puzzle08_2017_Types;

static vector<Instruction> ParseProgram(istream& input)
{
	const map<string, function<int64_t(int64_t)>> operations =
	{
		{ "inc", [](int64_t amount) { return amount; } },
		{ "dec", [](int64_t amount) { return -amount; } }
	};

	const map<string, function<bool(int64_t, int64_t)>> tests =
	{
		{ "==", [](int64_t amount, int64_t compareTo) { return amount == compareTo; } },
		{ "!=", [](int64_t amount, int64_t compareTo) { return amount != compareTo; } },
		{ ">", [](int64_t amount, int64_t compareTo) { return amount > compareTo; } },
		{ "<", [](int64_t amount, int64_t compareTo) { return amount < compareTo; } },
		{ ">=", [](int64_t amount, int64_t compareTo) { return amount >= compareTo; } },
		{ "<=", [](int64_t amount, int64_t compareTo) { return amount <= compareTo; } },
	};

	vector<Instruction> program;
	for (const auto& m : ReadEachLine(input, regex{ R"((\w+) (\w{3}) (.+) if (\w+) (.+) (.+))" }))
	{
		assert(m.empty() == false);

		Instruction inst;

		inst.DestinationRegister = m[1].str();
		inst.Amount = [op = operations.at(m[2].str()), amount = atoll(m[3].str().c_str())]() -> int64_t { return op(amount); };

		inst.TestRegister = m[4].str();
		inst.TestCondition = [test = tests.at(m[5].str()), amount = atoll(m[6].str().c_str())](int64_t regValue) -> bool { return test(regValue, amount); };

		program.push_back(inst);
	}
	return program;
}

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Registers registers;
	for (const Instruction& inst : program)
	{
		if (inst.TestCondition(registers[inst.TestRegister]))
		{
			registers[inst.DestinationRegister] += inst.Amount();
		}
	}

	int64_t answer = ranges::max(registers | views::values);

	printf("[2017] Puzzle08_A: %" PRId64 "\n", answer);
}


static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	int64_t answer = numeric_limits<int64_t>::min();

	Registers registers;
	for (const Instruction& inst : program)
	{
		if (inst.TestCondition(registers[inst.TestRegister]))
		{
			registers[inst.DestinationRegister] += inst.Amount();
		}
		answer = max(answer, ranges::max(registers | views::values));
	}

	printf("[2017] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2017(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
