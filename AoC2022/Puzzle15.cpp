#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2022_Types
{
	struct Sensor
	{
		Point2 Position;
		Point2 NearestBeacon;
		int64_t Range;
	};

	struct Range
	{
		int64_t First;
		int64_t Last;
	};

	template <typename CLASS, auto MEMBER>
	struct GetMember
	{
		auto operator()(const CLASS &c) const
		{
			return c.*MEMBER;
		}
	};
}

using namespace Puzzle15_2022_Types;

static vector<Sensor> CreateSensors(istream& input)
{
	vector<Sensor> sensors;

	const regex pattern{ R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))" };
	for (const auto& m : ReadEachLine(input, pattern))
	{
		Sensor s;
		s.Position = Point2{ Utils::ToNumber(m[1]), Utils::ToNumber(m[2]) };
		s.NearestBeacon = Point2{ Utils::ToNumber(m[3]), Utils::ToNumber(m[4]) };
		s.Range = ManhattanDistance(s.Position, s.NearestBeacon);

		sensors.push_back(s);
	}

	return sensors;
}

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

static vector<Range> SplitRanges(const vector<Range>& ranges, const Range& splitter)
{
	vector<Range> newRanges;

	for (Range r : ranges)
	{
		// Splitter fully contains this range
		if ((r.First >= splitter.First) && (r.Last <= splitter.Last))
		{
			continue;
		}

		// Splitter bisects this range
		if ((splitter.First > r.First) && (splitter.Last < r.Last))
		{
			Range left{ .First = r.First, .Last = splitter.First - 1 };
			Range right{ .First = splitter.Last + 1, .Last = r.Last };
			newRanges.push_back(left);
			newRanges.push_back(right);
			continue;
		}

		// Splitter chops off the left of the range
		if (splitter.Last < r.Last)
		{
			r.First = max(r.First, splitter.Last + 1);
		}

		// Splitter chops off the right of the range
		if (splitter.First > r.First)
		{
			r.Last = min(r.Last, splitter.First - 1);
		}

		newRanges.push_back(r);
	}

	return newRanges;
}

static bool SensorRangeAtRow(const Sensor& sensor, int64_t row, Range* range)
{
	if (abs(sensor.Position.Y - row) > sensor.Range)
	{
		return false;
	}

	*range = {};
	range->First = (sensor.Position.X - sensor.Range) + abs(sensor.Position.Y - row);
	range->Last = (sensor.Position.X + sensor.Range) - abs(sensor.Position.Y - row);

	return range->First <= range->Last;
}

static void Puzzle15_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Sensor> sensors = CreateSensors(input);

	set<Point2> beaconLocations = sensors | views::transform(GetMember<Sensor, &Sensor::NearestBeacon>{}) | ranges::to<set>();
	set<Point2> sensorLocations = sensors | views::transform(GetMember<Sensor, &Sensor::Position>{}) | ranges::to<set>();

	int64_t minX = ranges::min(sensors | views::transform([](const auto &s) { return s.Position.X - s.Range; }));
	int64_t maxX = ranges::max(sensors | views::transform([](const auto &s) { return s.Position.X + s.Range; }));

	const int64_t testLine = 2000000;

	int64_t answer = 0;
	for (Point2 candidatePos : RasteriseLine({ minX, testLine }, { maxX, testLine }))
	{
		// Ignore beacons we know about
		if (beaconLocations.contains(candidatePos))
		{
			continue;
		}

		// Can't overlap sensors
		if (sensorLocations.contains(candidatePos))
		{
			answer++;
			continue;
		}

		// Check to see if we're in range of any sensors
		bool withinRange = ranges::any_of(sensors,
			[&](const auto& s)
			{
				return ManhattanDistance(s.Position, candidatePos) <= s.Range;
			});
		if (withinRange)
		{
			answer++;
		}
	}

	printf("[2022] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Sensor> sensors = CreateSensors(input);

	Point2 beacon;
	for (int64_t y = 0; y < 4000000; y++)
	{
		vector<Range> validPositions;
		validPositions.push_back({ .First = 0, .Last = 4000000 });

		for (const Sensor& sensor : sensors)
		{
			Range sensorRange;
			if (SensorRangeAtRow(sensor, y, &sensorRange))
			{
				validPositions = SplitRanges(validPositions, sensorRange);
			}
		}

		if (validPositions.size() > 0)
		{
			beacon.X = validPositions[0].First;
			beacon.Y = y;
			break;
		}
	}

	int64_t answer = (beacon.X * 4000000) + beacon.Y;

	printf("[2022] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2022(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
