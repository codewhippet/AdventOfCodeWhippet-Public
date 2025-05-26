#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2021_Types
{
	struct GameState
	{
		int CurrentPlayer = 0;
		int Positions[2] = { 0, 0 };
		int Scores[2] = { 0, 0 };
	};

	bool operator<(const GameState& a, const GameState& b)
	{
		if (a.CurrentPlayer != b.CurrentPlayer) return a.CurrentPlayer < b.CurrentPlayer;
		if (a.Positions[0] != b.Positions[0]) return a.Positions[0] < b.Positions[0];
		if (a.Positions[1] != b.Positions[1]) return a.Positions[1] < b.Positions[1];
		if (a.Scores[0] != b.Scores[0]) return a.Scores[0] < b.Scores[0];
		return a.Scores[1] < b.Scores[1];
	}
}

using namespace Puzzle21_2021_Types;

static vector<int> GetStartingPositions(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	vector<int> positions = { 0, 0 };
	for (int i = 0; i < 2; i++)
	{
		sscanf(lines[i].c_str(), "Player %*c starting position: %d", &positions[i]);
		positions[i]--;
	}

	return positions;
}

static int g_diceA = 0;

static int Roll_A()
{
	int roll = 0;

	roll += g_diceA + 1;
	g_diceA = (g_diceA + 1) % 100;
	roll += g_diceA + 1;
	g_diceA = (g_diceA + 1) % 100;
	roll += g_diceA + 1;
	g_diceA = (g_diceA + 1) % 100;

	return roll;
}

// 1 1 1 = 3
// 1 1 2 = 4
// 1 1 3 = 5
// 1 2 1 = 4
// 1 2 2 = 5
// 1 2 3 = 6
// 1 3 1 = 5
// 1 3 2 = 6
// 1 3 3 = 7

// 2 1 1 = 4
// 2 1 2 = 5
// 2 1 3 = 6
// 2 2 1 = 5
// 2 2 2 = 6
// 2 2 3 = 7
// 2 3 1 = 6
// 2 3 2 = 7
// 2 3 3 = 8

// 3 1 1 = 5
// 3 1 2 = 6
// 3 1 3 = 7
// 3 2 1 = 6
// 3 2 2 = 7
// 3 2 3 = 8
// 3 3 1 = 7
// 3 3 2 = 8
// 3 3 3 = 9

// 3 = 1
// 4 = 3
// 5 = 6
// 6 = 7
// 7 = 6
// 8 = 3
// 9 = 1

static map<GameState, int64_t> g_cachedOutcomes;

static GameState UpdateGame(const GameState& current, int roll)
{
	GameState next = current;
	next.Positions[current.CurrentPlayer] = (current.Positions[current.CurrentPlayer] + roll) % 10;
	next.Scores[current.CurrentPlayer] += next.Positions[current.CurrentPlayer] + 1;
	next.CurrentPlayer = 1 - current.CurrentPlayer;
	return next;
}

static int64_t Player1WinOutcomes(const GameState& current)
{
	map<GameState, int64_t>::const_iterator cachedOutcome = g_cachedOutcomes.find(current);
	if (cachedOutcome != g_cachedOutcomes.end())
		return cachedOutcome->second;

	int64_t numberOfWins = 0;

	// 1 universe in which we roll 3
	{
		GameState next = UpdateGame(current, 3);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 1 : 0;
		}
		else
		{
			numberOfWins += 1 * Player1WinOutcomes(next);
		}
	}

	// 3 universes in which we roll 4
	{
		GameState next = UpdateGame(current, 4);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 3 : 0;
		}
		else
		{
			numberOfWins += 3 * Player1WinOutcomes(next);
		}
	}

	// 6 universes in which we roll 5
	{
		GameState next = UpdateGame(current, 5);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 6 : 0;
		}
		else
		{
			numberOfWins += 6 * Player1WinOutcomes(next);
		}
	}

	// 7 universes in which we roll 6
	{
		GameState next = UpdateGame(current, 6);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 7 : 0;
		}
		else
		{
			numberOfWins += 7 * Player1WinOutcomes(next);
		}
	}

	// 6 universes in which we roll 7
	{
		GameState next = UpdateGame(current, 7);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 6 : 0;
		}
		else
		{
			numberOfWins += 6 * Player1WinOutcomes(next);
		}
	}

	// 3 universes in which we roll 8
	{
		GameState next = UpdateGame(current, 8);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 3 : 0;
		}
		else
		{
			numberOfWins += 3 * Player1WinOutcomes(next);
		}
	}

	// 1 universe in which we roll 9
	{
		GameState next = UpdateGame(current, 9);
		if (next.Scores[current.CurrentPlayer] >= 21)
		{
			numberOfWins += current.CurrentPlayer == 0 ? 1 : 0;
		}
		else
		{
			numberOfWins += 1 * Player1WinOutcomes(next);
		}
	}

	g_cachedOutcomes[current] = numberOfWins;
	return numberOfWins;
}

static void Puzzle21_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> positions = GetStartingPositions(input);
	vector<int> scores = { 0, 0 };

	int numRolls = 0;

	int currentPlayer = 0;
	while (true)
	{
		numRolls += 3;

		positions[currentPlayer] = (positions[currentPlayer] + Roll_A()) % 10;
		scores[currentPlayer] += positions[currentPlayer] + 1;

		if (scores[currentPlayer] >= 1000)
			break;

		currentPlayer = 1 - currentPlayer;
	}

	int64_t answer = numRolls * scores[1 - currentPlayer];

	printf("[2021] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> positions = GetStartingPositions(input);

	GameState start;
	start.Positions[0] = positions[0];
	start.Positions[1] = positions[1];

	int64_t numberOfPlayer1Wins = Player1WinOutcomes(start);

	swap(start.Positions[0], start.Positions[1]);
	start.CurrentPlayer = 1;

	int64_t numberOfPlayer2Wins = Player1WinOutcomes(start);

	int64_t answer = max(numberOfPlayer1Wins, numberOfPlayer2Wins);

	printf("[2021] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2021(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
