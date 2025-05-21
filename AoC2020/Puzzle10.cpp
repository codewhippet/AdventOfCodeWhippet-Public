#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2020_Types
{
}

using namespace Puzzle10_2020_Types;

static vector<int64_t> ReadPuzzle(istream& input)
{
	return MakeEnumerator(ReadAllLines(input))
		->Select<int64_t>([](const string& line) { return stoll(line); })
		->ToVector();
}

static int64_t CountArrangements(const vector<int64_t>& puzzle, size_t pos, map<size_t, int64_t>& cache)
{
	assert(pos < puzzle.size());

	map<size_t, int64_t>::const_iterator cacheHit = cache.find(pos);
	if (cacheHit != cache.end())
	{
		return cacheHit->second;
	}

	int64_t arrangements = 0;

	if (pos == puzzle.size() - 1)
	{
		arrangements = 1;
	}
	else
	{
		// Look for the next valid adapter
		for (int64_t offset = 1; pos + offset < puzzle.size(); offset++)
		{
			int64_t diff = puzzle[pos + offset] - puzzle[pos];
			if (diff > 3)
			{
				break;
			}

			arrangements += CountArrangements(puzzle, pos + offset, cache);
		}
	}

	cache[pos] = arrangements;
	return arrangements;
}

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> puzzle = ReadPuzzle(input);
	puzzle.push_back(0);
	sort(puzzle.begin(), puzzle.end());
	puzzle.push_back(puzzle.back() + 3);

	int64_t singleDiffs = 0;
	int64_t tripleDiffs = 0;
	for (size_t i = 0; i + 1 < puzzle.size(); i++)
	{
		int64_t diff = puzzle[i + 1] - puzzle[i];
		if (diff == 1)
		{
			singleDiffs++;
		}
		else if (diff == 3)
		{
			tripleDiffs++;
		}
	}

	int64_t answer = singleDiffs * tripleDiffs;

	printf("[2020] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> puzzle = ReadPuzzle(input);
	puzzle.push_back(0);
	sort(puzzle.begin(), puzzle.end());
	puzzle.push_back(puzzle.back() + 3);

	map<size_t, int64_t> cache;

	int64_t answer = CountArrangements(puzzle, 0, cache);

	printf("[2020] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2020(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
