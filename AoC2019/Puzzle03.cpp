#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2019_Types
{
}

using namespace Puzzle03_2019_Types;

static map<Point2, int64_t> WireDistances(string path)
{
	const map<char, Point2> directions = { { 'U', Point2::Up() }, { 'D', Point2::Down() }, { 'L', Point2::Left() }, { 'R', Point2::Right() } };

	Point2 pos;
	int64_t steps = 0;
	map<Point2, int64_t> visited{};
	for (char* move = strtok(path.data(), ","); move != nullptr; move = strtok(nullptr, ","))
	{
		Point2 dir = directions.at(move[0]);
		for (int64_t i : views::iota(0, atoll(move + 1)))
		{
			(void)i;
			pos = pos + dir;
			visited.insert({ pos, ++steps });
		}
	}
	return visited;
}

static void Puzzle03_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, int64_t> wireA = WireDistances(ReadSingleLine(input));
	map<Point2, int64_t> wireB = WireDistances(ReadSingleLine(input));

	set<Point2> intersections;
	ranges::set_intersection(wireA | views::keys, wireB | views::keys, inserter(intersections, intersections.end()));
	intersections.erase(Point2{});

	int64_t answer = ranges::min(intersections | views::transform([&](const Point2& p) { return abs(p.X) + abs(p.Y); }));

	printf("[2019] Puzzle03_A: %" PRId64 "\n", answer);
}


static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, int64_t> wireA = WireDistances(ReadSingleLine(input));
	map<Point2, int64_t> wireB = WireDistances(ReadSingleLine(input));

	set<Point2> intersections;
	ranges::set_intersection(wireA | views::keys, wireB | views::keys, inserter(intersections, intersections.end()));
	intersections.erase(Point2{});

	int64_t answer = ranges::min(intersections | views::transform([&](const Point2& p) { return wireA.at(p) + wireB.at(p); }));

	printf("[2019] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2019(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
