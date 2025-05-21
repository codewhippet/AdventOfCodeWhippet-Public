#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2020_Types
{
}

using namespace Puzzle09_2020_Types;

static vector<int64_t> ReadPuzzle(istream& input)
{
	return MakeEnumerator(ReadAllLines(input))
		->Select<int64_t>([](const string& line) { return stoll(line); })
		->ToVector();
}

static int64_t FirstEncodingError(const vector<int64_t>& puzzle)
{
	const size_t windowSize = 25;

	deque<int64_t> window;
	set<int64_t> previous;

	for (size_t i = 0; i < puzzle.size(); i++)
	{
		if (window.size() == windowSize)
		{
			assert(previous.size() == windowSize);

			bool isSum = false;
			for (int64_t a : window)
			{
				int64_t b = puzzle[i] - a;
				if (previous.contains(b))
				{
					isSum = true;
					break;
				}
			}
			if (isSum == false)
			{
				return puzzle[i];
			}

			previous.erase(window.front());
			window.pop_front();
		}

		window.push_back(puzzle[i]);
		previous.insert(puzzle[i]);
	}

	return -1;
}

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> puzzle = ReadPuzzle(input);

	int64_t answer = FirstEncodingError(puzzle);

	printf("[2020] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> puzzle = ReadPuzzle(input);

	const int64_t kTARGET = FirstEncodingError(puzzle);

	size_t i = 0;
	size_t j = 0;
	int64_t runningSum = 0;

	while (true)
	{
		assert(i <= j);
		assert(i < puzzle.size());
		assert(j < puzzle.size());

		if (runningSum == kTARGET)
		{
			break;
		}

		if (runningSum < kTARGET)
		{
			runningSum += puzzle[j++];
		}
		else
		{
			assert(runningSum > kTARGET);
			runningSum -= puzzle[i++];
		}
	}

	int64_t smallest = numeric_limits<int64_t>::max();
	int64_t largest = numeric_limits<int64_t>::min();

	for (/***/; i < j; i++)
	{
		smallest = min(smallest, puzzle[i]);
		largest = max(largest, puzzle[i]);
	}

	int64_t answer = smallest + largest;

	printf("[2020] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2020(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
