#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2022_Types
{
	struct Blizzard
	{
		Point2 InitialPosition;
		Point2 DirectionOfTravel;
	};

	struct Puzzle
	{
		ArrayMap2D Map;
		vector<Blizzard> Blizzards;
		Point2 Start;
		Point2 End;
	};

	struct PosTime
	{
		Point2 Pos;
		int64_t Time;

		auto operator<=>(const PosTime&) const = default;
	};

	struct PosTimeSnacks
	{
		Vector3 Pos;
		int64_t Time;

		auto operator<=>(const PosTimeSnacks&) const = default;
	};

}

using namespace Puzzle24_2022_Types;

static Puzzle ParsePuzzle(istream& input)
{
	Puzzle puzzle{ ReadArrayMap(input) };

	for (const auto& p : puzzle.Map.Grid())
	{
		switch (p.second)
		{
		case '^':
			puzzle.Blizzards.push_back({ p.first, Point2::Up() });
			break;
		case '>':
			puzzle.Blizzards.push_back({ p.first, Point2::Right() });
			break;
		case 'v':
			puzzle.Blizzards.push_back({ p.first, Point2::Down() });
			break;
		case '<':
			puzzle.Blizzards.push_back({ p.first, Point2::Left() });
			break;
		}
	}

	puzzle.Start = Point2{ 1, 0 };
	puzzle.End = Point2{ puzzle.Map.GetWidth() - 2, puzzle.Map.GetHeight() - 1 };

	return puzzle;
}

static generator<PosTime> NextMoves(const Puzzle &, const PosTime& current)
{
	co_yield PosTime{ current.Pos + Point2::Up(), current.Time + 1 };
	co_yield PosTime{ current.Pos + Point2::Down(), current.Time + 1 };
	co_yield PosTime{ current.Pos + Point2::Left(), current.Time + 1 };
	co_yield PosTime{ current.Pos + Point2::Right(), current.Time + 1 };
	co_yield PosTime{ current.Pos, current.Time + 1 };
}

static generator<PosTimeSnacks> NextMoves(const Puzzle &p, const PosTimeSnacks& current)
{
	Vector3 goingBackForSnacks;
	Vector3 returningWithSnacks;

	Point2 currentLocation{ current.Pos.X, current.Pos.Y };
	if (current.Pos.Z == 0)
	{
		// About to hit the end without snacks
		if (currentLocation == (p.End + Point2::Up()))
		{
			goingBackForSnacks = Vector3{ 0, 0, 1 };
		}
	}
	else if (current.Pos.Z == 1)
	{
		// Grabbing snacks
		if (currentLocation == (p.Start + Point2::Down()))
		{
			returningWithSnacks = Vector3{ 0, 0, 1 };
		}
	}

	co_yield PosTimeSnacks{ current.Pos + Vector3{ 0, 1, 0 } + goingBackForSnacks, current.Time + 1 };
	co_yield PosTimeSnacks{ current.Pos + Vector3{ 0, -1, 0 } + returningWithSnacks, current.Time + 1 };
	co_yield PosTimeSnacks{ current.Pos + Vector3{ -1, 0, 0 }, current.Time + 1 };
	co_yield PosTimeSnacks{ current.Pos + Vector3{ 1, 0, 0 }, current.Time + 1 };
	co_yield PosTimeSnacks{ current.Pos, current.Time + 1 };
}

static void SimulateBlizzards(const Puzzle &p, set<PosTime> *activeBlizzards, int64_t time)
{
	Point2 innerMapSize = p.Map.GetDimensions() - Point2{ 2, 2 };
	int64_t distanceTravelledX = (time % innerMapSize.X);
	int64_t distanceTravelledY = (time % innerMapSize.Y);

	ranges::copy(p.Blizzards
		| views::transform([&](const auto &b) -> PosTime
			{
				const Point2 unit{ 1, 1 };
				Point2 newPos = b.InitialPosition - unit + Point2{ b.DirectionOfTravel.X * distanceTravelledX, b.DirectionOfTravel.Y * distanceTravelledY };
				newPos.X = (newPos.X + innerMapSize.X) % innerMapSize.X;
				newPos.Y = (newPos.Y + innerMapSize.Y) % innerMapSize.Y;
				return { newPos + unit, time };
			}),
		inserter(*activeBlizzards, activeBlizzards->end()));
}

template <typename STATE>
static int64_t ShortestRoute(const Puzzle &p, const STATE& start, const STATE& end)
{
	int64_t simulatedTime = -1;
	set<PosTime> activeBlizzards;

	vector<STATE> searchQueue{ start };
	set<STATE> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		const STATE current = searchQueue[i];
		if (visited.insert(current).second == false)
			continue;

		if (current.Pos == end.Pos)
		{
			return current.Time;
		}

		// Simulate more blizzards if needed
		while (simulatedTime < (current.Time + 1))
		{
			SimulateBlizzards(p, &activeBlizzards, ++simulatedTime);
		}

		for (const STATE& next : NextMoves(p, current))
		{
			Point2 nextXY{ next.Pos.X, next.Pos.Y };
			if (p.Map.IsInside(nextXY)
				&& p.Map(nextXY) != '#'
				&& activeBlizzards.contains({ { next.Pos.X, next.Pos.Y }, next.Time }) == false)
			{
				searchQueue.push_back(next);
			}
		}
	}

	return -1;
}

static void Puzzle24_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);

	int64_t answer = ShortestRoute<PosTime>(puzzle, PosTime{ puzzle.Start, 0 }, PosTime{ puzzle.End, -1 });;

	printf("[2022] Puzzle24_A: %" PRId64 "\n", answer);
}

static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ParsePuzzle(input);

	int64_t answer = ShortestRoute<PosTimeSnacks>(puzzle,
		PosTimeSnacks{ { puzzle.Start.X, puzzle.Start.Y, 0 }, 0 },
		PosTimeSnacks{ { puzzle.End.X, puzzle.End.Y, 2 }, -1 });

	printf("[2022] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2022(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
