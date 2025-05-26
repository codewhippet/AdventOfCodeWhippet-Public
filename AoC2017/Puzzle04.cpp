#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2017_Types
{
}

using namespace Puzzle04_2017_Types;

static bool IsValid(const string& passphrase)
{
	istringstream input{ passphrase };
	set<string> words;
	while (input)
	{
		string w;
		if (input >> w)
		{
			if (words.contains(w))
			{
				return false;
			}
			words.insert(w);
		}
	}
	return true;
}

static bool IsValidPartTwo(const string& passphrase)
{
	istringstream input{ passphrase };
	set<string> words;
	while (input)
	{
		string w;
		if (input >> w)
		{
			ranges::sort(w);
			if (words.contains(w))
			{
				return false;
			}
			words.insert(w);
		}
	}
	return true;
}

static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		if (IsValid(line))
		{
			answer++;
		}
	}

	printf("[2017] Puzzle04_A: %" PRId64 "\n", answer);
}


static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		if (IsValidPartTwo(line))
		{
			answer++;
		}
	}

	printf("[2017] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2017(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
