#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2018_Types
{
	using Registers = array<int64_t, 4>;
	using Instruction = array<int64_t, 4>;

	struct Sample
	{
		Registers Before;
		Instruction Instruction;
		Registers After;
	};

	struct Puzzle
	{
		vector<Sample> Samples;
		vector<Instruction> Program;
	};

	const int A = 1;
	const int B = 2;
	const int C = 3;

	const auto addr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] + reg[inst[B]];
			return reg;
		};
	const auto addi = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] + inst[B];
			return reg;
		};

	const auto mulr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] * reg[inst[B]];
			return reg;
		};
	const auto muli = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] * inst[B];
			return reg;
		};

	const auto banr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] & reg[inst[B]];
			return reg;
		};
	const auto bani = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] & inst[B];
			return reg;
		};

	const auto borr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] | reg[inst[B]];
			return reg;
		};
	const auto bori = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] | inst[B];
			return reg;
		};

	const auto setr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]];
			return reg;
		};
	const auto seti = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = inst[A];
			return reg;
		};

	const auto gtir = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = inst[A] > reg[inst[B]];
			return reg;
		};
	const auto gtri = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] > inst[B];
			return reg;
		};
	const auto gtrr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] > reg[inst[B]];
			return reg;
		};

	const auto eqir = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = inst[A] == reg[inst[B]];
			return reg;
		};
	const auto eqri = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] == inst[B];
			return reg;
		};
	const auto eqrr = [](const Instruction& inst, Registers reg) -> Registers
		{
			reg[inst[C]] = reg[inst[A]] == reg[inst[B]];
			return reg;
		};
}

using namespace Puzzle16_2018_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);
	size_t lineIndex = 0;
	for (/***/; lineIndex + 2 < lines.size(); lineIndex += 4)
	{
		if (lines[lineIndex].starts_with("Before"sv) == false)
		{
			break;
		}

		Sample s;
		ranges::copy(ReadAsVectorOfNumbers(lines[lineIndex + 0]), s.Before.begin());
		ranges::copy(ReadAsVectorOfNumbers(lines[lineIndex + 1]), s.Instruction.begin());
		ranges::copy(ReadAsVectorOfNumbers(lines[lineIndex + 2]), s.After.begin());
		p.Samples.push_back(s);
	}

	for (/***/; lineIndex < lines.size(); lineIndex++)
	{
		vector<int64_t> numbers = ReadAsVectorOfNumbers(lines[lineIndex]);
		if (numbers.empty() == false)
		{
			Instruction inst;
			ranges::copy(numbers, inst.begin());
			p.Program.push_back(inst);
		}
	}

	return p;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);

	const vector<function<Registers(const Instruction&, Registers)>> instructions =
	{
		addr, addi,
		mulr, muli,
		banr, bani,
		borr, bori,
		setr, seti,
		gtir, gtri, gtrr,
		eqir, eqri, eqrr,
	};

	int64_t answer = 0;
	for (const Sample &sample : puzzle.Samples)
	{
		int matches = 0;
		for (const auto& inst : instructions)
		{
			if (inst(sample.Instruction, sample.Before) == sample.After)
			{
				matches++;
			}
		}
		if (matches >= 3)
		{
			answer++;
		}
	}

	printf("[2018] Puzzle16_A: %" PRId64 "\n", answer);
}


static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);

	const vector<function<Registers(const Instruction&, Registers)>> instructions =
	{
		addr, addi,
		mulr, muli,
		banr, bani,
		borr, bori,
		setr, seti,
		gtir, gtri, gtrr,
		eqir, eqri, eqrr,
	};

	set<int64_t> instructionIndices;
	ranges::copy(ranges::iota_view(0ull, instructions.size()), inserter(instructionIndices, instructionIndices.end()));

	vector<set<int64_t>> possibleOpcodes(instructions.size(), instructionIndices);

	for (const Sample& sample : puzzle.Samples)
	{
		for (size_t i = 0; i < instructions.size(); i++)
		{
			const auto& inst = instructions[i];
			if (inst(sample.Instruction, sample.Before) != sample.After)
			{
				possibleOpcodes[i].erase(sample.Instruction[0]);
			}
		}
	}

	map<int64_t, size_t> opcodeToInstruction;
	while (opcodeToInstruction.size() < instructions.size())
	{
		const auto uniqueOption = ranges::find_if(possibleOpcodes, [](const auto& s) { return s.size() == 1; });
		assert(uniqueOption != possibleOpcodes.end());
		assert(uniqueOption->size() == 1);

		int64_t opcode = *uniqueOption->begin();
		int64_t instruction = uniqueOption - possibleOpcodes.begin();

		opcodeToInstruction[opcode] = instruction;

		for (auto& possibleOpcode : possibleOpcodes)
		{
			possibleOpcode.erase(opcode);
		}
	}

	Registers reg{ 0, 0, 0, 0 };
	for (const Instruction& inst : puzzle.Program)
	{
		int64_t opcode = inst[0];
		int64_t instructionIndex = opcodeToInstruction.at(opcode);
		reg = instructions[instructionIndex](inst, reg);
	}

	int64_t answer = reg[0];

	printf("[2018] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2018(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
