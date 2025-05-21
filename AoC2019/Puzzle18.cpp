#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2019_Types
{
	struct Puzzle
	{
		ArrayMap2D Maze;
		Point2 Start;
		string Keys;
		map<char, Point2> SpecialLocations;
		map<char, set<pair<char, int64_t>>> Edges;
	};

	struct SearchNode
	{
		string CurrentLocations;
		string Collected;
		auto operator<=>(const SearchNode&) const = default;
	};
}

using namespace Puzzle18_2019_Types;

static Puzzle ParsePuzzle(istream &input)
{
	Puzzle puzzle{ ReadArrayMap(input) };
	for (const auto& p : puzzle.Maze.Grid())
	{
		if (p.second == '@')
		{
			puzzle.SpecialLocations[p.second] = p.first;
			puzzle.Start = p.first;
		}
		else if (isalpha(p.second))
		{
			puzzle.SpecialLocations[p.second] = p.first;
			if (islower(p.second))
			{
				puzzle.Keys += p.second;
			}
		}
	}
	return puzzle;
}

static void GenerateEdges(Puzzle* p, char poi)
{
	vector<pair<Point2, int64_t>> searchQueue{ { p->SpecialLocations.at(poi), 0 } };
	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPos, currentSteps] = searchQueue[i];
		if (visited.insert(currentPos).second == false)
			continue;

		char currentTile = p->Maze(currentPos);
		if ((currentTile != poi) && (currentTile != '.'))
		{
			p->Edges[poi].insert({ currentTile, currentSteps });
			continue;
		}

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = currentPos + dir;
			if (p->Maze(neighbour) != '#')
				searchQueue.push_back({ neighbour, currentSteps + 1 });
		}
	}
}

static void GenerateEdgesForAll(Puzzle* p)
{
	for (char c : p->SpecialLocations | views::keys)
	{
		GenerateEdges(p, c);
	}
}

static int64_t ShortestPathToCollectAll(const Puzzle& p, const SearchNode& startingState)
{
	multimap<int64_t, SearchNode> searchQueue{ { 0, startingState } };
	set<SearchNode> visited;

	while (searchQueue.empty() == false)
	{
		multimap<int64_t, SearchNode>::value_type current = *searchQueue.begin();
		searchQueue.erase(searchQueue.begin());
		if (visited.insert(current.second).second == false)
			continue;

		if (current.second.Collected.size() == p.Keys.size())
			return current.first;

		for (size_t robotIndex = 0; robotIndex < current.second.CurrentLocations.size(); robotIndex++)
		{
			char robotLocation = current.second.CurrentLocations[robotIndex];
			for (const auto& neighbour : p.Edges.at(robotLocation))
			{
				bool isUnlockedDoor = isupper(neighbour.first) && (current.second.Collected.find((char)tolower(neighbour.first)) != string::npos);
				bool isCollectedKey = islower(neighbour.first) && (current.second.Collected.find(neighbour.first) != string::npos);
				bool isStartingPoint = isdigit(neighbour.first);

				bool isUncollectedKey = islower(neighbour.first) && (current.second.Collected.find(neighbour.first) == string::npos);

				string nextRobotLocations{ current.second.CurrentLocations };
				nextRobotLocations[robotIndex] = neighbour.first;

				if (isUnlockedDoor || isCollectedKey || isStartingPoint)
				{
					SearchNode nextNode{ nextRobotLocations, current.second.Collected };
					searchQueue.insert({ current.first + neighbour.second, nextNode });
				}
				else if (isUncollectedKey)
				{
					SearchNode nextNode{ nextRobotLocations, current.second.Collected + neighbour.first };
					ranges::sort(nextNode.Collected);
					searchQueue.insert({ current.first + neighbour.second, nextNode });
				}
			}
		}
	}

	return -1;
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);
	puzzle.Maze(puzzle.Start) = '1';
	puzzle.SpecialLocations['1'] = puzzle.SpecialLocations.at('@');
	puzzle.SpecialLocations.erase('@');

	GenerateEdgesForAll(&puzzle);

	int64_t answer = ShortestPathToCollectAll(puzzle, { "1", "" });

	printf("[2019] Puzzle18_A: %" PRId64 "\n", answer);
}


static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);

	puzzle.SpecialLocations.erase('@');
	for (const auto& p : vector<Point2>{ {}, Point2::Up(), Point2::Down(), Point2::Left(), Point2::Right() })
	{
		puzzle.Maze(puzzle.Start + p) = '#';
	}

	const vector<pair<char, Point2>> startingPositions =
	{
		{ '1', Point2::Up() + Point2::Left() },
		{ '2', Point2::Up() + Point2::Right() },
		{ '3', Point2::Down() + Point2::Left() },
		{ '4', Point2::Down() + Point2::Right() }
	};
	for (const auto& p : startingPositions)
	{
		puzzle.Maze(puzzle.Start + p.second) = p.first;
		puzzle.SpecialLocations[p.first] = puzzle.Start + p.second;
	}

	GenerateEdgesForAll(&puzzle);

	int64_t answer = ShortestPathToCollectAll(puzzle, { "1234", "" });

	printf("[2019] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2019(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
