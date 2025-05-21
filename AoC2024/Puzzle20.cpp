#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2024_Types
{
	struct Puzzle
	{
		ArrayMap2D Maze;

		Point2 Start;
		Point2 End;
	};
}

using namespace Puzzle20_2024_Types;

static Puzzle MakePuzzle(istream& input)
{
	Puzzle p{ ReadArrayMap(input) };

	for (const auto& kvp : p.Maze.Grid())
	{
		switch (kvp.second)
		{
		case 'S':
			p.Start = kvp.first;
			break;

		case 'E':
			p.End = kvp.first;
			break;
		}
	}

	return p;
}

static vector<Point2> GetShortestPath(const Puzzle& p)
{
	vector<Point2> shortestPath;

	set<Point2> visited;

	vector<pair<Point2, int64_t>> searchQueue{ { p.Start, 0 } };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentPos = searchQueue[i].first;
		int64_t currentCost = searchQueue[i].second;

		if (visited.contains(currentPos))
		{
			continue;
		}

		visited.insert(currentPos);
		shortestPath.push_back(currentPos);

		size_t queueBefore = searchQueue.size();
		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 nextPos = currentPos + dir;
			if (!visited.contains(nextPos) && p.Maze(nextPos) != '#')
			{
				searchQueue.push_back({ nextPos, currentCost + 1 });
			}
		}
		assert((searchQueue.size() == queueBefore + 1) || (searchQueue.size() == queueBefore));
		(void)queueBefore;
	}

	return shortestPath;
}

static void Puzzle20_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = MakePuzzle(input);

	int64_t maximumCheatDistance = 2;
	int64_t savingsWanted = 100;

	int64_t answer = 0;

	vector<Point2> shortestPath = GetShortestPath(p);
	for (size_t cheatStart = 0; cheatStart < shortestPath.size(); cheatStart++)
	{
		for (size_t cheatEnd = shortestPath.size() - 1; cheatEnd > cheatStart + 1; cheatEnd--)
		{
			int64_t cheatDistance = ManhattanDistance(shortestPath[cheatStart], shortestPath[cheatEnd]);
			int64_t cheatSavings = (cheatEnd - cheatStart) - cheatDistance;
			if ((cheatDistance <= maximumCheatDistance) && (cheatSavings >= savingsWanted))
			{
				answer++;
			}
		}
	}

	printf("[2024] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = MakePuzzle(input);

	int64_t maximumCheatDistance = 20;
	int64_t savingsWanted = 100;

	int64_t answer = 0;

	vector<Point2> shortestPath = GetShortestPath(p);
	for (size_t cheatStart = 0; cheatStart < shortestPath.size(); cheatStart++)
	{
		for (size_t cheatEnd = shortestPath.size() - 1; cheatEnd > cheatStart + 1; cheatEnd--)
		{
			int64_t cheatDistance = ManhattanDistance(shortestPath[cheatStart], shortestPath[cheatEnd]);
			int64_t cheatSavings = (cheatEnd - cheatStart) - cheatDistance;
			if ((cheatDistance <= maximumCheatDistance) && (cheatSavings >= savingsWanted))
			{
				answer++;
			}
		}
	}

	printf("[2024] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2024(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
