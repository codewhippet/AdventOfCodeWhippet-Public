#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2017_Types
{
	struct Instruction
	{
		int Value;
		int Move;
		char NextState;
	};

	struct Puzzle
	{
		char StartingState;
		int64_t StepsToExecute;
		map<pair<char, int>, Instruction> StateMachine;
	};
}

using namespace Puzzle25_2017_Types;

template <typename T>
static T Scan(istream& input, const char* format)
{
	string line;
	getline(input, line);

	T value;
	int scanned = sscanf(line.c_str(), format, &value);
	assert(scanned == 1);
	(void)scanned;

	return value;
}

static void ParseHeader(istream& input, Puzzle* p)
{
	p->StartingState = Scan<char>(input, "Begin in state %c.");
	p->StepsToExecute = Scan<int64_t>(input, "Perform a diagnostic checksum after %lld steps.");
}

static void ParseState(istream& input, Puzzle* p)
{
	struct Direction { char Name[16] = { 0 }; };

	char state = Scan<char>(input, "In state %c:");
	for (int i = 0; i < 2; i++)
	{
		int currentValue = Scan<int>(input, "  If the current value is %d:");
		int writeValue = Scan<int>(input, "    - Write the value %d.");
		Direction move = Scan<Direction>(input, "    - Move one slot to the %[a-z].");
		char nextState = Scan<char>(input, "    - Continue with state %c.");

		int moveDirection = move.Name == "right"sv ? 1 : -1;

		p->StateMachine[{ state, currentValue }] = Instruction{ writeValue, moveDirection, nextState };
	}
}

static void ParsePuzzle(istream& input, Puzzle* p)
{
	string line;

	ParseHeader(input, p);
	getline(input, line);

	while (input)
	{
		ParseState(input, p);
		getline(input, line);
	}
}

static void Puzzle25_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p;
	ParsePuzzle(input, &p);

	set<int64_t> tape;
	int64_t head = 0;
	char currentState = p.StartingState;
	for (int64_t i = 0; i < p.StepsToExecute; i++)
	{
		int currentValue = tape.contains(head) ? 1 : 0;
		const Instruction& inst = p.StateMachine.at({ currentState, currentValue });
		if (inst.Value)
		{
			tape.insert(head);
		}
		else
		{
			tape.erase(head);
		}
		head += inst.Move;
		currentState = inst.NextState;
	}

	int64_t answer = (int64_t)tape.size();

	printf("[2017] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2017(const string& filename)
{
	Puzzle25_A(filename);
}
