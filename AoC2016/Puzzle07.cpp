#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2016_Types
{
}

using namespace Puzzle07_2016_Types;

static vector<string> SplitIntoSequences(const string& s, const string &separator)
{
	vector<string> sequences{ "", "" };
	size_t index = 0;
	for (char c : s)
	{
		switch (c)
		{
		case '[':
		case ']':
			sequences[index] += separator;
			index = 1 - index;
			break;
		default:
			sequences[index] += c;
			break;
		}
	}
	return sequences;
}

static bool ContainsAbba(const string& s)
{
	for (size_t i = 0; i + 3 < s.size(); i++)
	{
		if ((s[i + 0] == s[i + 3]) &&
			(s[i + 1] == s[i + 2]) &&
			(s[i + 0] != s[i + 1]))
		{
			return true;
		}
	}
	return false;
}

static int64_t FindNextAba(int64_t lastAba, const string& s)
{
	for (int64_t i = lastAba + 1; i + 2 < (int64_t)s.size(); i++)
	{
		if ((s[i + 0] == s[i + 2]) &&
			(s[i + 0] != s[i + 1]))
		{
			return i;
		}
	}
	return -1;
}

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		vector<string> sequences = SplitIntoSequences(line, ".");
		if (ContainsAbba(sequences[0]) && !ContainsAbba(sequences[1]))
		{
			answer++;
		}
	}

	printf("[2016] Puzzle07_A: %" PRId64 "\n", answer);
}


static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadEachLine(input))
	{
		vector<string> sequences = SplitIntoSequences(line, "..");
		for (int64_t abaIndex = FindNextAba(-1, sequences[0]); abaIndex != -1; abaIndex = FindNextAba(abaIndex, sequences[0]))
		{
			string bab{ sequences[0][abaIndex + 1], sequences[0][abaIndex + 0], sequences[0][abaIndex + 1] };
			if (sequences[1].find(bab) != string::npos)
			{
				answer++;
				break;
			}
		}
	}

	printf("[2016] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2016(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
