#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2024_Types
{
	struct Puzzle
	{
		vector<vector<int>> Keys;
		vector<vector<int>> Locks;
	};
}

using namespace Puzzle25_2024_Types;

static Puzzle ParsePuzzle(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);

	for (size_t line = 0; line + 6 < lines.size(); line += 8)
	{
		assert(lines[line].size() == 5);

		if (lines[line] == "#####")
		{
			vector<int> lock;
			for (size_t pin = 0; pin < 5; pin++)
			{
				int height = 0;
				while (height < 6)
				{
					if (lines[line + height + 1][pin] == '.')
					{
						break;
					}
					height++;
				}
				lock.push_back(height);
			}
			p.Locks.push_back(lock);
		}
		else if (lines[line] == ".....")
		{
			vector<int> key;
			for (size_t pin = 0; pin < 5; pin++)
			{
				int height = 0;
				while (height < 6)
				{
					if (lines[line + 5 - height][pin] == '.')
					{
						break;
					}
					height++;
				}
				key.push_back(height);
			}
			p.Keys.push_back(key);
		}
		else
		{
			assert(false);
		}
	}

	return p;
}

static void Puzzle25_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	int64_t answer = 0;
	for (const auto& key : p.Keys)
	{
		for (const auto& lock : p.Locks)
		{
			bool overlap = false;
			for (size_t pin = 0; pin < 5; pin++)
			{
				if ((key[pin] + lock[pin]) > 5)
				{
					overlap = true;
				}
			}
			answer += overlap ? 0 : 1;
		}
	}

	printf("[2024] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2024(const string& filename)
{
	Puzzle25_A(filename);
}
