#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2023_Types
{
	struct Graph
	{
		set<Point2> Vertices;
		map<Point2, set<pair<Point2, int64_t>>> Edges;
	};

	struct Puzzle
	{
		ArrayMap2D Maze;
		Point2 Start;
		Point2 End;
	};
}

using namespace Puzzle23_2023_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p{ ReadArrayMap(input, '#') };
	p.Start = Point2{ 1, 0 };
	p.End = Point2{ p.Maze.GetWidth() - 2, p.Maze.GetHeight() - 1 };

	assert(p.Maze(p.Start) == '.');
	assert(p.Maze(p.End) == '.');

	return p;
}

static bool WithDirectionConstraints(const Point2& direction, char tile)
{
	switch (tile)
	{
	case '^': return direction == Point2::North();
	case 'v': return direction == Point2::South();
	case '<': return direction == Point2::West();
	case '>': return direction == Point2::East();
	}
	return true;
}

static bool WithoutDirectionConstraints(const Point2&, char)
{
	return true;
}

static tuple<Point2, int64_t, Point2, bool> FindNextVertex(const Puzzle& puzzle,
	const Point2& startFrom,
	const Point2& startFacing,
	int64_t startingSteps,
	function<bool(const Point2&, char)> isAllowable,
	set<Point2> visited)
{
	// Follow the path as far as possible while there's only one option
	Point2 currentPos = startFrom;
	Point2 currentDirection = startFacing;
	int64_t currentSteps = startingSteps;

	while (true)
	{
		visited.insert(currentPos);

		// Have we reached the end of the map?
		if (currentPos == puzzle.End)
		{
			return { currentPos, currentSteps, currentDirection, true };
		};

		// How many ways can we go?
		Point2 choices[4];
		int numChoices = 0;
		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = currentPos + dir;
			char neighbourTile = puzzle.Maze(neighbour);

			if ((neighbourTile != '#') &&
				(visited.contains(neighbour) == false) &&
				isAllowable(dir, neighbourTile))
			{
				choices[numChoices++] = dir;
			}
		}

		if (numChoices == 0)
		{
			return { {}, -1, {}, false };
		}

		if (numChoices > 1)
		{
			return { currentPos, currentSteps, currentDirection, true };
		}

		currentPos = currentPos + choices[0];
		currentSteps++;
		currentDirection = choices[0];
	}
}

static Graph GenerateGraph(const Puzzle& puzzle, function<bool(const Point2&, char)> isAllowable)
{
	Graph graph;

	vector<pair<Point2, Point2>> searchQueue{ { puzzle.Start, Point2::South() } };
	set<pair<Point2, Point2>> visited;

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPosition, currentDirection] = searchQueue[i];
		graph.Vertices.insert(currentPosition);

		if (visited.insert({ currentPosition, currentDirection }).second == false)
			continue;

		for (const Point2& dir : Point2::CardinalDirections())
		{
			if (puzzle.Maze(currentPosition + dir) != '#')
			{
				auto [nextPosition, stepsTaken, nextDirection, isValid] = FindNextVertex(puzzle,
					currentPosition + dir,
					dir,
					1,
					isAllowable,
					{ currentPosition });
				if (isValid)
				{
					graph.Edges[currentPosition].insert({ nextPosition, stepsTaken });
					searchQueue.push_back({ nextPosition, nextDirection });
				}
			}
		}
	}

	return graph;
}

static int64_t FindMaximumPath(const Puzzle& puzzle, const Graph& graph, const Point2& currentPosition, int64_t currentSteps, set<Point2>* visited)
{
	if (currentPosition == puzzle.End)
	{
		return currentSteps;
	}

	visited->insert(currentPosition);

	int64_t maximumPath = 0;
	for (const auto& neighbour : graph.Edges.at(currentPosition))
	{
		if (visited->contains(neighbour.first))
			continue;

		int64_t candidatePath = FindMaximumPath(puzzle, graph, neighbour.first, currentSteps + neighbour.second, visited);
		maximumPath = max(candidatePath, maximumPath);
	}

	visited->erase(currentPosition);

	return maximumPath;
}

static void Puzzle23_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);
	Graph graph = GenerateGraph(puzzle, &WithDirectionConstraints);

	set<Point2> visited;
	int64_t answer = FindMaximumPath(puzzle, graph, puzzle.Start, 0, &visited);

	printf("[2023] Puzzle23_A: %" PRId64 "\n", answer);
}

static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);
	Graph graph = GenerateGraph(puzzle, &WithoutDirectionConstraints);

	set<Point2> visited;
	int64_t answer = FindMaximumPath(puzzle, graph, puzzle.Start, 0, &visited);

	printf("[2023] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2023(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
