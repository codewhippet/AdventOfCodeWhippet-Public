#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2024_Types
{
	struct Puzzle
	{
		Point2 Start;
		Point2 End;

		set<Vector4> Nodes;
	};

	struct PartialPath
	{
		Vector4 Node;
		vector<Point2> Path;
	};
}

using namespace Puzzle16_2024_Types;


static Vector4 MakeState(const Point2& p, const Point2& d)
{
	return { p.X, p.Y, d.X, d.Y };
}

static Point2 GetPos(const Vector4& v)
{
	return { v.X, v.Y };
}

static Point2 GetDir(const Vector4& v)
{
	return { v.Z, v.W };
}

static Puzzle MakePuzzle(const ArrayMap2D& board)
{
	Puzzle p;

	for (const auto& kvp : board.Grid())
	{
		switch (kvp.second)
		{
		case '#':
			continue;

		case 'S':
			p.Start = kvp.first;
			break;

		case 'E':
			p.End = kvp.first;
			break;
		}

		for (const auto& dir : Point2::CardinalDirections())
		{
			p.Nodes.insert(MakeState(kvp.first, dir));
		}
	}

	return p;
}

static int64_t FindBestPath(const Puzzle& p)
{
	multimap<int64_t, Vector4> searchQueue;
	searchQueue.insert({ 0, { p.Start.X, p.Start.Y, Point2::East().X, Point2::East().Y} });

	map<Vector4, int64_t> visited;
	while (searchQueue.empty() == false)
	{
		pair<int64_t, Vector4> current = *searchQueue.begin();
		searchQueue.erase(searchQueue.begin());

		// Have we been here before at a lower cost?
		map<Vector4, int64_t>::const_iterator prev = visited.find(current.second);
		if (prev != visited.end() && prev->second <= current.first)
			continue;

		if (GetPos(current.second) == p.End)
			return current.first;

		visited.insert({ current.second, current.first });

		Point2 currentPos = GetPos(current.second);
		Point2 currentDir = GetDir(current.second);

		Vector4 moveForward = MakeState(currentPos + currentDir, currentDir);
		if (p.Nodes.contains(moveForward))
		{
			searchQueue.insert({ current.first + 1, moveForward });
		}

		Vector4 turnRight = MakeState(currentPos, Point2::RotateClockwise(currentDir));
		searchQueue.insert({ current.first + 1000, turnRight });

		Vector4 turnLeft = MakeState(currentPos, Point2::RotateAnticlockwise(currentDir));
		searchQueue.insert({ current.first + 1000, turnLeft });
	}

	assert(false);
	return -1;
}

static bool HaveBeenHereBeforeAtLowerCost(const Vector4& state, int64_t cost, const map<Vector4, int64_t>& visited)
{
	map<Vector4, int64_t>::const_iterator prev = visited.find(state);
	return (prev != visited.end()) && (prev->second < cost);
}

static vector<vector<Point2>> FindAllBestPaths(const Puzzle& p)
{
	vector<vector<Point2>> bestPaths;

	multimap<int64_t, PartialPath> searchQueue;
	Vector4 startingState = MakeState(p.Start, Point2::East());
	searchQueue.insert({ 0, { startingState, { GetPos(startingState) } } });

	int64_t bestPathLength = 100000;

	map<Vector4, int64_t> visited;
	while (searchQueue.empty() == false)
	{
		pair<int64_t, PartialPath> current = *searchQueue.begin();
		searchQueue.erase(searchQueue.begin());

		if (HaveBeenHereBeforeAtLowerCost(current.second.Node, current.first, visited))
			continue;

		if (current.first > bestPathLength)
			break;

		if (GetPos(current.second.Node) == p.End)
		{
			bestPaths.push_back(current.second.Path);
			bestPathLength = min(current.first, bestPathLength);
		}

		visited.insert({ current.second.Node, current.first });

		Point2 currentPos = GetPos(current.second.Node);
		Point2 currentDir = GetDir(current.second.Node);

		{
			Vector4 moveForward = MakeState(currentPos + currentDir, currentDir);
			if (p.Nodes.contains(moveForward))
			{
				PartialPath nextState{ moveForward, current.second.Path };
				nextState.Path.push_back(GetPos(moveForward));
				searchQueue.insert({ current.first + 1, nextState });
			}
		}

		{
			Vector4 turnRight = MakeState(currentPos, Point2::RotateClockwise(currentDir));
			PartialPath nextState{ turnRight, current.second.Path };
			searchQueue.insert({ current.first + 1000, nextState });
		}

		{
			Vector4 turnLeft = MakeState(currentPos, Point2::RotateAnticlockwise(currentDir));
			PartialPath nextState{ turnLeft, current.second.Path };
			searchQueue.insert({ current.first + 1000, nextState });
		}
	}

	return bestPaths;
}

static void Puzzle16_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = MakePuzzle(ReadArrayMap(input));

	int64_t answer = FindBestPath(p);

	printf("[2024] Puzzle16_A: %" PRId64 "\n", answer);
}

static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = MakePuzzle(ReadArrayMap(input));

	auto bestPaths = FindAllBestPaths(puzzle);

	set<Point2> locations;
	for (const auto& p : bestPaths)
	{
		for (const auto& s : p)
		{
			locations.insert(s);
		}
	}

	int64_t answer = locations.size();

	printf("[2024] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2024(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
