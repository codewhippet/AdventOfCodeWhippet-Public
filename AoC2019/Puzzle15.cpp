#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2019_Types
{
	struct SearchNode
	{
		pair<Point2, int64_t> Movement;
		int64_t Steps;
		int64_t From;
	};
}

using namespace Puzzle15_2019_Types;

static vector<pair<Point2, int64_t>> FindNearest(map<Point2, char>& maze, const Point2& start, char target)
{
	static const vector<pair<Point2, int64_t>> directions =
	{
		{ Point2::North(), 1 },
		{ Point2::South(), 2 },
		{ Point2::West(), 3 },
		{ Point2::East(), 4 },
	};

	vector<SearchNode> searchQueue{ { { start, 0 }, -1 } };
	set<Point2> visited;
	for (int64_t searchIndex = 0; searchIndex < (int64_t)searchQueue.size(); searchIndex++)
	{
		SearchNode current = searchQueue[searchIndex];
		if (maze[current.Movement.first] == target)
		{
			vector<pair<Point2, int64_t>> reconstructedPath;
			for (int64_t node = searchIndex; node != 0; node = searchQueue[node].From)
			{
				reconstructedPath.push_back(searchQueue[node].Movement);
			}
			ranges::reverse(reconstructedPath);
			return reconstructedPath;
		}

		if (visited.insert(current.Movement.first).second == false)
			continue;

		for (const auto& dir : directions)
		{
			Point2 nextPos = current.Movement.first + dir.first;
			if (maze[nextPos] != '#')
			{
				searchQueue.push_back({ { nextPos, dir.second }, current.Steps + 1, searchIndex });
			}
		}
	}

	return {};
}

static map<Point2, char> ExploreMaze(istream &input)
{
	Intputer puter(input);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	map<Point2, char> maze{ { {}, '.'} };
	Point2 droidPos;
	Point2 oxygenSystem;
	while (true)
	{
		vector<pair<Point2, int64_t>> movements = FindNearest(maze, droidPos, '\0');
		if (movements.empty())
			break;

		ranges::for_each(movements | views::take(movements.size() - 1),
			[&](const auto& move)
			{
				in.push_back(move.second);
				auto exec = puter.Execute();
				(void)exec;
				assert(exec == Intputer::ExecutionResult::PendingIo);
				assert(out.size() == 1);
				assert(out[0] == 1);
				out.clear();

				droidPos = move.first;
			});

		in.push_back(movements.back().second);
		auto exec = puter.Execute();
		assert(exec == Intputer::ExecutionResult::PendingIo);
		(void)exec;
		assert(out.size() == 1);
		switch (out[0])
		{
		case 0:
			maze[movements.back().first] = '#';
			break;
		case 1:
			maze[movements.back().first] = '.';
			droidPos = movements.back().first;
			break;
		case 2:
			maze[movements.back().first] = 'O';
			droidPos = movements.back().first;
			break;
		}
		out.clear();
	}

	return maze;
}

static int64_t TimeToFill(map<Point2, char>& maze, const Point2& start)
{
	vector<pair<Point2, int64_t>> searchQueue{ { start, 0 } };
	set<Point2> queued{ start };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPos, currentSteps] = searchQueue[i];
		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 nextPos = currentPos + dir;
			if ((maze.at(nextPos) != '#') && (queued.contains(nextPos) == false))
			{
				searchQueue.push_back({ nextPos, currentSteps + 1 });
				queued.insert(nextPos);
			}
		}
	}
	return searchQueue.back().second;
}

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, char> maze = ExploreMaze(input);

	auto shortestRoute = FindNearest(maze, {}, 'O');
	int64_t answer = shortestRoute.size();

	printf("[2019] Puzzle15_A: %" PRId64 "\n", answer);
}


static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, char> maze = ExploreMaze(input);

	Point2 oxygenBottle = ranges::find_if(maze, [](const auto& p) { return p.second == 'O'; })->first;
	int64_t answer = TimeToFill(maze, oxygenBottle);

	printf("[2019] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2019(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
