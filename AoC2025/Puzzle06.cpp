#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2025_Types
{
}

using namespace Puzzle06_2025_Types;

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<vector<int64_t>> numbers;
	vector<char> operations;

	string line;
	while (getline(input, line))
	{
		if ((line[0] == '+') || (line[0] == '*'))
		{
			operations = line | views::filter([](char c) { return c != ' '; }) | ranges::to<vector>();
		}
		else
		{
			numbers.push_back(ReadAsVectorOfNumbers(line));
		}
	}

	int64_t answer = 0;
	for (size_t i = 0; i < operations.size(); i++)
	{
		int64_t intermediate;
		if (operations[i] == '*')
		{
			intermediate = 1;
			for (const auto& n : numbers)
			{
				intermediate *= n[i];
			}
		}
		else
		{
			intermediate = 0;
			for (const auto& n : numbers)
			{
				intermediate += n[i];
			}
		}
		answer += intermediate;
	}

	printf("[2025] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	const size_t lastLine = lines.size() - 1;

	int64_t answer = 0;
	vector<int64_t> workingSet;
	for (int64_t column = lines.front().size() - 1; column > -1; column--)
	{
		int64_t accumulator = 0;
		for (size_t line = 0; line < lines.size(); line++)
		{
			char c = lines[line][column];
			if (line == lastLine)
			{
				workingSet.push_back(accumulator);
			}

			if (isdigit(c))
			{
				accumulator = (accumulator * 10) + (c - '0');
			}

			if (c == '+')
			{
				answer += ranges::fold_left(workingSet, 0, plus{});
				workingSet.clear();
				column--;
			}
			else if (c == '*')
			{
				answer += ranges::fold_left(workingSet, 1, multiplies{});
				workingSet.clear();
				column--;
			}
		}
	}

	printf("[2025] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2025(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
