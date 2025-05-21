#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2023_Types
{
	static map<pair<Point2, char>, vector<Point2>> ReactionMap =
	{
		{ { Point2::Left(), '\\' }, { Point2::Up() } },
		{ { Point2::Left(), '/' }, { Point2::Down() } },
		{ { Point2::Left(), '|' }, { Point2::Up(), Point2::Down() } },
		{ { Point2::Left(), '-' }, { Point2::Left() } },

		{ { Point2::Right(), '\\' }, { Point2::Down() } },
		{ { Point2::Right(), '/' }, { Point2::Up() } },
		{ { Point2::Right(), '|' }, { Point2::Up(), Point2::Down() } },
		{ { Point2::Right(), '-' }, { Point2::Right() } },

		{ { Point2::Up(), '\\' }, { Point2::Left() } },
		{ { Point2::Up(), '/' }, { Point2::Right()} },
		{ { Point2::Up(), '|' }, { Point2::Up() } },
		{ { Point2::Up(), '-' }, { Point2::Left(), Point2::Right() } },

		{ { Point2::Down(), '\\' }, { Point2::Right() } },
		{ { Point2::Down(), '/' }, { Point2::Left() } },
		{ { Point2::Down(), '|' }, { Point2::Down() } },
		{ { Point2::Down(), '-' }, { Point2::Left(), Point2::Right() } },
	};
}

using namespace Puzzle16_2023_Types;

static void FireRay(const Point2& from, const Point2& direction, const PointMap& field, set<pair<Point2, Point2>>* visited, map<Point2, int>* tilesHit)
{
	Point2 p = from;
	while (IsOutside(p, field) == false)
	{
		pair<Point2, Point2> visitKey = make_pair(p, direction);
		if (visited->contains(visitKey))
		{
			return;
		}
		visited->insert(visitKey);
		(*tilesHit)[p]++;

		map<Point2, char>::const_iterator objectIt = field.Data.find(p);
		if (objectIt != field.Data.end())
		{
			char c = objectIt->second;
			for (const Point2& q : ReactionMap[{ direction, c }])
			{
				FireRay(p + q, q, field, visited, tilesHit);
			}
			return;
		}

		p = p + direction;
	}
}

static void Puzzle16_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap p = ReadPointMap(input);

	set<pair<Point2, Point2>> visited;
	map<Point2, int> tilesHit;
	FireRay({ 0, 0 }, Point2::Right(), p, &visited, &tilesHit);

	int64_t answer = tilesHit.size();

	printf("[2023] Puzzle16_A: %" PRId64 "\n", answer);
}

static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap p = ReadPointMap(input);

	vector<Point2> topEdge = Enumerable::Line({ 0, 0 }, { p.Size.X - 1, 0 })->ToVector();
	vector<Point2> bottomEdge = Enumerable::Line({ 0, p.Size.Y - 1 }, { p.Size.X - 1, p.Size.Y - 1 })->ToVector();
	vector<Point2> leftEdge = Enumerable::Line({ 0, 0 }, { 0, p.Size.Y - 1 })->ToVector();
	vector<Point2> rightEdge = Enumerable::Line({ 0, 0 }, { p.Size.X - 1, p.Size.Y - 1 })->ToVector();

	auto checkEdge = [&p](const vector<Point2>& edge, const Point2& direction) -> int64_t
		{
			int64_t maximum = 0;
			for (const Point2& start : edge)
			{
				set<pair<Point2, Point2>> visited;
				map<Point2, int> tilesHit;
				FireRay(start, direction, p, &visited, &tilesHit);
				maximum = max(maximum, (int64_t)tilesHit.size());
			}
			return maximum;
		};

	int64_t answer = 0;
	answer = max(answer, checkEdge(topEdge, Point2::Down()));
	answer = max(answer, checkEdge(bottomEdge, Point2::Up()));
	answer = max(answer, checkEdge(leftEdge, Point2::Right()));
	answer = max(answer, checkEdge(rightEdge, Point2::Left()));

	printf("[2023] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2023(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
