#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2019_Types
{
	struct Puzzle
	{
		ArrayMap2D Maze;
		map<Point2, Point2> Teleporters;
		map<Point2, string> DebugLabels;
		Point2 Start;
		Point2 End;
	};
}

using namespace Puzzle20_2019_Types;

static string ParseLabel(const ArrayMap2D &maze, const Point2& adjacentChar)
{
	string label;
	for (const Point2& dir : array<Point2, 2>{ Point2::Left(), Point2::Up() })
	{
		if (isalpha(maze(adjacentChar + dir)))
		{
			label += maze(adjacentChar + dir);
			label += maze(adjacentChar);
		}
	}
	for (const Point2& dir : array<Point2, 2>{ Point2::Right(), Point2::Down() })
	{
		if (isalpha(maze(adjacentChar + dir)))
		{
			label += maze(adjacentChar);
			label += maze(adjacentChar + dir);
		}
	}
	return label;
}

static Puzzle ParsePuzzle(istream& input)
{
	Puzzle puzzle{ ReadArrayMap(input) };

	map<string, vector<Point2>> labels;
	for (const auto& p : puzzle.Maze.Grid())
	{
		if (p.second == '.')
		{
			for (const auto& dir : Point2::CardinalDirections())
			{
				Point2 neighbour = p.first + dir;
				if (isalpha(puzzle.Maze(neighbour)))
				{
					string label = ParseLabel(puzzle.Maze, neighbour);
					labels[label].push_back(p.first);
					puzzle.DebugLabels[p.first] = label;
					if (label == "AA"sv)
					{
						puzzle.Start = p.first;
					}
					else if (label == "ZZ"sv)
					{
						puzzle.End = p.first;
					}
				}
			}
		}
	}

	for (const auto& label : labels | views::filter([](const auto& l) { return l.second.size() == 2; }))
	{
		puzzle.Teleporters[label.second[0]] = label.second[1];
		puzzle.Teleporters[label.second[1]] = label.second[0];
	}

	return puzzle;
}

static bool IsOuterTeleporter(const Puzzle& puzzle, const Point2& teleporter)
{
	return (teleporter.X == 2)
		|| (teleporter.Y == 2)
		|| (teleporter.X == puzzle.Maze.GetWidth() - 3)
		|| (teleporter.Y == puzzle.Maze.GetHeight() - 3);
}

static int64_t ShortestPath(const Puzzle& puzzle)
{
	vector<pair<Point2, int64_t>> searchQueue{ { puzzle.Start, 0 } };
	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPos, currentSteps] = searchQueue[i];

		if (currentPos == puzzle.End)
			return currentSteps;

		if (visited.insert(currentPos).second == false)
			continue;

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = currentPos + dir;
			if (puzzle.Maze(neighbour) == '.')
			{
				searchQueue.push_back({ neighbour, currentSteps + 1 });
			}
		}

		const auto teleporter = puzzle.Teleporters.find(currentPos);
		if (teleporter != puzzle.Teleporters.end())
		{
			searchQueue.push_back({ teleporter->second, currentSteps + 1 });
		}
	}

	return -1;
}

static int64_t ShortestPathRecursive(const Puzzle& puzzle)
{
	const Vector3 start{ puzzle.Start.X, puzzle.Start.Y, 0 };
	const Vector3 end{ puzzle.End.X, puzzle.End.Y, 0 };

	vector<pair<Vector3, int64_t>> searchQueue{ { start, 0 } };
	set<Vector3> visited;

	map<Vector3, Vector3> from;

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPos, currentSteps] = searchQueue[i];
		if (currentPos == end)
			return currentSteps;

		if (visited.insert(currentPos).second == false)
			continue;

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = Point2{ currentPos.X, currentPos.Y } + dir;
			if (puzzle.Maze(neighbour) == '.')
			{
				Vector3 next{ neighbour.X, neighbour.Y, currentPos.Z };
				from.insert({ next, currentPos });
				searchQueue.push_back({ next, currentSteps + 1 });
			}
		}

		const auto teleporter = puzzle.Teleporters.find(Point2{ currentPos.X, currentPos.Y });
		if (teleporter != puzzle.Teleporters.end())
		{
			if (IsOuterTeleporter(puzzle, Point2{ currentPos.X, currentPos.Y }))
			{
				if (currentPos.Z > 0)
				{
					Vector3 next{ teleporter->second.X, teleporter->second.Y, currentPos.Z - 1 };
					from.insert({ next, currentPos });
					searchQueue.push_back({ next, currentSteps + 1 });
				}
			}
			else
			{
				Vector3 next{ teleporter->second.X, teleporter->second.Y, currentPos.Z + 1 };
				from.insert({ next, currentPos });
				searchQueue.push_back({ next, currentSteps + 1 });
			}
		}
	}

	return -1;
}

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);

	int64_t answer = ShortestPath(puzzle);

	printf("[2019] Puzzle20_A: %" PRId64 "\n", answer);
}


static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);

	int64_t answer = ShortestPathRecursive(puzzle);

	printf("[2019] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2019(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
