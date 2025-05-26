#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2015_Types
{
	enum class Opcode
	{
		Hlf,
		Tpl,
		Inc,
		Jmp,
		Jie,
		Jio
	};

	struct Instruction
	{
		Opcode Op;
		int64_t Reg;
		int64_t Immediate;
	};

	struct Computer
	{
		int64_t PC = 0;
		int64_t Registers[2] = { 0 };
	};
}

using namespace Puzzle23_2015_Types;

static vector<Instruction> ParseProgram(istream& input)
{
	vector<Instruction> program;

	const map<string, Opcode> opcodes =
	{
		{ "hlf", Opcode::Hlf },
		{ "tpl", Opcode::Tpl },
		{ "inc", Opcode::Inc },
		{ "jmp", Opcode::Jmp },
		{ "jie", Opcode::Jie },
		{ "jio", Opcode::Jio }
	};

	for (const string& line : ReadEachLine(input))
	{
		Instruction inst;
		{
			char instAsm[64] = { 0 };
			char reg = 'a';
			char immediate[64] = { 0 };

			if (line.find(',') != string::npos)
			{
				int scanned = sscanf(line.c_str(), "%s %c, %s",
					instAsm,
					&reg,
					immediate);
				assert(scanned == 3);
				(void)scanned;
			}
			else if (line.starts_with("jmp"))
			{
				int scanned = sscanf(line.c_str(), "%s %s",
					instAsm,
					immediate);
				assert(scanned == 2);
				(void)scanned;
			}
			else
			{
				int scanned = sscanf(line.c_str(), "%s %c",
					instAsm,
					&reg);
				assert(scanned == 2);
				(void)scanned;
			}

			inst.Op = opcodes.at(instAsm);
			inst.Reg = reg - 'a';
			inst.Immediate = atoll(immediate);
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
		case Opcode::Hlf:
			computer->Registers[program[computer->PC].Reg] /= 2;
			break;

		case Opcode::Tpl:
			computer->Registers[program[computer->PC].Reg] *= 3;
			break;

		case Opcode::Inc:
			computer->Registers[program[computer->PC].Reg] += 1;
			break;

		case Opcode::Jmp:
			computer->PC += program[computer->PC].Immediate - 1;
			break;

		case Opcode::Jie:
			if ((computer->Registers[program[computer->PC].Reg] % 2) == 0)
			{
				computer->PC += program[computer->PC].Immediate - 1;
			}
			break;

		case Opcode::Jio:
			if (computer->Registers[program[computer->PC].Reg] == 1)
			{
				computer->PC += program[computer->PC].Immediate - 1;
			}
			break;
		}

		computer->PC += 1;
	}
}

static void Puzzle23_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Computer comp;
	Execute(program, &comp);

	int64_t answer = comp.Registers[1];

	printf("[2015] Puzzle23_A: %" PRId64 "\n", answer);
}


static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Computer comp;
	comp.Registers[0] = 1;
	Execute(program, &comp);

	int64_t answer = comp.Registers[1];

	printf("[2015] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2015(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
