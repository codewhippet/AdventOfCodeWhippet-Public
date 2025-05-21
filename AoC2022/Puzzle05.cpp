#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2022_Types
{
}

using namespace Puzzle05_2022_Types;

static vector<string> ReadStacks(const vector<string>& lines)
{
	vector<string> stacks(9ull);
	for (const string& line : lines)
	{
		if (line.size() < 2 || line[1] == '1')
			break;

		for (size_t i = 0; i < 9; i++)
		{
			stacks[i] += line[(i * 4) + 1];
		}
	}

	ranges::for_each(stacks,
		[](string& stack)
		{
			ranges::reverse(stack);
			stack = Trim(stack);
		});

	return stacks;
}

static void Puzzle05_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	vector<string> stacks = ReadStacks(lines);

	const regex movePattern{ R"(move (\d+) from (\d+) to (\d+))" };
	for (const string& line : lines)
	{
		smatch m;
		if (!regex_match(line, m, movePattern))
			continue;

		int64_t moveN = atoll(m[1].str().c_str());
		int64_t from = atoll(m[2].str().c_str()) - 1;
		int64_t to = atoll(m[3].str().c_str()) - 1;

		for (int64_t i = 0; i < moveN; i++)
		{
			char c = stacks[from].back();
			stacks[from] = stacks[from].substr(0, stacks[from].size() - 1);
			stacks[to] += c;
		}
	}

	string answer;
	ranges::copy(stacks | views::transform([](const auto &stack) { return stack.back(); }), back_inserter(answer));

	printf("[2022] Puzzle05_A: %s\n", answer.c_str());
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	vector<string> stacks = ReadStacks(lines);

	const regex movePattern{ R"(move (\d+) from (\d+) to (\d+))" };
	for (const string& line : lines)
	{
		smatch m;
		if (!regex_match(line, m, movePattern))
			continue;

		int64_t moveN = atoll(m[1].str().c_str());
		int64_t from = atoll(m[2].str().c_str()) - 1;
		int64_t to = atoll(m[3].str().c_str()) - 1;

		size_t fromSize = stacks[from].size();
		string cratesToMove = stacks[from].substr(fromSize - moveN);
		stacks[from] = stacks[from].substr(0, fromSize - moveN);
		stacks[to] += cratesToMove;
	}

	string answer;
	ranges::copy(stacks | views::transform([](const auto &stack) { return stack.back(); }), back_inserter(answer));

	printf("[2022] Puzzle05_B: %s\n", answer.c_str());
}

void Puzzle05_2022(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
