#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2021_Types
{
	struct Board
	{
		map<int, pair<int, int>> Unchecked;
		map<int, pair<int, int>> Checked;
	};
}

using namespace Puzzle04_2021_Types;

static bool IsBingoFast(const Board& b)
{
	int rowCounts[5] = { 0 };
	int columnCounts[5] = { 0 };
	for (map<int, pair<int, int>>::const_reference cell : b.Checked)
	{
		rowCounts[cell.second.first]++;
		columnCounts[cell.second.second]++;
	}
	return (max(
		*max_element(&rowCounts[0], &rowCounts[5]),
		*max_element(&columnCounts[0], &columnCounts[5]))) == 5;
}

static bool IsBingo(const Board& b)
{
	return IsBingoFast(b);
}

static bool ReadBoard(istream& input, Board* pBoard)
{
	for (int row = 0; row < 5; row++)
	{
		for (int column = 0; column < 5; column++)
		{
			int value;
			if (!(input >> value))
				return false;

			assert(pBoard->Unchecked.find(value) == pBoard->Unchecked.end());

			pBoard->Unchecked[value] = make_pair(row, column);
		}
	}
	return true;
}

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string randomSequenceSource;
	getline(input, randomSequenceSource);

	vector<Board> boards;

	while (true)
	{
		Board b;
		if (ReadBoard(input, &b) == false)
			break;
		boards.emplace_back(move(b));
	}

	// Run the bingo
	for (char& c : randomSequenceSource)
	{
		if (c == ',')
			c = ' ';
	}
	istringstream randomSequence(randomSequenceSource);

	int64_t answer = 0;
	while (answer == 0)
	{
		int call;
		if (!(randomSequence >> call))
			break;

		for (Board& b : boards)
		{
			map<int, pair<int, int>>::const_iterator found = b.Unchecked.find(call);
			if (found != b.Unchecked.end())
			{
				b.Checked[found->first] = found->second;
				b.Unchecked.erase(found);
			}

			if (IsBingo(b))
			{
				int uncheckedScore = 0;
				for (map<int, pair<int, int>>::const_reference& cell : b.Unchecked)
				{
					uncheckedScore += cell.first;
				}
				answer = uncheckedScore * call;
				break;
			}
		}
	}

	printf("[2021] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string randomSequenceSource;
	getline(input, randomSequenceSource);

	vector<Board> boards;

	while (true)
	{
		Board b;
		if (ReadBoard(input, &b) == false)
			break;
		boards.emplace_back(move(b));
	}

	// Run the bingo
	for (char& c : randomSequenceSource)
	{
		if (c == ',')
			c = ' ';
	}
	istringstream randomSequence(randomSequenceSource);

	vector<pair<int, int>> winningOrder;
	while (winningOrder.size() < boards.size())
	{
		int call;
		if (!(randomSequence >> call))
			break;

		for (int i = 0; i < boards.size(); i++)
		{
			if (find_if(winningOrder.begin(), winningOrder.end(), [i](const pair<int, int>& pair) { return i == pair.first; }) != winningOrder.end())
				continue;

			Board& b = boards[i];

			map<int, pair<int, int>>::const_iterator found = b.Unchecked.find(call);
			if (found != b.Unchecked.end())
			{
				b.Checked[found->first] = found->second;
				b.Unchecked.erase(found);
			}

			if (IsBingo(b))
			{
				winningOrder.push_back(make_pair(i, call));
			}
		}
	}

	int lastBoard = winningOrder.rbegin()->first;
	int lastCall = winningOrder.rbegin()->second;

	int uncheckedScore = 0;
	for (map<int, pair<int, int>>::const_reference& cell : boards[lastBoard].Unchecked)
	{
		uncheckedScore += cell.first;
	}

	int64_t answer = uncheckedScore * lastCall;

	printf("[2021] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2021(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
