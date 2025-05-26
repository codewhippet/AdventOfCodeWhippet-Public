#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2018_Types
{
}

using namespace Puzzle05_2018_Types;

static string Reduce(const string& oldPolymer)
{
	string newPolymer;
	newPolymer.reserve(oldPolymer.size());

	for (size_t i = 0; i < oldPolymer.size(); /***/)
	{
		if (((i + 1) < oldPolymer.size()) &&
			(toupper(oldPolymer[i]) == toupper(oldPolymer[i + 1])) &&
			(oldPolymer[i] != oldPolymer[i + 1]))
		{
			i += 2;
		}
		else
		{
			newPolymer += oldPolymer[i];
			i += 1;
		}
	}

	return newPolymer;
}

static string FullyReduce(string polymer)
{
	while (true)
	{
		string reducedPolymer = Reduce(polymer);
		if (reducedPolymer.size() == polymer.size())
		{
			break;
		}
		polymer.swap(reducedPolymer);
	}
	return polymer;
}

static string RemoveUnit(string polymer, char unit)
{
	polymer.erase(remove_if(polymer.begin(), polymer.end(), [u = toupper(unit)](char c) { return toupper(c) == u; }), polymer.end());
	return polymer;
}

static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string polymer = FullyReduce(ReadSingleLine(input));
	int64_t answer = (int64_t)polymer.size();

	printf("[2018] Puzzle05_A: %" PRId64 "\n", answer);
}


static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const string startingPolymer = ReadSingleLine(input);

	int64_t answer = numeric_limits<int64_t>::max();
	for (int i = 0; i < 26; i++)
	{
		answer = min(answer, (int64_t)FullyReduce(RemoveUnit(startingPolymer, (char)('a' + i))).size());
	}

	printf("[2018] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2018(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
