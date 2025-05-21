#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2021_Types
{
}

using namespace Puzzle17_2021_Types;

static pair<Point2, Point2> ReadTargetArea(const string& description)
{
	int x1, x2, y1, y2;
	int parsed = sscanf(description.c_str(),
		"target area: x=%d..%d, y=%d..%d",
		&x1, &x2, &y1, &y2);
	assert(parsed == 4);
	(void)parsed;

	return
	{
		{ max(x1, x2), max(y1, y2) },
		{ min(x1, x2), min(y1, y2) },
	};
}

static int TriangleNumber(int n)
{
	return (n * (n + 1)) / 2;
}

static bool HasHitTarget(const Point2& probe, const pair<Point2, Point2>& target)
{
	return
		(probe.X <= target.first.X) &&
		(probe.X >= target.second.X) &&
		(probe.Y <= target.first.Y) &&
		(probe.Y >= target.second.Y);
}

static bool DoesHitTarget(Point2 velocity, const pair<Point2, Point2>& target)
{
	Point2 probe{ 0, 0 };

	while (true)
	{
		if (HasHitTarget(probe, target))
			return true;

		if (probe.Y < target.second.Y)
			return false;

		// Move
		probe = probe + velocity;

		// Drag
		if (velocity.X != 0)
		{
			velocity.X += velocity.X > 0 ? -1 : 1;
		}

		// Gravity
		velocity.Y--;
	}

	return false;
}

static void Puzzle17_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string targetDescription;
	getline(input, targetDescription);
	pair<Point2, Point2> target = ReadTargetArea(targetDescription);

	int64_t answer = TriangleNumber((int)abs(target.second.Y) - 1);

	printf("[2021] Puzzle17_A: %" PRId64 "\n", answer);
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string targetDescription;
	getline(input, targetDescription);
	pair<Point2, Point2> target = ReadTargetArea(targetDescription);

	// Assume the target is down and to the right

	int maximumY = (int)abs(target.second.Y) - 1;
	int minimumY = (int)target.second.Y;

	int maximumX = (int)target.first.X;
	int minimumX = 1;

	int hits = 0;
	for (int y = minimumY; y <= maximumY; y++)
	{
		for (int x = minimumX; x <= maximumX; x++)
		{
			if (DoesHitTarget({ x, y }, target))
				hits++;
		}
	}

	int64_t answer = hits;

	printf("[2021] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2021(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
