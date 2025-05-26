#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2016_Types
{
	enum class Opcode
	{
		Cpy,
		Inc,
		Dec,
		Jnz,
		Tgl,
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
		int64_t Registers[4] = { 0 };
	};
}

using namespace Puzzle23_2016_Types;

static pair<ArgType, int64_t> ParseParam(const char* param)
{
	return isalpha(param[0]) ? pair<ArgType, int64_t>{ ArgType::Register, param[0] - 'a' } : pair<ArgType, int64_t>{ ArgType::Immediate, atoi(param) };
}

static vector<Instruction> ParseProgram(istream& input)
{
	vector<Instruction> program;

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

				inst.Op = Opcode::Cpy;
				inst.A = ParseParam(paramA);
				inst.B = ParseParam(paramB);
			}
			else if (line.starts_with("inc"sv))
			{
				int scanned = sscanf(line.c_str(), "inc %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Inc;
				inst.A = ParseParam(paramA);
			}
			else if (line.starts_with("dec"sv))
			{
				int scanned = sscanf(line.c_str(), "dec %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Dec;
				inst.A = ParseParam(paramA);
			}
			else if (line.starts_with("jnz"sv))
			{
				int scanned = sscanf(line.c_str(), "jnz %s %s",
					paramA,
					paramB);
				assert(scanned == 2);
				(void)scanned;

				inst.Op = Opcode::Jnz;
				inst.A = ParseParam(paramA);
				inst.B = ParseParam(paramB);

			}
			else if (line.starts_with("tgl"sv))
			{
				int scanned = sscanf(line.c_str(), "tgl %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Tgl;
				inst.A = ParseParam(paramA);
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

static void Execute(vector<Instruction>* program, Computer* computer)
{
	auto ReadParam = [&](const pair<ArgType, int64_t>& param) -> int64_t
		{
			if (param.first == ArgType::Immediate)
			{
				return param.second;
			}

			if (param.first == ArgType::Register)
			{
				assert((param.second >= 0) && (param.second < 4));
				return computer->Registers[param.second];
			}

			assert(false);
			return 0;
		};

	const map<Opcode, Opcode> toggleOp =
	{
		{ Opcode::Cpy, Opcode::Jnz },
		{ Opcode::Inc, Opcode::Dec },
		{ Opcode::Dec, Opcode::Inc },
		{ Opcode::Jnz, Opcode::Cpy },
		{ Opcode::Tgl, Opcode::Inc },
	};

	while ((computer->PC >= 0) && (computer->PC < (int64_t)program->size()))
	{
		const Instruction& currentInst = (*program)[computer->PC];
		switch (currentInst.Op)
		{
		case Opcode::Cpy:
			if (currentInst.B.first == ArgType::Register)
			{
				computer->Registers[currentInst.B.second] = ReadParam(currentInst.A);
			}
			break;

		case Opcode::Inc:
			if (currentInst.A.first == ArgType::Register)
			{
				computer->Registers[currentInst.A.second]++;
			}
			break;

		case Opcode::Dec:
			if (currentInst.A.first == ArgType::Register)
			{
				computer->Registers[currentInst.A.second]--;
			}
			break;

		case Opcode::Jnz:
			if (ReadParam(currentInst.A) != 0)
			{
				int64_t bVal = ReadParam(currentInst.B);
				computer->PC += bVal - 1;
			}
			break;

		case Opcode::Tgl:
			{
				int64_t target = computer->PC + ReadParam(currentInst.A);
				if (target >= 0 && target < (int64_t)program->size())
				{
					(*program)[target].Op = toggleOp.at((*program)[target].Op);
				}
			}
			break;

		default:
			assert(false);
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
	comp.Registers[0] = 7;
	Execute(&program, &comp);

	int64_t answer = comp.Registers[0];

	printf("[2016] Puzzle23_A: %" PRId64 "\n", answer);
}


static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	Computer comp;
	comp.Registers[0] = 12;
	Execute(&program, &comp);

	int64_t answer = comp.Registers[0];

	printf("[2016] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2016(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
