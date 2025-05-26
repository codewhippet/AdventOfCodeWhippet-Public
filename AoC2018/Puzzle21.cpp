#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2018_Types
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

using namespace Puzzle21_2018_Types;

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

static void Execute(const vector<Instruction>& program, Computer* computer, int64_t runUntil, int64_t* breakAt)
{
	const size_t A = 0;
	const size_t B = 1;
	const size_t C = 2;

	for (int64_t i = 0; i < runUntil; i++)
	{
		if (breakAt && (computer->PC() == *breakAt))
			return;

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

static int64_t FindHighestFirstOccurence(int64_t puzzleConstant)
{
	int64_t r3 = 0;
	int64_t r4 = 0;
	int64_t r5 = 0;

	map<int64_t, size_t> firstOccurences;
	for (int64_t i = 0; i < 16777215; i++)
	{
		r3 = r4 | 65536;
		r4 = puzzleConstant;

		while (true)
		{
			r5 = r3 & 255;
			r4 = r4 + r5;
			r4 = r4 & 16777215;
			r4 = r4 * 65899;
			r4 = r4 & 16777215;

			if (r3 < 256)
				break;

			r5 = r3 / 256;
			r3 = r5;
		}

		firstOccurences.insert({ r4, i, });
	}

	auto highestFirstOccurence = ranges::max(firstOccurences | views::transform([](const auto& kvp) -> pair<size_t, int64_t> { return { kvp.second, kvp.first }; }));
	return highestFirstOccurence.second;
}

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [program, pcRegister] = ParseProgram(input);

	Computer comp;
	comp.PCReg = pcRegister;

	int64_t breakpoint = 28;
	Execute(program, &comp, numeric_limits<int64_t>::max(), &breakpoint);

	int64_t answer = comp.Registers[4];

	{
		// Check answer
		Computer testComp;
		testComp.PCReg = pcRegister;
		testComp.Registers[0] = answer;
		Execute(program, &testComp, numeric_limits<int64_t>::max(), nullptr);
	}

	printf("[2018] Puzzle21_A: %" PRId64 "\n", answer);
}


static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [program, pcRegister] = ParseProgram(input);
	int64_t puzzleConstant = program[7].Args[0];
	int64_t answer = FindHighestFirstOccurence(puzzleConstant);

	printf("[2018] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2018(const string& filename)
{
	//Puzzle21_A(filename);
	Puzzle21_B(filename);
	Puzzle21_A(filename);
}
