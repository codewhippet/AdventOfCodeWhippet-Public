#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2021_Types
{
}

using namespace Puzzle25_2021_Types;

static int StepEast(const vector<string>& oldState, vector<string>* newState)
{
	vector<string> state;
	state.reserve(oldState.size());
	for (size_t y = 0; y < oldState.size(); y++)
	{
		state.push_back(string(oldState[y].size(), '.'));
	}

	int moves = 0;
	for (size_t y = 0; y < oldState.size(); y++)
	{
		for (size_t x = 0; x < oldState[y].size(); x++)
		{
			if (oldState[y][x] == '>')
			{
				size_t newX = (x + 1) % oldState[y].size();
				if (oldState[y][newX] == '.')
				{
					state[y][newX] = '>';
					moves++;
				}
				else
				{
					state[y][x] = '>';
				}
			}
			else if (oldState[y][x] == 'v')
			{
				state[y][x] = 'v';
			}
		}
	}

	*newState = move(state);
	return moves;
}

static int StepSouth(const vector<string>& oldState, vector<string>* newState)
{
	vector<string> state;
	state.reserve(oldState.size());
	for (size_t y = 0; y < oldState.size(); y++)
	{
		state.push_back(string(oldState[y].size(), '.'));
	}

	int moves = 0;
	for (size_t x = 0; x < oldState[0].size(); x++)
	{
		for (size_t y = 0; y < oldState.size(); y++)
		{
			if (oldState[y][x] == 'v')
			{
				size_t newY = (y + 1) % oldState.size();
				if (oldState[newY][x] == '.')
				{
					state[newY][x] = 'v';
					moves++;
				}
				else
				{
					state[y][x] = 'v';
				}
			}
			else if (oldState[y][x] == '>')
			{
				state[y][x] = '>';
			}
		}
	}

	*newState = move(state);
	return moves;
}

static void Puzzle25_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> state = ReadAllLines(input);

	int steps = 1;
	while (true)
	{
		vector<string> midState;
		vector<string> endState;
		if ((StepEast(state, &midState) + StepSouth(midState, &endState)) == 0)
		{
			break;
		}

		state = move(endState);
		steps++;
	}

	int64_t answer = steps;

	printf("[2021] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2021(const string& filename)
{
	Puzzle25_A(filename);
}
