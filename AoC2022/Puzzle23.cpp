#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2022_Types
{
	struct Elf
	{
		Point2 ProposedMove;
	};
}

using namespace Puzzle23_2022_Types;

static generator<Point2> SurroundingChecks(const Point2& pos)
{
	for (const Point2& dir : Point2::CardinalAndDiagonalDirections())
	{
		co_yield pos + dir;
	}
}

static generator<Point2> NorthChecks(const Point2& pos)
{
	co_yield pos + Point2::North();
	co_yield pos + Point2::NorthEast();
	co_yield pos + Point2::NorthWest();
}

static generator<Point2> SouthChecks(const Point2& pos)
{
	co_yield pos + Point2::South();
	co_yield pos + Point2::SouthEast();
	co_yield pos + Point2::SouthWest();
}

static generator<Point2> WestChecks(const Point2& pos)
{
	co_yield pos + Point2::West();
	co_yield pos + Point2::NorthWest();
	co_yield pos + Point2::SouthWest();
}

static generator<Point2> EastChecks(const Point2& pos)
{
	co_yield pos + Point2::East();
	co_yield pos + Point2::NorthEast();
	co_yield pos + Point2::SouthEast();
}

static Point2 TryNorth(const Point2& pos, const map<Point2, Elf>& elves)
{
	bool blocked = ranges::any_of(NorthChecks(pos), [&](const auto& p) { return elves.contains(p); });
	return blocked ? pos : pos + Point2::North();
}

static Point2 TrySouth(const Point2& pos, const map<Point2, Elf>& elves)
{
	bool blocked = ranges::any_of(SouthChecks(pos), [&](const auto& p) { return elves.contains(p); });
	return blocked ? pos : pos + Point2::South();
}

static Point2 TryWest(const Point2& pos, const map<Point2, Elf>& elves)
{
	bool blocked = ranges::any_of(WestChecks(pos), [&](const auto& p) { return elves.contains(p); });
	return blocked ? pos : pos + Point2::West();
}

static Point2 TryEast(const Point2& pos, const map<Point2, Elf>& elves)
{
	bool blocked = ranges::any_of(EastChecks(pos), [&](const auto& p) { return elves.contains(p); });
	return blocked ? pos : pos + Point2::East();
}

static const vector<function<Point2(const Point2&, const map<Point2, Elf>&)>> GetMoveProposals()
{
	static const vector<function<Point2(const Point2&, const map<Point2, Elf>&)>> moveProposals =
	{
		&TryNorth,
		&TrySouth,
		&TryWest,
		&TryEast,
	};
	return moveProposals;
}

static bool ProposeMoves(map<Point2, Elf>* elves, int currentFirstMove)
{
	map<Point2, int64_t> proposals;

	// Move proposals
	for (auto& elfPair : *elves)
	{
		// No nearby elves, we should stop
		bool noNearbyElves = ranges::all_of(SurroundingChecks(elfPair.first), [&](const auto& p) { return elves->contains(p) == false; });
		if (noNearbyElves)
		{
			continue;
		}

		// Want to move
		for (int i = 0; i < (int)GetMoveProposals().size(); i++)
		{
			Point2 proposedMove = GetMoveProposals()[(currentFirstMove + i) % 4](elfPair.first, *elves);
			if (proposedMove != elfPair.first)
			{
				proposals[proposedMove]++;
				elfPair.second.ProposedMove = proposedMove;
				break;
			}
		}
	}

	// Cancel duplicate proposals
	for (auto& elfPair : *elves)
	{
		if ((elfPair.second.ProposedMove != elfPair.first) && (proposals.at(elfPair.second.ProposedMove) > 1))
		{
			elfPair.second.ProposedMove = elfPair.first;
		}
	}

	return ranges::any_of(proposals, [](const auto& p) { return p.second == 1; });
}

static map<Point2, Elf> MoveElves(const map<Point2, Elf>& elves)
{
	return elves | views::transform([](const auto &p) { return pair<Point2, Elf>{ p.second.ProposedMove, p.second }; }) | ranges::to<map>();
}

static void DEBUG_PrintElves(const map<Point2, Elf>& elves)
{
	const auto [minX, maxX] = ranges::minmax(elves | views::keys | views::transform(&Point2::GetX));
	const auto [minY, maxY] = ranges::minmax(elves | views::keys | views::transform(&Point2::GetY));

	string s;
	for (int64_t y = minY; y < maxY + 1; y++)
	{
		for (int64_t x = minX; x < maxX + 1; x++)
		{
			s += elves.contains({ x, y }) ? '#' : '.';
		}
		s += '\n';
	}
	printf("%s\n", s.c_str());
}
auto faf = &DEBUG_PrintElves;

static void Puzzle23_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, Elf> elves = ReadPointMap(input, '.').Data
		| views::transform([](const auto &p) -> pair<Point2, Elf>
			{
				return { p.first, { p.first } };
			})
		| ranges::to<map>();

	for (int i = 0; i < 10; i++)
	{
		ProposeMoves(&elves, i % 4);
		elves = MoveElves(elves);
	}

	const auto [minX, maxX] = ranges::minmax(elves | views::keys | views::transform(&Point2::GetX));
	const auto [minY, maxY] = ranges::minmax(elves | views::keys | views::transform(&Point2::GetY));

	int64_t answer = ((maxX - minX + 1) * (maxY - minY + 1)) - elves.size();

	printf("[2022] Puzzle23_A: %" PRId64 "\n", answer);
}

static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, Elf> elves = ReadPointMap(input, '.').Data
		| views::transform([](const auto &p) -> pair<Point2, Elf>
			{
				return { p.first, { p.first } };
			})
		| ranges::to<map>();

	int64_t round = 0;
	while (true)
	{
		if (ProposeMoves(&elves, round % 4) == false)
		{
			break;
		}

		elves = MoveElves(elves);
		round++;
	}

	int64_t answer = round + 1;

	printf("[2022] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2022(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
