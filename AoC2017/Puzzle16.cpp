#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2017_Types
{
}

using namespace Puzzle16_2017_Types;

static void Spin(vector<char>* oldPrograms, size_t spin)
{
	assert(spin <= oldPrograms->size());

	vector<char> newPrograms(oldPrograms->size());
	for (size_t i = 0; i < newPrograms.size(); i++)
	{
		int64_t sourceIndex = (i + oldPrograms->size() - spin) % oldPrograms->size();
		newPrograms[i] = (*oldPrograms)[sourceIndex];
	}
	oldPrograms->swap(newPrograms);
}

static void Exchange(vector<char>* programs, int64_t positionA, int64_t positionB)
{
	swap((*programs)[positionA], (*programs)[positionB]);
}

static void Partner(vector<char>* programs, char programA, char programB)
{
	for (size_t i = 0; i < programs->size(); i++)
	{
		if ((*programs)[i] == programA)
		{
			(*programs)[i] = programB;
		}
		else if ((*programs)[i] == programB)
		{
			(*programs)[i] = programA;
		}
	}
}

static vector<function<void(vector<char>*)>> ParseDance(istream& input)
{
	string instructions = ReadSingleLine(input);

	vector<function<void(vector<char>*)>> dance;
	char* token = strtok(instructions.data(), ",");
	while (token)
	{
		switch (token[0])
		{
		case 's':
			{
				int64_t a = 0;
				sscanf(token, "s%lld", &a);
				dance.push_back({ [a](vector<char>* p) { Spin(p, a); } });
			}
			break;
		case 'x':
			{
				int64_t a = 0;
				int64_t b = 0;
				sscanf(token, "x%lld/%lld", &a, &b);
				dance.push_back({ [a, b](vector<char>* p) { Exchange(p, a, b); } });
			}
			break;
		case 'p':
			{
				char a = 0;
				char b = 0;
				sscanf(token, "p%c/%c", &a, &b);
				dance.push_back({ [a, b](vector<char>* p) { Partner(p, a, b); } });
			}
			break;
		default:
			assert(false);
		}

		token = strtok(nullptr, ",");
	}
	return dance;
}

static vector<char> CreatePrograms(size_t programCount)
{
	vector<char> programs(programCount);
	iota(programs.begin(), programs.end(), 'a');
	return programs;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const vector<function<void(vector<char>*)>> dance = ParseDance(input);

	const size_t numPrograms = 16;
	vector<char> programs = CreatePrograms(numPrograms);

	for (const auto& func : dance)
	{
		func(&programs);
	}

	string answer(programs.data(), programs.size());

	printf("[2017] Puzzle16_A: %s\n", answer.c_str());
}


static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const vector<function<void(vector<char>*)>> dance = ParseDance(input);

	const size_t numPrograms = 16;
	vector<char> programs = CreatePrograms(numPrograms);

	map<vector<char>, size_t> states;

	size_t loopBegin = numeric_limits<size_t>::max();
	size_t loopEnd = numeric_limits<size_t>::max();

	size_t iterations = 0;
	while (true)
	{
		states.insert({ programs, iterations++ });

		for (const auto& func : dance)
		{
			func(&programs);
		}

		if (states.contains(programs))
		{
			loopBegin = states.at(programs);
			loopEnd = iterations;
			break;
		}
	}

	size_t loopSize = loopEnd - loopBegin;

	size_t loopRemainder = (1000000000 - loopBegin) % loopSize;
	size_t iterationsNeeded = loopBegin + loopRemainder;

	vector<char> finalArrangement = ranges::find_if(states, [&](const auto& kvp) { return kvp.second == iterationsNeeded; })->first;

	string answer(finalArrangement.data(), finalArrangement.size());

	printf("[2017] Puzzle16_B: %s\n", answer.c_str());
}

void Puzzle16_2017(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
