#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2023_Types
{
}

using namespace Puzzle01_2023_Types;

static string ReverseString(string s)
{
	reverse(s.begin(), s.end());
	return move(s);
}

static int FindFirstDigit(string line, map<string, int> digitWords)
{
	while (line.empty() == false)
	{
		if (isdigit(line.front()))
		{
			return line.front() - '0';
		}
		else
		{
			for (map<string, int>::const_reference word : digitWords)
			{
				if (line.substr(0, word.first.length()) == word.first)
				{
					return word.second;
				}
			}
		}

		line = line.substr(1);
	}

	return 0;
}

static void Puzzle01_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> lines = ReadAllLines(input);
	for (const string& line : lines)
	{
		vector<char> digits = MakeEnumerator(line)
			->Where([](char c) { return isdigit(c); })
			->Select<char>([](char c) { return (char)(c - '0'); })
			->ToVector();
		answer += (digits.front() * 10) + digits.back();
	}

	printf("[2023] Puzzle01_A: %" PRId64 "\n", answer);
}

static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, int> digitWordsForward =
	{
		{ "one", 1 },
		{ "two", 2 },
		{ "three", 3 },
		{ "four", 4 },
		{ "five", 5 },
		{ "six", 6 },
		{ "seven", 7 },
		{ "eight", 8 },
		{ "nine", 9 },
	};

	map<string, int> digitWordsReverse = MakeEnumerator(digitWordsForward)
		->Select<pair<string, int>>([](const pair<string, int>& kvp)
			{
				return make_pair(ReverseString(kvp.first), kvp.second);
			})
		->ToMap<string, int>();

	int64_t answer = 0;

	vector<string> lines = ReadAllLines(input);
	for (string line : lines)
	{
		string origLine = line;

		int first = FindFirstDigit(line, digitWordsForward);
		int last = FindFirstDigit(ReverseString(line), digitWordsReverse);

		int value = (first * 10) + last;
		answer += value;
	}

	printf("[2023] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2023(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
