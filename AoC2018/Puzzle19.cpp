#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2018_Types
{
	enum class Opcode
	{
		addr,
		addi,
		mulr,
		muli,
		banr,
		bani,
		borr,
		bori,
		setr,
		seti,
		gtir,
		gtri,
		gtrr,
		eqir,
		eqri,
		eqrr,
	};

	struct Instruction
	{
		Opcode Op;
		array<int64_t, 3> Args = { 0, 0, 0 };
	};

	struct Computer
	{
		int64_t& PC()
		{
			return Registers[PCReg];
		}

		int64_t PCReg = 0;
		array<int64_t, 6> Registers = { 0, 0, 0, 0, 0, 0 };
	};
}

using namespace Puzzle19_2018_Types;

static Instruction ParseTripleArgInstruction(Opcode op, const string& line)
{
	int64_t a, b, c;

	int scanned = sscanf(line.c_str(), "%*s %lld %lld %lld", &a, &b, &c);
	assert(scanned == 3);
	(void)scanned;

	return { op, { a, b, c } };
}

static pair<vector<Instruction>, int64_t> ParseProgram(istream& input)
{
	vector<Instruction> program;

	const vector<pair<string_view, function<Instruction(const string&)>>> assemblyTable =
	{
		{ "addr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::addr, line); } },
		{ "addi"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::addi, line); } },
		{ "mulr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::mulr, line); } },
		{ "muli"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::muli, line); } },
		{ "banr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::banr, line); } },
		{ "bani"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::bani, line); } },
		{ "borr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::borr, line); } },
		{ "bori"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::bori, line); } },
		{ "setr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::setr, line); } },
		{ "seti"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::seti, line); } },
		{ "gtir"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::gtir, line); } },
		{ "gtri"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::gtri, line); } },
		{ "gtrr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::gtrr, line); } },
		{ "eqir"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::eqir, line); } },
		{ "eqri"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::eqri, line); } },
		{ "eqrr"sv, [](const string& line) { return ParseTripleArgInstruction(Opcode::eqrr, line); } },
	};

	int64_t pcRegister = 0;
	for (const string& line : ReadEachLine(input))
	{
		for (const auto& asmInst : assemblyTable)
		{
			if (line.starts_with(asmInst.first))
			{
				program.push_back(asmInst.second(line));
				break;
			}
			if (line.starts_with("#ip"sv))
			{
				int scanned = sscanf(line.c_str(), "#ip %lld", &pcRegister);
				assert(scanned == 1);
				(void)scanned;
			}
		}
	}

	return { program, pcRegister };
}

static void Execute(const vector<Instruction>& program, Computer* computer, int64_t runFor)
{
	const size_t A = 0;
	const size_t B = 1;
	const size_t C = 2;

	for (int64_t i = 0; i < runFor; i++)
	{
		const Instruction& currentInst = program[computer->PC()];
		switch (currentInst.Op)
		{
		case Opcode::addr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] + computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::addi:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] + currentInst.Args[B];
			break;
		case Opcode::mulr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] * computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::muli:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] * currentInst.Args[B];
			break;
		case Opcode::banr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] & computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::bani:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] & currentInst.Args[B];
			break;
		case Opcode::borr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] | computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::bori:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] | currentInst.Args[B];
			break;
		case Opcode::setr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]];
			break;
		case Opcode::seti:
			computer->Registers[currentInst.Args[C]] = currentInst.Args[A];
			break;
		case Opcode::gtir:
			computer->Registers[currentInst.Args[C]] = currentInst.Args[A] > computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::gtri:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] > currentInst.Args[B];
			break;
		case Opcode::gtrr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] > computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::eqir:
			computer->Registers[currentInst.Args[C]] = currentInst.Args[A] == computer->Registers[currentInst.Args[B]];
			break;
		case Opcode::eqri:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] == currentInst.Args[B];
			break;
		case Opcode::eqrr:
			computer->Registers[currentInst.Args[C]] = computer->Registers[currentInst.Args[A]] == computer->Registers[currentInst.Args[B]];
			break;
		}

		int64_t nextPc = computer->PC() + 1;
		if ((nextPc < 0) || (nextPc >= (int64_t)program.size()))
		{
			break;
		}
		computer->PC() = nextPc;
	}
}

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [program, pcRegister] = ParseProgram(input);

	Computer comp;
	comp.PCReg = pcRegister;
	Execute(program, &comp, numeric_limits<int64_t>::max());

	int64_t answer = comp.Registers[0];

	printf("[2018] Puzzle19_A: %" PRId64 "\n", answer);
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [program, pcRegister] = ParseProgram(input);

	Computer comp;
	comp.PCReg = pcRegister;
	comp.Registers[0] = 1;
	Execute(program, &comp, program.size());

	int64_t target = comp.Registers[5];

	int64_t answer = 0;
	for (int64_t i = 1; i <= target; i++)
	{
		if ((target % i) == 0)
		{
			answer += i;
		}
	}

	printf("[2018] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2018(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
