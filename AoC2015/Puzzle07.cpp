#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2015_Types
{
	enum class Operation
	{
		Copy,
		And,
		Or,
		Not,
		LeftShift,
		RightShift,
	};

	struct Gate
	{
		string A;
		string B;
		Operation Op;
		string Out;
	};

	struct Circuit
	{
		map<string, uint16_t> Values;
		map<string, shared_ptr<Gate>> Gates;
	};

}

using namespace Puzzle07_2015_Types;

static map<string, Operation> Operations =
{
	{ "AND", Operation::And },
	{ "OR", Operation::Or },
	{ "NOT", Operation::Not },
	{ "LSHIFT", Operation::LeftShift },
	{ "RSHIFT", Operation::RightShift },
};

static void ParseNot(Circuit* circuit)
{
	const char* assignedFrom = strtok(nullptr, " ");
	const char* assignment = strtok(nullptr, " ");
	const char* assignedTo = strtok(nullptr, " ");

	assert(strcmp(assignment, "->") == 0);
	(void)assignment;

	circuit->Gates[assignedTo] = shared_ptr<Gate>{ new Gate{ assignedFrom, {}, Operation::Not, assignedTo } };
}

static void ParseCopy(const char* firstOperand, Circuit* circuit)
{
	const char* assignedTo = strtok(nullptr, " ");

	circuit->Gates[assignedTo] = shared_ptr<Gate>{ new Gate{ firstOperand, {}, Operation::Copy, assignedTo } };
}

static void ParseLogicGate(const char* firstOperand, const char *operation, Circuit* circuit)
{
	const char* secondOperand = strtok(nullptr, " ");
	const char* assignment = strtok(nullptr, " ");
	const char* assignedTo = strtok(nullptr, " ");

	assert(strcmp(assignment, "->") == 0);
	(void)assignment;

	circuit->Gates[assignedTo] = shared_ptr<Gate>{ new Gate{ firstOperand, secondOperand, Operations.at(operation), assignedTo } };
}

static void ParseAssignment(const char *firstToken, Circuit* circuit)
{
	const char* secondToken = strtok(nullptr, " ");
	if (strcmp(secondToken, "->") == 0)
	{
		ParseCopy(firstToken, circuit);
	}
	else
	{
		ParseLogicGate(firstToken, secondToken, circuit);
	}
}

static void ParseLine(string line, Circuit *circuit)
{
	const char* token = strtok(line.data(), " ");
	
	if (strcmp(token, "NOT") == 0)
	{
		ParseNot(circuit);
	}
	else
	{
		ParseAssignment(token, circuit);
	}
}

static uint16_t Evaluate(const string& wire, Circuit* circuit)
{
	auto existingValue = circuit->Values.find(wire);
	if (existingValue != circuit->Values.end())
	{
		return existingValue->second;
	}

	uint16_t value;
	if (isdigit(wire[0]))
	{
		value = (uint16_t)atoi(wire.c_str());
	}
	else
	{
		shared_ptr<Gate> gate = circuit->Gates.at(wire);
		switch (gate->Op)
		{
		case Operation::Copy:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				value = a;
			}
			break;

		case Operation::And:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				uint16_t b = Evaluate(gate->B, circuit);
				value = a & b;
			}
			break;

		case Operation::Or:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				uint16_t b = Evaluate(gate->B, circuit);
				value = a | b;
			}
			break;

		case Operation::Not:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				value = ~a;
			}
			break;

		case Operation::LeftShift:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				uint16_t b = Evaluate(gate->B, circuit);
				value = a << b;
			}
			break;

		case Operation::RightShift:
			{
				uint16_t a = Evaluate(gate->A, circuit);
				uint16_t b = Evaluate(gate->B, circuit);
				value = a >> b;
			}
			break;
		}
	}

	circuit->Values.insert({ wire, value });
	return value;
}

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Circuit c;
	for (const string& line : ReadEachLine(input))
	{
		ParseLine(line, &c);
	}

	uint16_t answer = Evaluate("a", &c);

	printf("[2015] Puzzle07_A: %u\n", answer);
}


static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Circuit c;
	for (const string& line : ReadEachLine(input))
	{
		ParseLine(line, &c);
	}

	uint16_t a = Evaluate("a", &c);
	c.Values.clear();
	c.Values["b"] = a;
	uint16_t answer = Evaluate("a", &c);

	printf("[2015] Puzzle07_B: %u\n", answer);
}

void Puzzle07_2015(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
