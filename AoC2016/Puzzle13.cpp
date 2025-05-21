#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2016_Types
{
}

using namespace Puzzle13_2016_Types;

static bool IsSpace(const int64_t magic, const Point2& pos)
{
	if ((pos.X < 0) || (pos.Y < 0))
	{
		return false;
	}

	uint64_t polynomial = (pos.X * pos.X) + (3 * pos.X) + (2 * pos.X * pos.Y) + (pos.Y) + (pos.Y * pos.Y);
	polynomial += magic;
	int bitsSet = popcount(polynomial);
	return (bitsSet % 2) == 0;
}

static int64_t FindShortestRoute(const int64_t magic, const Point2& start, const Point2& target)
{
	multiset<pair<int64_t, Point2>> openSet{ { 0, start } };
	unordered_map<Point2, Point2> cameFrom;
	unordered_map<Point2, int64_t> score;

	auto h = [&](const Point2& p) { return abs(target.X - p.X) + abs(target.Y - p.Y); };

	score[start] = 0;
	score[target] = numeric_limits<int64_t>::max();

	while (openSet.empty() == false)
	{
		Point2 current = openSet.begin()->second;
		openSet.erase(openSet.begin());

		if (current == target)
		{
			break;
		}

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = current + dir;
			if (IsSpace(magic, neighbour))
			{
				int64_t tentativeScore = score[current] + 1;

				auto neighbourScoreIt = score.find(neighbour);
				if ((neighbourScoreIt == score.end()) || (tentativeScore < neighbourScoreIt->second))
				{
					cameFrom[neighbour] = current;
					score[neighbour] = tentativeScore;
					int64_t searchPriority = tentativeScore + h(neighbour);
					openSet.insert({ searchPriority, neighbour });
				}
			}
		}
	}

	return score[target];
}

static size_t BFSToDepth(const int64_t magic, const Point2& start, const int64_t steps)
{
	vector<pair<int64_t, Point2>> searchQueue = { { 0, start } };

	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentLocation = searchQueue[i].second;
		int64_t currentSteps = searchQueue[i].first;
		if (currentSteps > steps)
		{
			break;
		}

		if (visited.insert(currentLocation).second == false)
		{
			continue;
		}

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = currentLocation + dir;
			if (IsSpace(magic, neighbour))
			{
				searchQueue.push_back({ currentSteps + 1, neighbour });
			}
		}
	}

	return visited.size();
}

static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Point2 start{ 1, 1 };
	const Point2 goal{ 31, 39 };

	string magicNumberInput;
	getline(input, magicNumberInput);

	int64_t magicNumber = atoll(magicNumberInput.c_str());

	assert(IsSpace(magicNumber, goal));

	int64_t answer = FindShortestRoute(magicNumber, start, goal);

	printf("[2016] Puzzle13_A: %" PRId64 "\n", answer);
}


static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Point2 start{ 1, 1 };

	string magicNumberInput;
	getline(input, magicNumberInput);

	int64_t magicNumber = atoll(magicNumberInput.c_str());

	int64_t answer = BFSToDepth(magicNumber, start, 50);

	printf("[2016] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2016(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
