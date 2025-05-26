#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2017_Types
{
	enum class Opcode
	{
		Set,
		Sub,
		Mul,
		Jnz,
	};

	enum class ArgType : uint8_t
	{
		Invalid,
		Immediate,
		Register,
	};

	struct Instruction
	{
		Opcode Op;
		pair<ArgType, int64_t> A = { ArgType::Invalid, 0 };
		pair<ArgType, int64_t> B = { ArgType::Invalid, 0 };
	};

	struct Computer
	{
		int64_t PC = 0;
		map<char, int64_t> Registers;
		int64_t MulCount = 0;
	};
}

using namespace Puzzle23_2017_Types;

static pair<ArgType, int64_t> ParseParam(const char* param)
{
	return isalpha(param[0]) ? pair<ArgType, int64_t>{ ArgType::Register, param[0] } : pair<ArgType, int64_t>{ ArgType::Immediate, atoi(param) };
}

static Instruction ParseDoubleArgInstruction(Opcode op, const char* format, const string& line)
{
	char paramA[16] = { 0 };
	char paramB[16] = { 0 };

	int scanned = sscanf(line.c_str(), format, paramA, paramB);
	assert(scanned == 2);
	(void)scanned;

	return { op, ParseParam(paramA), ParseParam(paramB) };
}

static vector<Instruction> ParseProgram(istream& input)
{
	vector<Instruction> program;

	const vector<pair<string_view, function<Instruction(const string&)>>> assemblyTable =
	{
		{ "set"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Set, "set %s %s", line); } },
		{ "sub"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Sub, "sub %s %s", line); } },
		{ "mul"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Mul, "mul %s %s", line); } },
		{ "jnz"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Jnz, "jnz %s %s", line); } },
	};

	for (const string& line : ReadEachLine(input))
	{
		for (const auto& asmInst : assemblyTable)
		{
			if (line.starts_with(asmInst.first))
			{
				program.push_back(asmInst.second(line));
				break;
			}
		}
	}

	return program;
}

static void Execute(const vector<Instruction>& program, Computer* computer)
{
	auto ReadParam = [&](const pair<ArgType, int64_t>& param) -> int64_t
		{
			if (param.first == ArgType::Immediate)
			{
				return param.second;
			}

			if (param.first == ArgType::Register)
			{
				return computer->Registers[(char)param.second];
			}

			assert(false);
			return 0;
		};

	while ((computer->PC >= 0) && (computer->PC < (int64_t)program.size()))
	{
		const Instruction& currentInst = program[computer->PC];
		switch (currentInst.Op)
		{
		case Opcode::Set:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] = ReadParam(currentInst.B);
			break;
		case Opcode::Sub:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] -= ReadParam(currentInst.B);
			break;
		case Opcode::Mul:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] *= ReadParam(currentInst.B);
			computer->MulCount++;
			break;
		case Opcode::Jnz:
			if (ReadParam(currentInst.A) != 0)
			{
				computer->PC += ReadParam(currentInst.B) - 1;
			}
			break;
		}
		computer->PC += 1;
	}
}

static int64_t SearchForCompositeNumbers(int64_t from, int64_t to, int64_t increment)
{
	int64_t composite = 0;
	for (int64_t i = from; i <= to; i += increment)
	{
		for (int64_t j = 2; j < (i / 2); j++)
		{
			if ((i % j) == 0)
			{
				composite++;
				break;
			}
		}
	}
	return composite;
}

static void Puzzle23_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);
	Computer comp;
	Execute(program, &comp);

	int64_t answer = comp.MulCount;

	printf("[2017] Puzzle23_A: %" PRId64 "\n", answer);
}


static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	auto ReadConstant = [](const pair<ArgType, int64_t>& param) -> int64_t
		{
			if (param.first == ArgType::Immediate)
			{
				return param.second;
			}

			assert(false);
			return 0;
		};

	int64_t from = ReadConstant(program[0].B);
	from *= ReadConstant(program[4].B);
	from -= ReadConstant(program[5].B);

	int64_t to = from;
	to -= ReadConstant(program[7].B);

	int64_t increment = -ReadConstant(program[30].B);

	int64_t answer = SearchForCompositeNumbers(from, to, increment);

	printf("[2017] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2017(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
