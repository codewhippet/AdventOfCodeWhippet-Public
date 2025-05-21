#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2015_Types
{
}

using namespace Puzzle10_2015_Types;

static string LookAndSay(const string& s)
{
	string newS;

	char lastChar = s[0];
	int64_t lastCharCount = 0;
	for (char c : s)
	{
		if (c != lastChar)
		{
			newS += to_string(lastCharCount);
			newS += lastChar;
			lastCharCount = 1;
		}
		else
		{
			lastCharCount++;
		}

		lastChar = c;
	}

	newS += to_string(lastCharCount);
	newS += lastChar;

	return newS;
}

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string s;
	getline(input, s);

	for (int i = 0; i < 40; i++)
	{
		s = LookAndSay(s);
	}

	int64_t answer = s.size();

	printf("[2015] Puzzle10_A: %" PRId64 "\n", answer);
}


static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string s;
	getline(input, s);

	for (int i = 0; i < 50; i++)
	{
		s = LookAndSay(s);
	}

	int64_t answer = s.size();

	printf("[2015] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2015(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
