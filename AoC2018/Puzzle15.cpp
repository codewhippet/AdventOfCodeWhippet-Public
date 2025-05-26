#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2018_Types
{
	enum class UnitType
	{
		Dead,
		Elf,
		Goblin,
	};

	struct Unit
	{
		UnitType Type = UnitType::Dead;
		int64_t AttackPower = -1;
		int64_t HP = -1;
	};

	struct ReadingOrder
	{
		bool operator()(const Point2& a, const Point2& b) const
		{
			return (a.Y == b.Y) ? a.X < b.X : a.Y < b.Y;
		}
	};

	using UnitMap = map<Point2, shared_ptr<Unit>, ReadingOrder>;

	struct CombatArena
	{
		ArrayMap2D Terrain;
		UnitMap Units;
	};

	struct Turn
	{
		pair<Point2, shared_ptr<Unit>> ActiveUnit;
		UnitType ActiveEnemy;
		char EnemySymbol;
		set<Point2, ReadingOrder> SquaresInRange;
		Point2 ChosenSquare;
		set<Point2, ReadingOrder> PossibleSteps;
		Point2 ChosenStep;
	};
}

using namespace Puzzle15_2018_Types;

static void PopulateUnits(CombatArena* arena)
{
	const Unit elf{ UnitType::Elf, 3, 200 };
	const Unit goblin{ UnitType::Goblin, 3, 200 };

	for (const auto& p : arena->Terrain.Grid())
	{
		if (p.second == 'E')
		{
			arena->Units.insert({ p.first, make_shared<Unit>(elf) });
		}
		else if (p.second == 'G')
		{
			arena->Units.insert({ p.first, make_shared<Unit>(goblin) });
		}
	}
}

static CombatArena ParseArena(istream& input)
{
	CombatArena arena{ ReadArrayMap(input) };
	PopulateUnits(&arena);
	return arena;
}

static CombatArena CloneArena(const CombatArena& original)
{
	CombatArena cloned{ original.Terrain };
	PopulateUnits(&cloned);
	return cloned;
}

static bool Pathfind(const CombatArena& arena, const Point2& startingPoint, const set<Point2, ReadingOrder>& endpoints, Point2 *endpointChosen)
{
	deque<pair<int64_t, Point2>> searchQueue{ { 0, startingPoint } };

	set<Point2, ReadingOrder> nearestEndpoints;

	int64_t shortestPath = numeric_limits<int64_t>::max();
	set<Point2> visited;
	while (searchQueue.empty() == false)
	{
		pair<int64_t, Point2> currentNode = searchQueue.front();
		searchQueue.pop_front();
		if (currentNode.first > shortestPath)
			break;

		if (visited.insert(currentNode.second).second == false)
		{
			continue;
		}

		if (endpoints.contains(currentNode.second))
		{
			shortestPath = currentNode.first;
			nearestEndpoints.insert(currentNode.second);
			continue;
		}

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 nextPos = currentNode.second + dir;
			if (arena.Terrain(nextPos) == '.')
			{
				searchQueue.push_back({ currentNode.first + 1, nextPos });
			}
		}
	}

	if (!nearestEndpoints.empty())
	{
		*endpointChosen = *nearestEndpoints.begin();
		return true;
	}

	return false;
}

static void FindSurroundingSquares(const CombatArena& arena, const Point2& origin, char lookingFor, set<Point2, ReadingOrder> *squares)
{
	for (const Point2& dir : Point2::CardinalDirections())
	{
		Point2 adjacent = origin + dir;
		if (arena.Terrain(adjacent) == lookingFor)
		{
			squares->insert(adjacent);
		}
	}
}

static bool FindSquaresInRange(const CombatArena& arena, Turn *turn)
{
	ranges::for_each(arena.Units | views::filter([&](const auto& unit) { return unit.second->Type == turn->ActiveEnemy; }),
		[&](const auto& unit)
		{
			FindSurroundingSquares(arena, unit.first, '.', &turn->SquaresInRange);
		});
	return turn->SquaresInRange.empty() == false;
}

static bool NeedToMove(const CombatArena& arena, Turn* turn)
{
	return ranges::none_of(Point2::CardinalDirections() | views::transform([&](const Point2& p) { return turn->ActiveUnit.first + p; }),
		[&](const Point2& p)
		{
			return arena.Terrain(p) == turn->EnemySymbol;
		});
}

static bool FindMove(const CombatArena& arena, Turn* turn)
{
	return FindSquaresInRange(arena, turn) && Pathfind(arena, turn->ActiveUnit.first, turn->SquaresInRange, &turn->ChosenSquare);
}

static void Move(CombatArena *arena, Turn* turn)
{
	FindSurroundingSquares(*arena, turn->ActiveUnit.first, '.', &turn->PossibleSteps);
	assert(turn->PossibleSteps.empty() == false);

	bool foundPath = Pathfind(*arena, turn->ChosenSquare, turn->PossibleSteps, &turn->ChosenStep);
	assert(foundPath);
	(void)foundPath;

	arena->Terrain(turn->ActiveUnit.first) = '.';
	arena->Terrain(turn->ChosenStep) = turn->ActiveUnit.second->Type == UnitType::Elf ? 'E' : 'G';

	arena->Units.erase(turn->ActiveUnit.first);
	arena->Units[turn->ChosenStep] = turn->ActiveUnit.second; // Force an overwrite, potentially clearing out dead bodies

	turn->ActiveUnit.first = turn->ChosenStep;
}

