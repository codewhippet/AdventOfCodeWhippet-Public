#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2017_Types
{
	enum class Opcode
	{
		Snd,
		Set,
		Add,
		Mul,
		Mod,
		Rcv,
		Jgz,
	};

	enum class ArgType : uint8_t
	{
		Invalid,
		Immediate,
		Register,
	};

	enum class HaltType : uint8_t
	{
		Finished,
		Waiting,
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
		deque<int64_t> RecvQueue;
		deque<int64_t>* SendTo;
		int64_t SendCount = 0;
	};
}

using namespace Puzzle18_2017_Types;

static pair<ArgType, int64_t> ParseParam(const char* param)
{
	return isalpha(param[0]) ? pair<ArgType, int64_t>{ ArgType::Register, param[0] } : pair<ArgType, int64_t>{ ArgType::Immediate, atoi(param) };
}

static Instruction ParseSingleArgInstruction(Opcode op, const char *format, const string& line)
{
	char paramA[16] = { 0 };

	int scanned = sscanf(line.c_str(), format, paramA);
	assert(scanned == 1);
	(void)scanned;

	return { op, ParseParam(paramA) };
}

static Instruction ParseDoubleArgInstruction(Opcode op, const char *format, const string& line)
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
		{ "snd"sv, [](const string& line) { return ParseSingleArgInstruction(Opcode::Snd, "snd %s", line); } },
		{ "set"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Set, "set %s %s", line); } },
		{ "add"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Add, "add %s %s", line); } },
		{ "mul"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Mul, "mul %s %s", line); } },
		{ "mod"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Mod, "mod %s %s", line); } },
		{ "rcv"sv, [](const string& line) { return ParseSingleArgInstruction(Opcode::Rcv, "rcv %s", line); } },
		{ "jgz"sv, [](const string& line) { return ParseDoubleArgInstruction(Opcode::Jgz, "jgz %s %s", line); } },
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

static HaltType Execute(const vector<Instruction>& program, Computer* computer)
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
		case Opcode::Snd:
			computer->SendTo->push_back(ReadParam(currentInst.A));
			computer->SendCount++;
			break;
		case Opcode::Set:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] = ReadParam(currentInst.B);
			break;
		case Opcode::Add:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] += ReadParam(currentInst.B);
			break;
		case Opcode::Mul:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] *= ReadParam(currentInst.B);
			break;
		case Opcode::Mod:
			assert(currentInst.A.first == ArgType::Register);
			computer->Registers[(char)currentInst.A.second] %= ReadParam(currentInst.B);
			break;
		case Opcode::Rcv:
			assert(currentInst.A.first == ArgType::Register);
			if (computer->RecvQueue.empty())
			{
				return HaltType::Waiting;
			}
			computer->Registers[(char)currentInst.A.second] = computer->RecvQueue.front();
			computer->RecvQueue.pop_front();
			break;
		case Opcode::Jgz:
			if (ReadParam(currentInst.A) > 0)
			{
				computer->PC += ReadParam(currentInst.B) - 1;
			}
			break;
		}
		computer->PC += 1;
	}

	return HaltType::Finished;
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);
	deque<int64_t> output;
	Computer comp;
	comp.SendTo = &output;
	Execute(program, &comp);

	int64_t answer = output.back();

	printf("[2017] Puzzle18_A: %" PRId64 "\n", answer);
}


static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> program = ParseProgram(input);

	vector<Computer> computers(2);
	computers[0].SendTo = &computers[1].RecvQueue;
	computers[0].Registers['p'] = 0;

	computers[1].SendTo = &computers[0].RecvQueue;
	computers[1].Registers['p'] = 1;

	while (true)
	{
		int64_t oldSendCounts[2] = { computers[0].SendCount, computers[1].SendCount };
		HaltType haltTypes[2];

		haltTypes[0] = Execute(program, &computers[0]);
		haltTypes[1] = Execute(program, &computers[1]);

		// Both terminated
		if ((haltTypes[0] == HaltType::Finished) && (haltTypes[1] == HaltType::Finished))
		{
			break;
		}

		// Both waiting and neither sending
		if ((haltTypes[0] == HaltType::Waiting) && (computers[1].SendCount == oldSendCounts[1]) &&
			(haltTypes[1] == HaltType::Waiting) && (computers[0].SendCount == oldSendCounts[0]))
		{
			break;
		}
	}

	int64_t answer = computers[1].SendCount;

	printf("[2017] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2017(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
