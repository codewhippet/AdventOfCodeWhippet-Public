#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2024_Types
{
	static const map<Point2, char> Directions =
	{
		{ Point2::Up(), '^' },
		{ Point2::Down(), 'v' },
		{ Point2::Left(), '<' },
		{ Point2::Right(), '>' },
	};

	static const map<char, Point2> MoveDirections =
	{
		{ '^', Point2::Up() },
		{ 'v', Point2::Down() },
		{ '<', Point2::Left() },
		{ '>', Point2::Right() },
		{ 'A', {} }
	};

	static vector<pair<char, Point2>> DoorKeypad =
	{
		{ 'A', Point2{} },

		{ '0', Point2{} + Point2::Left() },

		{ '1', Point2{} + Point2::Up() + Point2::Left() + Point2::Left() },
		{ '2', Point2{} + Point2::Up() + Point2::Left() },
		{ '3', Point2{} + Point2::Up() },

		{ '4', Point2{} + Point2::Up() + Point2::Up() + Point2::Left() + Point2::Left() },
		{ '5', Point2{} + Point2::Up() + Point2::Up() + Point2::Left() },
		{ '6', Point2{} + Point2::Up() + Point2::Up() },

		{ '7', Point2{} + Point2::Up() + Point2::Up() + Point2::Up() + Point2::Left() + Point2::Left() },
		{ '8', Point2{} + Point2::Up() + Point2::Up() + Point2::Up() + Point2::Left() },
		{ '9', Point2{} + Point2::Up() + Point2::Up() + Point2::Up() },
	};

	static vector<pair<char, Point2>> RobotKeypad =
	{
		{ 'A', Point2{} },

		{ '^', Point2{} + Point2::Left() },

		{ '<', Point2{} + Point2::Down() + Point2::Left() + Point2::Left() },
		{ 'v', Point2{} + Point2::Down() + Point2::Left() },
		{ '>', Point2{} + Point2::Down() },
	};
}

using namespace Puzzle21_2024_Types;

static vector<string> FindShortestRoutes(const Point2& from, char to, const map<Point2, char>& buttons)
{
	vector<string> routes;

	vector<pair<Point2, string>> searchQueue{ { from, "" } };

	set<Point2> visited;
	size_t shortestRoute = numeric_limits<size_t>::max();
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentPos = searchQueue[i].first;
		string currentSequence = searchQueue[i].second;
		if (currentSequence.size() > shortestRoute)
			break;

		if (visited.contains(currentPos))
			continue;

		if (buttons.at(currentPos) == to)
		{
			routes.push_back(currentSequence);
			shortestRoute = currentSequence.size();
		}

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 nextPos = currentPos + dir;
			string nextSequence = currentSequence + Directions.at(dir);
			if (buttons.contains(nextPos))
			{
				searchQueue.push_back({ nextPos, nextSequence });
			}
		}
	}

	return routes;
}

static map<string, vector<string>> GenerateShortestSequences(const vector<pair<char, Point2>>& keypad)
{
	map<Point2, char> buttons;
	ranges::copy(keypad |
		views::transform([](const auto& buttonLocation)
			{
				return make_pair(buttonLocation.second, buttonLocation.first);
			}),
		inserter(buttons, buttons.end()));

	map<string, vector<string>> sequences;
	for (size_t i = 0; i < keypad.size(); i++)
	{
		for (size_t j = 0; j < keypad.size(); j++)
		{
			string buttonPair = string{} + keypad[i].first + keypad[j].first;
			sequences[buttonPair] = FindShortestRoutes(keypad[i].second, keypad[j].first, buttons);
		}
	}

	return sequences;
}

static map<pair<int, string>, size_t> CalculateRobotCosts(const map<string, vector<string>>& robotSequences, int numRobots)
{
	map<pair<int, string>, size_t> costs;

	if (numRobots == 1)
	{
		for (const auto& keyPair : robotSequences)
		{
			costs[{ numRobots, keyPair.first }] = ranges::min(keyPair.second | views::transform([](const string& s) { return s.size(); })) + 1;
		}

		return costs;
	}

	costs = CalculateRobotCosts(robotSequences, numRobots - 1);

	for (const auto& keyPair : robotSequences)
	{
		string buttonPair = keyPair.first;
		size_t cost = 0;
		if (buttonPair[0] == buttonPair[1])
		{
			cost = 1;
		}
		else
		{
			cost = ranges::min(keyPair.second
				| views::transform([&](const string& directionSequence)
					{
						string fullSequence = "A" + directionSequence + "A";
						size_t cost = 0;
						for (size_t i = 0; i + 1 < fullSequence.size(); i++)
						{
							cost += costs.at({ numRobots - 1, string{} + fullSequence[i] + fullSequence[i + 1] });
						}
						return cost;
					}));
		}
		costs[{ numRobots, buttonPair }] = cost;
	}

	return costs;
}

static size_t CalculateDoorCost(const string& password, const map<string, vector<string>>& doorSequences, int numRobots, const map<pair<int, string>, size_t>& robotCosts)
{
	size_t doorCost = 0;
	for (size_t i = 0; i + 1 < password.size(); i++)
	{
		string buttonPair = string{} + password[i] + password[i + 1];

		size_t stepCost = 0;
		if (buttonPair[0] == buttonPair[1])
		{
			stepCost = 1;
		}
		else
		{
			stepCost = ranges::min(doorSequences.at(buttonPair)
				| views::transform([&](const string& directionSequence)
					{
						string fullSequence = "A" + directionSequence + "A";
						size_t cost = 0;
						for (size_t i = 0; i + 1 < fullSequence.size(); i++)
						{
							cost += robotCosts.at({ numRobots - 1, string{} + fullSequence[i] + fullSequence[i + 1] });
						}
						return cost;
					}));
		}

		doorCost += stepCost;
	}

	return doorCost;
}

static void Puzzle21_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int numRobots = 3;

	map<string, vector<string>> doorSequences = GenerateShortestSequences(DoorKeypad);
	map<string, vector<string>> robotSequences = GenerateShortestSequences(RobotKeypad);

	map<pair<int, string>, size_t> robotSequenceCosts = CalculateRobotCosts(robotSequences, numRobots - 1);

	int64_t answer = 0;
	for (const string& password : ReadAllLines(input))
	{
		size_t doorCost = CalculateDoorCost(string{ "A" } + password, doorSequences, numRobots, robotSequenceCosts);
		int64_t doorCode = atoll(password.c_str());
		int64_t complexity = doorCost * doorCode;

		answer += complexity;
	}

	printf("[2024] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int numRobots = 26;

	map<string, vector<string>> doorSequences = GenerateShortestSequences(DoorKeypad);
	map<string, vector<string>> robotSequences = GenerateShortestSequences(RobotKeypad);

	map<pair<int, string>, size_t> robotSequenceCosts = CalculateRobotCosts(robotSequences, numRobots - 1);

	int64_t answer = 0;
	for (const string& password : ReadAllLines(input))
	{
		size_t doorCost = CalculateDoorCost(string{ "A" } + password, doorSequences, numRobots, robotSequenceCosts);
		int64_t doorCode = atoll(password.c_str());
		int64_t complexity = doorCost * doorCode;

		answer += complexity;
	}

	printf("[2024] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2024(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
