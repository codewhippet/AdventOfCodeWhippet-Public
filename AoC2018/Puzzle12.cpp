#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2018_Types
{
	struct Puzzle
	{
		string InitialState;
		set<string> PositiveStates;
	};
}

using namespace Puzzle12_2018_Types;

static string SingleMatch(const string& source, const regex& pattern)
{
	smatch m;
	regex_match(source, m, pattern);
	return m[1].str();
}

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);

	p.InitialState = SingleMatch(lines[0], regex{ R"(initial state: ([#\.]+))" });

	regex rule{ R"(([#\.]{5}) => #)" };
	for (size_t i = 2; i < lines.size(); i++)
	{
		p.PositiveStates.insert(SingleMatch(lines[i], rule));
	}

	return p;
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t generations = 20;

	Puzzle p = ReadPuzzle(input);

	string pots = string(generations, '.') + p.InitialState + string(generations, '.');

	for (size_t gen = 0; gen < generations; gen++)
	{
		string newPots = "..";
		for (size_t i = 2; i < pots.size(); i++)
		{
			char n = p.PositiveStates.contains(pots.substr(i - 2, 5)) ? '#' : '.';
			newPots += n;
		}
		pots.swap(newPots);
	}

	int64_t answer = 0;
	for (int64_t i = 0; i < (int64_t)pots.size(); i++)
	{
		if (pots[i] == '#')
		{
			int64_t potIndex = i - generations;
			answer += potIndex;
		}
	}

	printf("[2018] Puzzle12_A: %" PRId64 "\n", answer);
}


static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t totalGenerations = 50000000000;
	const size_t startingGenerations = 250;
	const size_t remainingGenerations = totalGenerations - startingGenerations;

	Puzzle p = ReadPuzzle(input);

	string pots = string(startingGenerations, '.') + p.InitialState + string(startingGenerations, '.');

	for (size_t gen = 0; gen < startingGenerations; gen++)
	{
		string newPots = "..";
		for (size_t i = 2; i < pots.size(); i++)
		{
			char n = p.PositiveStates.contains(pots.substr(i - 2, 5)) ? '#' : '.';
			newPots += n;
		}
		pots.swap(newPots);
	}

	int64_t startingScore = 0;
	for (int64_t i = 0; i < (int64_t)pots.size(); i++)
	{
		if (pots[i] == '#')
		{
			int64_t potIndex = i - startingGenerations;
			startingScore += potIndex;
		}
	}

	int64_t stableCount = count(pots.begin(), pots.end(), '#');

	int64_t answer = startingScore + stableCount * remainingGenerations;

	printf("[2018] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2018(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
