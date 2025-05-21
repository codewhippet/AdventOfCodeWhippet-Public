#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2023_Types
{
}

using namespace Puzzle09_2023_Types;

static vector<int64_t> ExtendOne(const vector<int64_t>& sequence)
{
	vector<vector<int64_t>> sequences;
	sequences.push_back(sequence);

	// Work down the ladder of diffs
	for (int64_t level = 1; level < (int64_t)sequence.size(); level++)
	{
		const vector<int64_t>& parent = sequences[level - 1];
		bool allZeros = true;
		vector<int64_t> diffs;
		for (int64_t x = 0; (x + 1) < (int64_t)parent.size(); x++)
		{
			int64_t diff = parent[x + 1] - parent[x];
			allZeros = allZeros && (diff == 0);
			diffs.push_back(diff);
		}

		sequences.emplace_back(move(diffs));
		if (allZeros)
			break;
	}

	// Work back up, extending as you go
	sequences.back().push_back(0);
	for (int64_t i = sequences.size() - 2; i >= 0; i--)
	{
		sequences[i].push_back(sequences[i].back() + sequences[i + 1].back());
	}

	return sequences[0];
}

static vector<int64_t> ExtendMinusOne(const vector<int64_t>& sequence)
{
	vector<vector<int64_t>> sequences;
	sequences.push_back(sequence);

	// Work down the ladder of diffs
	for (int64_t level = 1; level < (int64_t)sequence.size(); level++)
	{
		const vector<int64_t>& parent = sequences[level - 1];
		bool allZeros = true;
		vector<int64_t> diffs;
		for (int64_t x = 0; (x + 1) < (int64_t)parent.size(); x++)
		{
			int64_t diff = parent[x + 1] - parent[x];
			allZeros = allZeros && (diff == 0);
			diffs.push_back(diff);
		}

		sequences.emplace_back(move(diffs));
		if (allZeros)
			break;
	}

	// Work back up, extending as you go
	sequences.back().insert(sequences.back().begin(), 0);
	for (int64_t i = sequences.size() - 2; i >= 0; i--)
	{
		sequences[i].insert(sequences[i].begin(), sequences[i].front() - sequences[i + 1].front());
	}

	return sequences[0];
}

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string line : ReadAllLines(input))
	{
		vector<int64_t> extension = ExtendOne(ReadAsVectorOfNumbers(line));
		answer += extension.back();
	}

	printf("[2023] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string line : ReadAllLines(input))
	{
		vector<int64_t> extension = ExtendMinusOne(ReadAsVectorOfNumbers(line));
		answer += extension.front();
	}

	printf("[2023] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2023(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
