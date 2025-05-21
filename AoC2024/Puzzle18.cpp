#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2024_Types
{
}

using namespace Puzzle18_2024_Types;

static vector<Point2> ReadBytes(istream& input)
{
	vector<Point2> bytes;

	vector<string> lines = ReadAllLines(input);
	bytes.reserve(lines.size());
	for (size_t i = 0; i < lines.size(); i++)
	{
		Point2 byte;
		assert(sscanf(lines[i].c_str(), "%lld,%lld", &byte.X, &byte.Y) == 2);
		bytes.push_back(byte);
	}

	return bytes;
}

static int64_t StepsNeeded(const Point2& start, const Point2& end, const ArrayMap2D& room)
{
	vector<pair<Point2, int64_t>> searchQueue{ { start, 0 } };

	set<Point2> queued{ start };

	for (int i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentPosition = searchQueue[i].first;
		int64_t currentCost = searchQueue[i].second;

		if (currentPosition == end)
			return currentCost;

		for (const Point2 dir : Point2::CardinalDirections())
		{
			Point2 neighbour = currentPosition + dir;
			if ((room.IsInside(neighbour)) &&
				(room(neighbour) != '#') &&
				(queued.contains(neighbour) == false))
			{
				searchQueue.push_back({ neighbour, currentCost + 1 });
				queued.insert(neighbour);
			}
		}
	}

	return -1;
}

static void Puzzle18_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto bytes = ReadBytes(input);

	ArrayMap2D memory({ 0, 0 }, 71, 71, '.');
	for (int i = 0; i < 1024; i++)
	{
		assert(memory.IsInside(bytes[i]));
		memory(bytes[i]) = '#';
	}

	int64_t answer = StepsNeeded({ 0, 0 }, { 70, 70 }, memory);

	printf("[2024] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto bytes = ReadBytes(input);

	Point2 MazeEnd{ 70, 70 };

	size_t lower = 0;
	size_t upper = bytes.size();

	while (lower < upper)
	{
		size_t testPoint = lower + ((upper - lower) / 2);

		ArrayMap2D memory({ 0, 0 }, MazeEnd.X + 1, MazeEnd.Y + 1, '.');
		for (int i = 0; i < testPoint + 1; i++)
		{
			assert(memory.IsInside(bytes[i]));
			memory(bytes[i]) = '#';
		}

		int64_t pathLength = StepsNeeded({ 0, 0 }, MazeEnd, memory);
		if (pathLength == -1)
		{
			upper = testPoint;
		}
		else
		{
			lower = testPoint + 1;
		}
	}

	assert(upper == lower);
	Point2 answer = bytes[lower];

	printf("[2024] Puzzle18_B: %" PRId64 ",%" PRId64 "\n", answer.X, answer.Y);
}

void Puzzle18_2024(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
