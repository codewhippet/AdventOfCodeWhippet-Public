#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2016_Types
{
	enum class Opcode
	{
		Cpy,
		Inc,
		Dec,
		Jnz,
		Out,
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

		auto operator<=>(const Computer& other) const = default;
	};

	struct ComputerHash
	{
		size_t operator()(const Computer &comp) const noexcept
		{
			size_t hash = std::hash< int64_t>{}(comp.PC);
			hash = std::hash<int64_t>{}(comp.Registers[0]) ^ (hash >> 16 | hash << 48);
			hash = std::hash<int64_t>{}(comp.Registers[1]) ^ (hash >> 16 | hash << 48);
			hash = std::hash<int64_t>{}(comp.Registers[2]) ^ (hash >> 16 | hash << 48);
			hash = std::hash<int64_t>{}(comp.Registers[3]) ^ (hash >> 16 | hash << 48);
			return hash;
		}
	};
}

using namespace Puzzle25_2016_Types;

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
			else if (line.starts_with("out"sv))
			{
				int scanned = sscanf(line.c_str(), "out %s",
					paramA);
				assert(scanned == 1);
				(void)scanned;

				inst.Op = Opcode::Out;
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

static bool Execute(const vector<Instruction>& program,
	Computer* computer,
	size_t instructionLimit)
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

	unordered_map<Computer, size_t, ComputerHash> previousStates;
	vector<pair<int64_t, int64_t>> output;

	size_t instructionCount = 0;
	while ((computer->PC >= 0) && (computer->PC < (int64_t)program.size()))
	{
		if (++instructionCount > instructionLimit)
		{
			return false;
		}

		// Infinite loop check
		if (previousStates.contains(*computer))
		{
			// When did we first encounter this state?
			int64_t duplicateOf = previousStates.at(*computer);
			vector<pair<int64_t, int64_t>> outputSinceStartOfLoop;
			ranges::copy(output | views::filter([&](const auto& kvp) { return kvp.first >= duplicateOf; }), back_inserter(outputSinceStartOfLoop));

			// If we're in an infinite loop, and we've output a non-zero even number of tokens since the start of the loop,
			// then we'll continue to output the correct tokens forever
			if ((outputSinceStartOfLoop.size() > 0) && ((outputSinceStartOfLoop.size() % 2) == 0))
			{
				return true;
			}

			return false;
		}

		previousStates.insert({ *computer, instructionCount });

		const Instruction& currentInst = program[computer->PC];
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

		case Opcode::Out:
			{
				int64_t expectedOutput = (output.size() % 2);
				if (ReadParam(currentInst.A) == expectedOutput)
				{
					output.push_back({ instructionCount, expectedOutput });
				}
				else
				{
					return false;
				}
			}
			break;

		default:
			assert(false);
			break;
		}

		computer->PC += 1;
	}

	return false;
}

static void Puzzle25_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	int64_t answer = 0;
	for (size_t i = 0; i < 100000; i++)
	{
		Computer comp;
		comp.Registers[0] = i;
		if (Execute(program, &comp, 1000000))
		{
			answer = i;
			break;
		}
	}

	printf("[2016] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2016(const string& filename)
{
	Puzzle25_A(filename);
}
