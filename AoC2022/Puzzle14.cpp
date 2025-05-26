#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2022_Types
{
}

using namespace Puzzle14_2022_Types;

static generator<Point2> RasteriseLine(Point2 start, const Point2& stop)
{
    Point2 diff;
    if (start.X > stop.X)
    {
		diff = Point2{ -1, 0 };
    }
    else if (start.X < stop.X)
    {
		diff = Point2{ 1, 0 };
    }
    else if (start.Y > stop.Y)
    {
		diff = Point2{ 0, -1 };
    }
    else if (start.Y < stop.Y)
    {
		diff = Point2{ 0, 1 };
    }

    while (start != stop)
    {
        co_yield start;
        start += diff;
    }

    co_yield stop;
}

static generator<Point2> SandMovement(const Point2& sand)
{
	co_yield sand - Point2{ 0, 1 };
	co_yield sand - Point2{ 0, 1 } - Point2{ 1, 0 };
	co_yield sand - Point2{ 0, 1 } + Point2{ 1, 0 };
}

static map<Point2, char> MakeStartingMap(istream &input)
{
	const regex coordsPattern{ R"((\d+),(\d+))" };

	map<Point2, char> startingMap;

	for (const string &line : ReadEachLine(input))
	{
		sregex_iterator coordsBegin{ line.begin(), line.end(), coordsPattern };
		sregex_iterator coordsEnd{};
		auto matches = ranges::subrange(coordsBegin, coordsEnd) | ranges::to<vector>();
		for (size_t i = 0; (i + 1) < matches.size(); i++)
		{
			const auto& m0 = matches[i + 0];
			const auto& m1 = matches[i + 1];

			Point2 start{ atoll(m0[1].str().c_str()), -atoll(m0[2].str().c_str()) };
			Point2 stop{ atoll(m1[1].str().c_str()), -atoll(m1[2].str().c_str()) };

			for (Point2 pixel : RasteriseLine(start, stop))
			{
				startingMap[pixel] = '#';
			}
		}
	}

	return startingMap;
}

static void Puzzle14_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, char> simulation = MakeStartingMap(input);

	bool halt = false;
	while (halt == false)
	{
		Point2 currentPos{ 500, 0 };
		while (true)
		{
			// Abyss
			if (currentPos.Y < -200)
			{
				halt = true;
				break;
			}

			Point2 nextPos = currentPos;
			for (Point2 candidate : SandMovement(currentPos))
			{
				if (!simulation.contains(candidate))
				{
					nextPos = candidate;
					break;
				}
			}

			// Come to rest
			if (nextPos == currentPos)
			{
				simulation.insert({ currentPos, 'o' });
				break;
			}

			// Move
			currentPos = nextPos;
		}
	}

	int64_t answer = ranges::count(simulation | views::values, 'o');

	printf("[2022] Puzzle14_A: %" PRId64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, char> simulation = MakeStartingMap(input);

	int64_t maxDepth = ranges::min(simulation | views::transform([](const auto &kvp) { return kvp.first.Y; })) - 2;

	const Point2 startingPoint{ 500, 0 };
	while (simulation.contains(startingPoint) == false)
	{
		Point2 currentPos = startingPoint;
		while (true)
		{
			// Infinite floor
			if (currentPos.Y == maxDepth + 1)
			{
				simulation.insert({ currentPos, 'o' });
				break;
			}

			Point2 nextPos = currentPos;
			for (Point2 candidate : SandMovement(currentPos))
			{
				if (!simulation.contains(candidate))
				{
					nextPos = candidate;
					break;
				}
			}

			// Come to rest
			if (nextPos == currentPos)
			{
				simulation.insert({ currentPos, 'o' });
				break;
			}

			// Move
			currentPos = nextPos;
		}
	}

	int64_t answer = ranges::count(simulation | views::values, 'o');

	printf("[2022] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2022(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
