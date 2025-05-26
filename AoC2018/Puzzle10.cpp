#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2018_Types
{
	struct Particle
	{
		Point2 Position;
		Point2 Velocity;
	};
}

using namespace Puzzle10_2018_Types;

static vector<Particle> ReadParticles(istream& input)
{
	vector<Particle> particles;
	for (const smatch& m : ReadEachLine(input, regex{ R"(position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>)" }))
	{
		Particle p;
		p.Position.X = atoll(m[1].str().c_str());
		p.Position.Y = atoll(m[2].str().c_str());
		p.Velocity.X = atoll(m[3].str().c_str());
		p.Velocity.Y = atoll(m[4].str().c_str());
		particles.push_back(p);
	}
	return particles;
}

static vector<Point2> SimulateAt(const vector<Particle>& particles, int64_t time)
{
	vector<Point2> newPositions;
	newPositions.reserve(particles.size());
	ranges::copy(particles | views::transform([time](const Particle& p) { return p.Position + p.Velocity * time; }), back_inserter(newPositions));
	return newPositions;
}

static int64_t FindMinimumHeightTime(const vector<Particle>& particles)
{
	double totalTime = 0;
	int64_t totalCount = 0;
	for (const Particle& p : particles)
	{
		if (p.Velocity.X != 0)
		{
			totalTime += abs((double)p.Position.X / p.Velocity.X);
			totalCount++;
		}
		if (p.Velocity.Y != 0)
		{
			totalTime += abs((double)p.Position.Y / p.Velocity.Y);
			totalCount++;
		}
	}

	int64_t approximateTime = llround(totalTime / totalCount);
	int64_t searchBegin = approximateTime - 10;
	int64_t searchEnd = approximateTime + 10;

	int64_t minHeight = numeric_limits<int64_t>::max();
	int64_t minHeightTime = -1;
	for (int64_t i = searchBegin; i < searchEnd; i++)
	{
		vector<Point2> points = SimulateAt(particles, i);
		Point2 min = accumulate(points.begin(), points.end(), Point2::Max(), [](const Point2& a, const Point2& b) { return Point2::MinElements(a, b); });
		Point2 max = accumulate(points.begin(), points.end(), Point2::Min(), [](const Point2& a, const Point2& b) { return Point2::MaxElements(a, b); });

		int64_t height = max.Y - min.Y;
		if (height < minHeight)
		{
			minHeight = height;
			minHeightTime = i;
		}
	}

	return minHeightTime;
}

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Particle> particles = ReadParticles(input);
	int64_t minHeightTime = FindMinimumHeightTime(particles);

	vector<Point2> points = SimulateAt(particles, minHeightTime);
	Point2 min = accumulate(points.begin(), points.end(), Point2::Max(), [](const Point2& a, const Point2& b) { return Point2::MinElements(a, b); });
	Point2 max = accumulate(points.begin(), points.end(), Point2::Min(), [](const Point2& a, const Point2& b) { return Point2::MaxElements(a, b); });

	Point2 size = max - min + Point2{ 1, 1 };
	ArrayMap2D message(min, size.X, size.Y, '.');
	for (const Point2& p : points)
	{
		message(p) = '#';
	}

	printf("[2018] Puzzle10_A:\n");
	message.Print();
}


static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Particle> particles = ReadParticles(input);

	int64_t answer = FindMinimumHeightTime(particles);

	printf("[2018] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2018(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
