#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2016_Types
{
	struct Node
	{
		int64_t Size = 0;
		int64_t Used = 0;
		int64_t Avail = 0;
	};
}

using namespace Puzzle22_2016_Types;

static map<Point2, Node> ParseNodes(istream& input)
{
	map<Point2, Node> nodes;

	for (const auto& m : ReadEachLine(input, regex{ R"(/dev/grid/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+\d+%)" }))
	{
		if (m.empty())
			continue;

		Point2 nodeLocation{ atoll(m[1].str().c_str()), atoll(m[2].str().c_str()) };
		Node nodeDetails;
		nodeDetails.Size = atoll(m[3].str().c_str());
		nodeDetails.Used = atoll(m[4].str().c_str());
		nodeDetails.Avail = atoll(m[5].str().c_str());

		nodes[nodeLocation] = nodeDetails;
	}

	return nodes;
}

static vector<Vector4> Neighbours(const ArrayMap2D& grid, const Vector4 &state)
{
	vector<Vector4> neighbours;

	const Point2 emptySlot{ state.X, state.Y };
	const Point2 goalData{ state.Z, state.W };

	// Can we move the goal data into an empty slot?
	if (ManhattanDistance(emptySlot, goalData) == 1)
	{
		neighbours.push_back({ goalData.X, goalData.Y, emptySlot.X, emptySlot.Y });
	}

	// Move the empty slot
	for (const Point2& dir : Point2::CardinalDirections())
	{
		Point2 newEmptySlot = emptySlot + dir;
		if (grid.IsInside(newEmptySlot) &&
			newEmptySlot != goalData &&
			grid(newEmptySlot) != '#')
		{
			neighbours.push_back({ newEmptySlot.X, newEmptySlot.Y, goalData.X, goalData.Y });
		}
	}

	return neighbours;
}

static int64_t ShortestPathForGoalData(const ArrayMap2D& grid, const Point2 &emptyNode, const Point2 &goalData)
{
	const Vector4 startingState{ emptyNode.X, emptyNode.Y, goalData.X, goalData.Y };

	list<pair<int64_t, Vector4>> searchQueue = { { 0, startingState } };
	set<Vector4> queued{ startingState };

	while (searchQueue.empty() == false)
	{
		Vector4 current = searchQueue.front().second;
		int64_t currentSteps = searchQueue.front().first;
		searchQueue.pop_front();

		if ((current.Z == 0) && (current.W == 0))
		{
			return currentSteps;
		}

		vector<Vector4> neighbours = Neighbours(grid, current);
		for (const Vector4& n : neighbours)
		{
			if (queued.contains(n) == false)
			{
				searchQueue.push_back({ currentSteps + 1, n });
				queued.insert(n);
			}
		}
	}

	return -1;
}

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, Node> nodes = ParseNodes(input);

	vector<Node> nodeList;
	ranges::copy(nodes | views::values, back_inserter(nodeList));

	int64_t answer = 0;
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		for (size_t j = i + 1; j < nodeList.size(); j++)
		{
			answer += ((nodeList[i].Used) && (nodeList[i].Used <= nodeList[j].Avail) ? 1 : 0);
			answer += ((nodeList[j].Used) && (nodeList[j].Used <= nodeList[i].Avail) ? 1 : 0);
		}
	}

	printf("[2016] Puzzle22_A: %" PRId64 "\n", answer);
}


static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, Node> nodes = ParseNodes(input);

	Point2 emptyNode = (nodes | views::filter([](const auto& kvp) { return kvp.second.Used == 0; })).front().first;
	int64_t maximumX = ranges::max(nodes | views::keys | views::transform([](const Point2& p) { return p.X; }));
	int64_t maximumY = ranges::max(nodes | views::keys | views::transform([](const Point2& p) { return p.Y; }));
	Point2 goalData{ maximumX, 0 };

	int64_t emptyNodeHddSize = nodes.at(emptyNode).Size;

	ArrayMap2D grid({ 0, 0 }, maximumX + 1, maximumY + 1, '.');
	for (const auto& kvp : nodes)
	{
		if (kvp.second.Used > emptyNodeHddSize)
		{
			grid(kvp.first) = '#';
		}
	}

	int64_t answer = ShortestPathForGoalData(grid, emptyNode, goalData);

	printf("[2016] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2016(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
