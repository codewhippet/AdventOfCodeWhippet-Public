#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2016_Types
{
}

using namespace Puzzle24_2016_Types;

static int64_t DistanceBetweenNodes(const ArrayMap2D& maze, const Point2& start, const Point2& stop)
{
	vector<pair<int64_t, Point2>> searchQueue{ { 0, start } };

	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentSteps, currentPos] = searchQueue[i];
		if (currentPos == stop)
		{
			return currentSteps;
		}

		if (visited.contains(currentPos))
		{
			continue;
		}

		visited.insert(currentPos);

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 next = currentPos + dir;
			if (maze(next) != '#')
			{
				searchQueue.push_back({ currentSteps + 1, next });
			}
		}
	}

	return -1;
}

static auto DistancesBetweenAllNodes(const ArrayMap2D& maze, const vector<pair<char, Point2>>& nodes)
{
	map<pair<size_t, size_t>, int64_t> distancesBetweenNodes;
	for (size_t i = 0; i < nodes.size(); i++)
	{
		for (size_t j = i + 1; j < nodes.size(); j++)
		{
			int64_t distance = DistanceBetweenNodes(maze, nodes[i].second, nodes[j].second);
			distancesBetweenNodes[{ i, j }] = distance;
			distancesBetweenNodes[{ j, i }] = distance;
		}
	}
	return distancesBetweenNodes;
}

static int64_t ShortestVisitToAll(const vector<pair<char, Point2>>& nodes,
	const map<pair<size_t, size_t>, int64_t>& distancesBetweenNodes,
	const function<bool(pair<size_t, size_t>, size_t, size_t, size_t)> &isMoveAllowed)
{
	multimap<int64_t, pair<size_t, size_t>> searchQueue{ { 0, { 0, 1ll << 0 } } };

	const size_t targetSet = (1ll << nodes.size()) - 1;

	set<pair<size_t, size_t>> visited;
	while (searchQueue.empty() == false)
	{
		auto [currentSteps, currentState] = *searchQueue.begin();

		searchQueue.erase(searchQueue.begin());
		if (currentState.second == targetSet)
		{
			return currentSteps;
		}

		if (visited.contains(currentState))
		{
			continue;
		}

		visited.insert(currentState);

		for (size_t nextNode = 0; nextNode < nodes.size(); nextNode++)
		{
			size_t nextSetElement = 1ll << nextNode;

			if (isMoveAllowed(currentState, nodes.size(), nextNode, nextSetElement) == false)
			{
				continue;
			}

			pair<size_t, size_t> nextState{ nextNode, currentState.second | nextSetElement };
			int64_t stepCost = distancesBetweenNodes.at({ currentState.first, nextState.first });
			searchQueue.insert({ currentSteps + stepCost, nextState });
		}
	}

	return -1;
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D maze = ReadArrayMap(input);

	vector<pair<char, Point2>> nodes;
	for (const auto& cell : maze.Grid())
	{
		if (isdigit(cell.second))
		{
			nodes.push_back({ cell.second, cell.first });
		}
	}
	ranges::sort(nodes);

	map<pair<size_t, size_t>, int64_t> distancesBetweenNodes = DistancesBetweenAllNodes(maze, nodes);

	int64_t answer = ShortestVisitToAll(nodes, distancesBetweenNodes,
		[](pair<size_t, size_t> currentState, size_t, size_t nextNode, size_t nextSetElement) -> bool
		{
			// Don't try 0 moves
			if (nextNode == currentState.first)
			{
				return false;
			}

			// Don't bother with nodes we've already visited
			if (currentState.second & nextSetElement)
			{
				return false;
			}

			return true;
		});

	printf("[2016] Puzzle24_A: %" PRId64 "\n", answer);
}


static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D maze = ReadArrayMap(input);

	vector<pair<char, Point2>> nodes;
	for (const auto& cell : maze.Grid())
	{
		if (isdigit(cell.second))
		{
			nodes.push_back({ cell.second, cell.first });
		}
	}
	ranges::sort(nodes);

	// Returning to the starting node is the same as having a final node at the same location
	nodes.push_back({ (char)('0' + nodes.size()), nodes[0].second });

	map<pair<size_t, size_t>, int64_t> distancesBetweenNodes = DistancesBetweenAllNodes(maze, nodes);

	int64_t answer = ShortestVisitToAll(nodes, distancesBetweenNodes,
		[](pair<size_t, size_t> currentState, size_t nodeCount, size_t nextNode, size_t nextSetElement) -> bool
		{
			// Don't try 0 moves
			if (nextNode == currentState.first)
			{
				return false;
			}

			// Don't bother with nodes we've already visited
			if (currentState.second & nextSetElement)
			{
				return false;
			}

			const size_t targetSubset = (1ll << (nodeCount - 1)) - 1;

			// Moves to the final node are only valid if we've visited everything else
			bool isLastNode = (nextNode == nodeCount - 1);
			bool visitedTargetSubset = (currentState.second == targetSubset);
			if (isLastNode && !visitedTargetSubset)
			{
				return false;
			}

			return true;
		});

	printf("[2016] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2016(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
