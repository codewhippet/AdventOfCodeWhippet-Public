#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2024_Types
{
	static const vector<Point2> Directions =
	{
		Point2::North(),
		Point2::East(),
		Point2::South(),
		Point2::West()
	};
}

using namespace Puzzle10_2024_Types;

static int64_t ScoreTrailhead(const Point2& start, const ArrayMap2D& mountain)
{
	int64_t score = 0;

	vector<Point2> searchQueue;
	searchQueue.push_back(start);

	set<Point2> visited;

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 p = searchQueue[i];
		if (visited.contains(p))
			continue;

		visited.insert(p);

		if (mountain(p) == '9')
		{
			score++;
			continue;
		}

		char currentHeight = mountain(p);
		for (const Point2& dir : Directions)
		{
			Point2 next = p + dir;
			if (mountain.IsInside(next) && (mountain(next) == currentHeight + 1))
			{
				searchQueue.push_back(next);
			}
		}
	}

	return score;
}

static int64_t ScoreTrailhead2(const Point2& start, const ArrayMap2D& mountain)
{
	if (mountain(start) == '9')
		return 1;

	int64_t score = 0;
	char currentHeight = mountain(start);
	for (Point2 dir : Directions)
	{
		Point2 next = start + dir;
		if (mountain.IsInside(next) && (mountain(next) == currentHeight + 1))
		{
			score += ScoreTrailhead2(next, mountain);
		}
	}
	return score;
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D mountain = ReadArrayMap(input);

	auto trailScores = mountain.Grid()
		| views::filter([](const auto& p) { return p.second == '0'; })
		| views::transform([&](const auto& p) { return ScoreTrailhead(p.first, mountain); });
	int64_t answer = accumulate(trailScores.begin(), trailScores.end(), 0ll);

	printf("[2024] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D mountain = ReadArrayMap(input);

	auto trailScores = mountain.Grid()
		| views::filter([](const auto& p) { return p.second == '0'; })
		| views::transform([&](const auto& p) { return ScoreTrailhead2(p.first, mountain); });
	int64_t answer = accumulate(trailScores.begin(), trailScores.end(), 0ll);

	printf("[2024] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2024(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
