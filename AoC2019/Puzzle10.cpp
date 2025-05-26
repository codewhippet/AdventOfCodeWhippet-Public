#include "stdafx.h"
#include <numbers>

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2019_Types
{
}

using namespace Puzzle10_2019_Types;

static Point2 Normalise(const Point2& dir)
{
	int64_t scaleFactor = gcd(dir.X, dir.Y);
	return Point2{ dir.X / scaleFactor, dir.Y / scaleFactor };
}

static double AngleFromDirection(const Point2& dir)
{
	double angle = atan2(dir.X, -dir.Y);
	return (angle >= 0.0) ? angle : (2.0 * numbers::pi + angle);
}

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap asteroidField = ReadPointMap(input);

	map<Point2, set<Point2>> seenFrom;
	for (const auto &asteroidA : asteroidField.Data)
	{
		for (const auto& asteroidB : asteroidField.Data)
		{
			if (asteroidA == asteroidB)
				continue;

			Point2 heading = Normalise(asteroidB.first - asteroidA.first);
			seenFrom[asteroidA.first].insert(heading);
		}
	}

	int64_t answer = ranges::max(seenFrom
		| views::transform([&](const auto& from)
			{
				return from.second.size();
			}));

	printf("[2019] Puzzle10_A: %" PRId64 "\n", answer);
}


static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap asteroidField = ReadPointMap(input);

	using distanceToTargets = map<int64_t, Point2>;
	using headingToDistanceToTargets = map<double, distanceToTargets>;

	map<Point2, headingToDistanceToTargets> targetsFrom;
	for (const auto& asteroidA : asteroidField.Data)
	{
		for (const auto& asteroidB : asteroidField.Data)
		{
			if (asteroidA == asteroidB)
				continue;

			Point2 heading = Normalise(asteroidB.first - asteroidA.first);
			double headingAngle = AngleFromDirection(heading);
			int64_t distanceToTarget = ManhattanDistance(asteroidA.first, asteroidB.first);

			targetsFrom[asteroidA.first][headingAngle][distanceToTarget] = asteroidB.first;
		}
	}

	auto bestAsteroid = max_element(targetsFrom.begin(), targetsFrom.end(),
		[](const auto& a, const auto& b)
		{
			return a.second.size() < b.second.size();
		});
	headingToDistanceToTargets targets = bestAsteroid->second;

	vector<Point2> destroyed;
	while (destroyed.size() < 200)
	{
		for (auto& targetsByDistance : targets)
		{
			if (targetsByDistance.second.empty() == false)
			{
				Point2 nextTarget = targetsByDistance.second.begin()->second;
				destroyed.push_back(nextTarget);
				targetsByDistance.second.erase(targetsByDistance.second.begin());
			}
		}
	}

	Point2 twoHundredth = destroyed[199];
	int64_t answer = twoHundredth.X * 100 + twoHundredth.Y;

	printf("[2019] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2019(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
