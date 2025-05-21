#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2016_Types
{
}

using namespace Puzzle18_2016_Types;

static string NextRow(const string& prevRow)
{
	string next(prevRow.size(), '.');
	for (size_t i = 0; i < next.size(); i++)
	{
		bool leftTrap = (i > 0) && (prevRow[i - 1] == '^');
		//bool centreTrap = prevRow[i] == '^';
		bool rightTrap = (i + 1 < prevRow.size()) && (prevRow[i + 1] == '^');

		bool isTrap = leftTrap ^ rightTrap;
		next[i] = (isTrap ? '^' : '.');
	}

	return next;
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t rowsNeeded = 40;

	string row;
	getline(input, row);

	int64_t answer = 0;
	for (size_t i = 0; i < rowsNeeded; i++)
	{
		answer += ranges::count(row, '.');
		row = NextRow(row);
	}

	printf("[2016] Puzzle18_A: %" PRId64 "\n", answer);
}


static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t rowsNeeded = 400000;

	string row;
	getline(input, row);

	int64_t answer = 0;
	for (size_t i = 0; i < rowsNeeded; i++)
	{
		answer += ranges::count(row, '.');
		row = NextRow(row);
	}

	printf("[2016] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2016(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