static bool CanAttack(const CombatArena& arena, Turn* turn)
{
	return ranges::any_of(Point2::CardinalDirections() | views::transform([&](const Point2& p) { return turn->ActiveUnit.first + p; }),
		[&](const Point2& p)
		{
			return arena.Terrain(p) == turn->EnemySymbol;
		});
}

static void Attack(CombatArena *arena, Turn* turn)
{
	set<Point2, ReadingOrder> surroundingEnemySquares;
	FindSurroundingSquares(*arena, turn->ActiveUnit.first, turn->EnemySymbol, &surroundingEnemySquares);
	assert(surroundingEnemySquares.empty() == false);

	pair<Point2, shared_ptr<Unit>> enemyToAttack;
	int64_t lowestHp = numeric_limits<int64_t>::max();
	for (const auto& enemySquare : surroundingEnemySquares)
	{
		pair<Point2, shared_ptr<Unit>> candidateEnemy = *arena->Units.find(enemySquare);
		if (candidateEnemy.second->HP < lowestHp)
		{
			enemyToAttack = candidateEnemy;
			lowestHp = enemyToAttack.second->HP;
		}
	}
	assert((bool)enemyToAttack.second);

	enemyToAttack.second->HP -= turn->ActiveUnit.second->AttackPower;
	if (enemyToAttack.second->HP <= 0)
	{
		enemyToAttack.second->Type = UnitType::Dead;
		arena->Terrain(enemyToAttack.first) = '.';
	}
}

static bool NoMoreTargets(const CombatArena& arena, Turn* turn)
{
	return ranges::count_if(arena.Units, [&](const auto& u) { return u.second->Type == turn->ActiveEnemy; }) == 0;
}

static void SetAttackPowerForElves(CombatArena* arena, int64_t attackPower)
{
	for (auto& unit : arena->Units)
	{
		if (unit.second->Type == UnitType::Elf)
		{
			unit.second->AttackPower = attackPower;
		}
	}
}

static int64_t RunSimulation(CombatArena* arena)
{
	int64_t round = 0;
	while (true)
	{
		vector<pair<Point2, shared_ptr<Unit>>> roundOrder;
		ranges::copy(arena->Units, back_inserter(roundOrder));

		for (const auto& unit : roundOrder)
		{
			if (unit.second->Type == UnitType::Dead)
				continue;

			Turn turn;
			turn.ActiveUnit = unit;
			turn.ActiveEnemy = turn.ActiveUnit.second->Type == UnitType::Elf ? UnitType::Goblin : UnitType::Elf;
			turn.EnemySymbol = turn.ActiveEnemy == UnitType::Elf ? 'E' : 'G';

			if (NoMoreTargets(*arena, &turn))
			{
				return round;
			}

			if (NeedToMove(*arena, &turn) && FindMove(*arena, &turn))
			{
				Move(arena, &turn);
			}
			if (CanAttack(*arena, &turn))
			{
				Attack(arena, &turn);
			}
		}

		round++;
	}

	return -1;
}

static pair<int64_t, int64_t> GetCombatOutcome(const CombatArena startingArena, int64_t attackPower)
{
	CombatArena arena = CloneArena(startingArena);
	SetAttackPowerForElves(&arena, attackPower);

	int64_t fullRounds = RunSimulation(&arena);

	auto aliveUnitHp = arena.Units
		| views::filter([](const auto& unit) { return unit.second->Type != UnitType::Dead; })
		| views::transform([](const auto& unit) { return unit.second->HP; });
	int64_t hitpointsRemaining = accumulate(aliveUnitHp.begin(), aliveUnitHp.end(), 0ll);

	return { fullRounds * hitpointsRemaining, ranges::count_if(arena.Units, [](const auto& unit) { return unit.second->Type == UnitType::Elf; }) };
}

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	CombatArena world = ParseArena(input);
	auto [outcome, elvesAlive] = GetCombatOutcome(world, 3);

	int64_t answer = outcome;

	printf("[2018] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const CombatArena startingWorld = ParseArena(input);
	const int64_t startingElves = ranges::count_if(startingWorld.Units, [](const auto& unit) { return unit.second->Type == UnitType::Elf; });

	int64_t answer = 0;

	int64_t lowerAttackPower = 4;
	int64_t upperAttackPower = 100 + 1; // No point going beyond insta-kill
	while (true)
	{
		int64_t midAttackPower = (lowerAttackPower + upperAttackPower) / 2;
		auto outcomeAtMid = GetCombatOutcome(startingWorld, midAttackPower);
		auto outcomeAfterMid = GetCombatOutcome(startingWorld, midAttackPower + 1);

		if ((outcomeAtMid.second < startingElves) && (outcomeAfterMid.second == startingElves))
		{
			answer = outcomeAfterMid.first;
			break;
		}

		if (outcomeAtMid.second < startingElves)
		{
			lowerAttackPower = midAttackPower;
		}
		else
		{
			upperAttackPower = midAttackPower;
		}
	}

	printf("[2018] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2018(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
