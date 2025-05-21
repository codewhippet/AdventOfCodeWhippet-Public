#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2021_Types
{
	static map<char, char> OpeningFor =
	{
		{')', '('},
		{']', '['},
		{'}', '{'},
		{'>', '<'},
	};

	static map<char, char> ClosingFor =
	{
		{'(', ')'},
		{'[', ']'},
		{'{', '}'},
		{'<', '>'},
	};

	static map<char, int> PointsForCorruption =
	{
		{')', 3},
		{']', 57},
		{'}', 1197},
		{'>', 25137},
	};

	static map<char, int64_t> PointsForClosing =
	{
		{')', 1},
		{']', 2},
		{'}', 3},
		{'>', 4},
	};
}

using namespace Puzzle10_2021_Types;

static bool IsOpen(char c)
{
	return
		c == '(' ||
		c == '[' ||
		c == '{' ||
		c == '<';
}

static bool IsClose(char c)
{
	return
		c == ')' ||
		c == ']' ||
		c == '}' ||
		c == '>';
}

static char FirstCorrupt(const string& line)
{
	stack<char> openSet;

	for (char c : line)
	{
		if (IsOpen(c))
		{
			openSet.push(c);
			continue;
		}

		assert(IsClose(c));
		(void)&IsClose;

		if (openSet.empty())
		{
			return c;
		}

		if (openSet.top() != OpeningFor[c])
		{
			return c;
		}

		openSet.pop();
	}

	return '\0';
}

static string GetClosingChars(const string& line)
{
	stack<char> openSet;

	for (char c : line)
	{
		if (IsOpen(c))
		{
			openSet.push(c);
			continue;
		}

		assert(IsClose(c));

		if (openSet.empty())
		{
			return "";
		}

		if (openSet.top() != OpeningFor[c])
		{
			return "";
		}

		openSet.pop();
	}

	string closingChars;

	while (openSet.empty() == false)
	{
		closingChars += ClosingFor[openSet.top()];
		openSet.pop();
	}

	return closingChars;
}

static int64_t ClosingScore(const string& line)
{
	int64_t score = 0;
	for (char c : line)
	{
		score = score * 5 + PointsForClosing[c];
	}
	return score;
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> lines = ReadAllLines(input);

	answer = MakeEnumerator(lines)
		->Select<char>([](const string& s) { return FirstCorrupt(s); })
		->Where([](const char& c) { return c != '\0'; })
		->Select<int>([](const char& c) { return PointsForCorruption[c]; })
		->Sum();

	printf("[2021] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> lines = ReadAllLines(input);

	vector<int64_t> closingLines = MakeEnumerator(lines)
		->Select<string>([](const string& s) { return GetClosingChars(s); })
		->Where([](const string& s) { return s.length() > 0; })
		->Select<int64_t>([](const string& s) { return ClosingScore(s); })
		->ToVector();

	sort(closingLines.begin(), closingLines.end());

	answer = (int)closingLines[(closingLines.size() / 2)];

	printf("[2021] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2021(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
