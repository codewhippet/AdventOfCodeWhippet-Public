#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2020_Types
{
	struct GameState
	{
		deque<int> PlayerHands[2];
	};

	bool operator<(const GameState& a, const GameState& b)
	{
		if (a.PlayerHands[0] != b.PlayerHands[0]) return a.PlayerHands[0] < b.PlayerHands[0];
		return a.PlayerHands[1] < b.PlayerHands[1];
	}
}

using namespace Puzzle22_2020_Types;

static GameState ReadStartingState(istream& input)
{
	GameState state;

	int handToRead = -1;
	for (const string& line : ReadAllLines(input))
	{
		if (line.substr(0, sizeof("Player") - 1) == "Player")
		{
			handToRead++;
			continue;
		}

		int card;
		int cardsScanned = sscanf(line.c_str(), "%d", &card);
		if (cardsScanned == 1)
		{
			state.PlayerHands[handToRead].push_back(card);
		}
	}

	return state;
}

static GameState CopyStateForRecursion(const GameState& oldState, int player1Card, int player2Card)
{
	GameState newState;

	newState.PlayerHands[0].insert(newState.PlayerHands[0].end(), oldState.PlayerHands[0].begin(), oldState.PlayerHands[0].begin() + player1Card);
	newState.PlayerHands[1].insert(newState.PlayerHands[1].end(), oldState.PlayerHands[1].begin(), oldState.PlayerHands[1].begin() + player2Card);

	return newState;
}

static int PlayGame(GameState* state, map<GameState, int> *globalCache)
{
	map<GameState, int>::const_iterator cachedResult = globalCache->find(*state);
	if (cachedResult != globalCache->end())
	{
		return cachedResult->second;
	}

	GameState startingState = *state;

	set<GameState> seenStates;

	int winner = -1;
	while (true)
	{
		if (state->PlayerHands[0].empty())
		{
			winner = 1;
			break;
		}
		if (state->PlayerHands[1].empty())
		{
			winner = 0;
			break;
		}

		// Stop infinite games
		if (seenStates.contains(*state))
		{
			winner = 0;
			break;
		}

		seenStates.insert(*state);

		// Draw
		int playerCards[2];
		playerCards[0] = state->PlayerHands[0].front();
		playerCards[1] = state->PlayerHands[1].front();
		assert(playerCards[0] != playerCards[1]);

		state->PlayerHands[0].pop_front();
		state->PlayerHands[1].pop_front();

		int winningPlayer = -1;

		// Should we recurse?
		if ((state->PlayerHands[0].size() >= playerCards[0]) &&
			(state->PlayerHands[1].size() >= playerCards[1]))
		{
			GameState newState = CopyStateForRecursion(*state, playerCards[0], playerCards[1]);
			winningPlayer = PlayGame(&newState, globalCache);
		}
		else
		{
			winningPlayer = (playerCards[0] > playerCards[1] ? 0 : 1);
		}

		state->PlayerHands[winningPlayer].push_back(playerCards[winningPlayer]);
		state->PlayerHands[winningPlayer].push_back(playerCards[1 - winningPlayer]);
	}

	globalCache->insert(make_pair(startingState, winner));
	return winner;
}

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	GameState game = ReadStartingState(input);

	while ((game.PlayerHands[0].empty() == false) &&
		(game.PlayerHands[1].empty() == false))
	{
		int player1Card = game.PlayerHands[0].front();
		int player2Card = game.PlayerHands[1].front();
		assert(player1Card != player2Card);

		if (player1Card > player2Card)
		{
			game.PlayerHands[0].push_back(player1Card);
			game.PlayerHands[0].push_back(player2Card);
		}
		else
		{
			game.PlayerHands[1].push_back(player2Card);
			game.PlayerHands[1].push_back(player1Card);
		}

		game.PlayerHands[0].pop_front();
		game.PlayerHands[1].pop_front();
	}

	int winnerIndex = (game.PlayerHands[0].empty() ? 1 : 0);
	size_t numCards = game.PlayerHands[winnerIndex].size();

	int64_t answer = 0;
	for (int card : game.PlayerHands[winnerIndex])
	{
		answer += card * numCards;
		numCards--;
	}

	printf("[2020] Puzzle22_A: %" PRId64 "\n", answer);
}

static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	GameState game = ReadStartingState(input);

	map<GameState, int> globalCache;
	int winner = PlayGame(&game, &globalCache);

	size_t numCards = game.PlayerHands[winner].size();

	int64_t answer = 0;
	for (int card : game.PlayerHands[winner])
	{
		answer += card * numCards;
		numCards--;
	}

	printf("[2020] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2020(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
