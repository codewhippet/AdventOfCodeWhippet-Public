#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2024_Types
{
	struct Robot
	{
		Point2 Start;
		Point2 Velocity;
	};

	bool operator==(const Robot& r, const Point2& p)
	{
		return r.Start == p;
	}
}

using namespace Puzzle14_2024_Types;

static Point2 RobotAfter(const Robot& robot, int64_t seconds, int64_t fieldWidth, int64_t fieldHeight)
{
	Point2 pos = robot.Start + robot.Velocity * seconds;
	pos.X %= fieldWidth;
	pos.Y %= fieldHeight;
	return pos;
}

static set<Point2> RobotsAfter(const vector<Robot>& robots, int64_t seconds, int64_t fieldWidth, int64_t fieldHeight)
{
	set<Point2> robotsAfter;
	ranges::copy(robots
		| views::transform([&](const Robot& r)
			{
				return RobotAfter(r, seconds, fieldWidth, fieldHeight);
			}),
		inserter(robotsAfter, robotsAfter.end()));
	return robotsAfter;
}

static int64_t CalculateClumpiness(const set<Point2>& robots, int64_t clumpingFactor)
{
	map<Point2, int64_t> group;
	ranges::for_each(robots | views::transform([&](const Point2& p) { return Point2{ p.X / clumpingFactor, p.Y / clumpingFactor }; }),
		[&](const Point2& p)
		{
			group[p]++;
		});

	vector<int64_t> clumpCounts;
	clumpCounts.resize((clumpingFactor * clumpingFactor) + 1);
	ranges::for_each(group | views::values, [&](const auto& clump) { clumpCounts[clump]++; });

	int64_t clumpiness = 0;
	for (int64_t i = clumpingFactor; i < (int64_t)clumpCounts.size(); i++)
	{
		clumpiness += (i * clumpCounts[i]);
	}

	return clumpiness;
}

static void Puzzle14_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t fieldWidth = 101;
	int64_t fieldHeight = 103;
	int64_t elapsedTime = 100;

	vector<Robot> robots;
	ranges::copy(ReadAllLines(input)
		| views::transform([](const string& line)
			{
				Robot r;
				int scanned = sscanf(line.c_str(), "p=%lld,%lld v=%lld,%lld",
					&r.Start.X,
					&r.Start.Y,
					&r.Velocity.X,
					&r.Velocity.Y);
				assert(scanned == 4);
				(void)scanned;
				return r;
			})
		| views::transform([&](const Robot& r)
			{
				Robot newRobot = r;
				if (newRobot.Velocity.X < 0)
					newRobot.Velocity.X += fieldWidth;
				if (newRobot.Velocity.Y < 0)
					newRobot.Velocity.Y += fieldHeight;
				return newRobot;
			}),
		back_inserter(robots));

	vector<Point2> robotsAfter;
	ranges::copy(robots
		| views::transform([&](const Robot& r)
			{
				return RobotAfter(r, elapsedTime, fieldWidth, fieldHeight);
			}),
		back_inserter(robotsAfter));

	int64_t ignoreX = fieldWidth / 2;
	int64_t ignoreY = fieldHeight / 2;

	vector<Vector4> quadrantCounts;
	auto splitIntoQuadrants = robotsAfter
		| views::filter([&](const Point2& p)
			{
				return (p.X != ignoreX) && (p.Y != ignoreY);
			})
		| views::transform([&](const Point2& p)
			{
				int offset = 0;
				if (p.X > ignoreX)
					offset += 1;
				if (p.Y > ignoreY)
					offset += 2;

				Vector4 v;
				(&v.X)[offset]++;
				return v;
			});
	Vector4 quadrantCount = accumulate(splitIntoQuadrants.begin(), splitIntoQuadrants.end(), Vector4{});

	int64_t answer = quadrantCount.X * quadrantCount.Y * quadrantCount.Z * quadrantCount.W;

	printf("[2024] Puzzle14_A: %" PRId64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t fieldWidth = 101;
	int64_t fieldHeight = 103;

	vector<Robot> robots;
	ranges::copy(ReadAllLines(input)
		| views::transform([](const string& line)
			{
				Robot r;
				int scanned = sscanf(line.c_str(), "p=%lld,%lld v=%lld,%lld",
					&r.Start.X,
					&r.Start.Y,
					&r.Velocity.X,
					&r.Velocity.Y);
				assert(scanned == 4);
				(void)scanned;
				return r;
			})
		| views::transform([&](const Robot& r)
			{
				Robot newRobot = r;
				if (newRobot.Velocity.X < 0)
					newRobot.Velocity.X += fieldWidth;
				if (newRobot.Velocity.Y < 0)
					newRobot.Velocity.Y += fieldHeight;
				return newRobot;
			}),
		back_inserter(robots));


	int64_t maximumFrame = 0;
	int64_t maximumClumpiness = 0;
	for (int64_t i = 0; i < 20000; i++)
	{
		set<Point2> robotsAfter = RobotsAfter(robots, i, fieldWidth, fieldHeight);
		int64_t clumpiness = CalculateClumpiness(robotsAfter, 4);
		if (clumpiness > maximumClumpiness)
		{
			maximumFrame = i;
			maximumClumpiness = clumpiness;
		}
	}

	int64_t answer = maximumFrame;

	printf("[2024] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2024(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
