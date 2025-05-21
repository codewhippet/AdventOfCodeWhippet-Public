#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2023_Types
{
	struct Puzzle
	{
		string Instructions;
		map<string, map<char, string>> Nodes;
	};
}

using namespace Puzzle08_2023_Types;

static Puzzle ParsePuzzle(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);

	p.Instructions = lines[0];

	const regex nodePattern{ R"((\w+) = \((\w+), (\w+)\))" };
	for (size_t i = 2; i < lines.size(); i++)
	{
		smatch m;
		if (regex_match(lines[i], m, nodePattern))
		{
			p.Nodes[m[1].str()]['L'] = m[2].str();
			p.Nodes[m[1].str()]['R'] = m[3].str();
		}
	}

	return p;
}

static void Puzzle08_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	string state = "AAA";
	size_t inst = 0;

	int64_t answer = 0;
	while (true)
	{
		if (state == "ZZZ")
			break;

		state = p.Nodes[state][p.Instructions[inst]];
		inst = (inst + 1) % p.Instructions.size();

		answer++;
	}

	printf("[2023] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	vector<string> states = MakeEnumerator(p.Nodes)
		->Select<string>([](map<string, map<char, string>>::const_reference& node) { return node.first; })
		->Where([](const string& node) { return node[2] == 'A'; })
		->ToVector();

	// Initial check of frequencies
	vector<int64_t> cycleSizes;
	for (size_t i = 0; i < states.size(); i++)
	{
		string state = states[i];
		size_t inst = 0;
		int64_t cycles = 0;

		vector<int64_t> finishStates;
		while (finishStates.size() < 3)
		{
			if (state[2] == 'Z')
			{
				finishStates.push_back(cycles);
			}

			cycles++;

			state = p.Nodes[state][p.Instructions[inst]];
			inst = (inst + 1) % p.Instructions.size();
		}

		assert((finishStates[2] - finishStates[1]) == (finishStates[1] - finishStates[0]));
		cycleSizes.push_back(finishStates[2] - finishStates[1]);
	}

	int64_t answer = cycleSizes[0];
	for (size_t i = 1; i < cycleSizes.size(); i++)
	{
		int64_t gcd = std::gcd(answer, cycleSizes[i]);
		answer = (answer * cycleSizes[i]) / gcd;
	}

	printf("[2023] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2023(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
