#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2016_Types
{
	enum class Opcode
	{
		CpyImm,
		Cpy,
		Inc,
		Dec,
		JnzImm,
		Jnz,
	};

	struct Instruction
	{
		Opcode Op;
		int64_t A = 0;
		int64_t B = 0;
	};

	struct Computer
	{
		int64_t PC = 0;
		int64_t Registers[4] = { 0 };
	};
}

using namespace Puzzle12_2016_Types;

static vector<Instruction> ParseProgram(istream& input)
{
	vector<Instruction> program;

	auto Register = [](char c) -> int64_t { return c - 'a'; };

	for (const string& line : ReadEachLine(input))
	{
		Instruction inst;
		{
			char paramA[16] = { 0 };
			char paramB[16] = { 0 };

			if (line.starts_with("cpy"sv))
			{
				int scanned = sscanf(line.c_str(), "cpy %s %s",
					paramA,
					paramB);
				assert(scanned == 2);
				(void)scanned;

				if (isdigit(paramA[0]))
				{
					inst.Op = Opcode::CpyImm;
					inst.A = atoi(paramA);
				}
				else
				{
					inst.Op = Opcode::Cpy;
					inst.A = Register(paramA[0]);
				}
				inst.B = Register(paramB[0]);
			}
			else if (line.starts_with("inc"sv))
			{
				int scanned = sscanf(line.c_str(), "inc %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Inc;
				inst.A = Register(paramA[0]);
			}
			else if (line.starts_with("dec"sv))
			{
				int scanned = sscanf(line.c_str(), "dec %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Dec;
				inst.A = Register(paramA[0]);
			}
			else if (line.starts_with("jnz"sv))
			{
				int scanned = sscanf(line.c_str(), "jnz %s %s",
					paramA,
					paramB);
				assert(scanned == 2);
				(void)scanned;

				if (isdigit(paramA[0]))
				{
					inst.Op = Opcode::JnzImm;
					inst.A = atoi(paramA);
				}
				else
				{
					inst.Op = Opcode::Jnz;
					inst.A = Register(paramA[0]);
				}
				inst.B = atoi(paramB);
			}
			else
			{
				assert(false);
			}
		}
		program.push_back(inst);
	}

	return program;
}

static void Execute(const vector<Instruction>& program, Computer* computer)
{
	while ((computer->PC >= 0) && (computer->PC < (int64_t)program.size()))
	{
		switch (program[computer->PC].Op)
		{
		case Opcode::CpyImm:
			computer->Registers[program[computer->PC].B] = program[computer->PC].A;
			break;

		case Opcode::Cpy:
			computer->Registers[program[computer->PC].B] = computer->Registers[program[computer->PC].A];
			break;

		case Opcode::Inc:
			computer->Registers[program[computer->PC].A]++;
			break;

		case Opcode::Dec:
			computer->Registers[program[computer->PC].A]--;
			break;

		case Opcode::JnzImm:
			if (program[computer->PC].A != 0)
			{
				computer->PC += program[computer->PC].B - 1;
			}
			break;

		case Opcode::Jnz:
			if (computer->Registers[program[computer->PC].A] != 0)
			{
				computer->PC += program[computer->PC].B - 1;
			}
			break;

		default:
			assert(false);
			break;
		}

		computer->PC += 1;
	}
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Computer comp;
	Execute(program, &comp);

	int64_t answer = comp.Registers[0];

	printf("[2016] Puzzle12_A: %" PRId64 "\n", answer);
}


static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Computer comp;
	comp.Registers[2] = 1;
	Execute(program, &comp);

	int64_t answer = comp.Registers[0];

	printf("[2016] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2016(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
