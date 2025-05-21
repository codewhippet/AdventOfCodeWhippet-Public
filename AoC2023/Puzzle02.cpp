#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2023_Types
{
	struct Game
	{
		int Id;
		vector<map<string, int>> Rounds;
	};
}

using namespace Puzzle02_2023_Types;

static vector<map<string, int>> ParseRounds(const string& allRoundsText)
{
	vector<map<string, int>> rounds;

	for (const string& roundText : StringSplit(allRoundsText, ';'))
	{
		map<string, int> round;
		for (const string& numRocks : StringSplit(roundText, ','))
		{
			istringstream rockInput(numRocks);
			int num;
			string rock;
			rockInput >> num >> rock;
			round[rock] = num;
		}
		rounds.emplace_back(move(round));
	}

	return rounds;
}

static vector<Game> ParseGames(istream& input)
{
	const regex lineFormat(R"(Game (\d+): (.+))");

	vector<Game> games;

	for (const string& line : ReadAllLines(input))
	{
		smatch lineMatch;
		if (regex_match(line, lineMatch, lineFormat))
		{
			Game game;
			game.Id = stoi(lineMatch[1].str());
			game.Rounds = ParseRounds(lineMatch[2].str());
			games.emplace_back(move(game));
		}
	}

	return games;
}

static void Puzzle02_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const map<string, int> maxCubes =
	{
		{"red", 12},
		{"green", 13},
		{"blue", 14},
	};

	vector<Game> games = ParseGames(input);

	int64_t answer = MakeEnumerator(games)
		->Where([&maxCubes](const Game& g)
			{
				for (const map<string, int>& round : g.Rounds)
				{
					for (map<string, int>::const_reference cubes : round)
					{
						if (cubes.second > maxCubes.at(cubes.first))
						{
							return false;
						}
					}
				}

				return true;
			})
		->Select<int>([](const Game& g)
			{
				return g.Id;
			})
		->Sum();

	printf("[2023] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Game> games = ParseGames(input);

	int64_t answer = MakeEnumerator(games)
		->Select<map<string, int>>([](const Game& g)
			{
				map<string, int> minimums;
				for (const map<string, int>& round : g.Rounds)
				{
					for (map<string, int>::const_reference cubes : round)
					{
						minimums[cubes.first] = max(minimums[cubes.first], cubes.second);
					}
				}
				return minimums;
			})
		->Select<int64_t>([](const map<string, int>& minimums)
			{
				int64_t power = 1;
				for (map<string, int>::const_reference cubes : minimums)
				{
					power *= cubes.second;
				}
				return power;
			})
		->Sum();

	printf("[2023] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2023(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
