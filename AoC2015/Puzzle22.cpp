#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2015_Types
{
	enum EffectType
	{
		Shield,
		Poison,
		Recharge,

		Count
	};

	struct Fighter
	{
		int64_t HitPoints = 0;
		int64_t Mana = 0;
		int64_t Damage = 0;
		int64_t Armour = 0;
		int64_t EffectTimers[EffectType::Count] = { 0 };
	};

	enum class Difficulty
	{
		Easy,
		Hard
	};
}

using namespace Puzzle22_2015_Types;

static Fighter ParseBoss(istream& input)
{
	vector<string> lines = ReadAllLines(input);
	assert(lines.size() == 2);

	Fighter boss;

	int scanned = 0;
	scanned += sscanf(lines[0].c_str(), "Hit Points: %lld", &boss.HitPoints);
	scanned += sscanf(lines[1].c_str(), "Damage: %lld", &boss.Damage);
	assert(scanned == 2);
	(void)scanned;

	return boss;
}

static Fighter CreatePlayer()
{
	Fighter player;
	player.HitPoints = 50;
	player.Mana = 500;
	return player;
}

static int64_t MinimumManaForWin(vector<Fighter> fighters, int64_t activePlayer, Difficulty difficulty, int64_t currentManaSpend, int64_t *globalManaMinimum)
{
	assert((activePlayer == 0) || (activePlayer == 1));

	int64_t minimumMana = numeric_limits<int64_t>::max();

	// If we've already exceeded the known global minimum, then bail out
	if (currentManaSpend > *globalManaMinimum)
	{
		return minimumMana;
	}

	// Have we killed the boss?
	if (fighters[1].HitPoints <= 0)
	{
		minimumMana = currentManaSpend;
		if (currentManaSpend < *globalManaMinimum)
		{
			*globalManaMinimum = currentManaSpend;
		}
		return minimumMana;
	}

	// At the start of each player turn (before any other effects apply), you lose 1 hit point.
	if ((activePlayer == 0) && (difficulty == Difficulty::Hard))
	{
		fighters[0].HitPoints -= 1;
	}

	// Have we died?
	if (fighters[0].HitPoints <= 0)
	{
		return minimumMana;
	}

	// Apply effects
	if (fighters[0].EffectTimers[EffectType::Shield] > 0)
	{
		fighters[0].Armour = 7;
		fighters[0].EffectTimers[EffectType::Shield]--;
	}
	else
	{
		fighters[0].Armour = 0;
	}

	if (fighters[0].EffectTimers[EffectType::Poison] > 0)
	{
		fighters[1].HitPoints -= 3;
		fighters[0].EffectTimers[EffectType::Poison]--;
	}

	if (fighters[0].EffectTimers[EffectType::Recharge] > 0)
	{
		fighters[0].Mana += 101;
		fighters[0].EffectTimers[EffectType::Recharge]--;
	}

	// Have we killed the boss (second check because poison might have killed him)?
	if (fighters[1].HitPoints <= 0)
	{
		minimumMana = currentManaSpend;
		if (currentManaSpend < *globalManaMinimum)
		{
			*globalManaMinimum = currentManaSpend;
		}
		return minimumMana;
	}

	// Player: Cast spell
	if (activePlayer == 0)
	{
		// We need to be able to cast at least one spell
		if (fighters[0].Mana < 53)
		{
			return minimumMana;
		}

		// Magic Missile costs 53 mana. It instantly does 4 damage.
		if (fighters[0].Mana >= 53)
		{
			vector<Fighter> newFighters = fighters;
			newFighters[0].Mana -= 53;
			newFighters[1].HitPoints -= 4;
			int64_t candidateCost = MinimumManaForWin(newFighters, 1, difficulty, currentManaSpend + 53, globalManaMinimum);
			minimumMana = min(minimumMana, candidateCost);
		}

		// Drain costs 73 mana. It instantly does 2 damage and heals you for 2 hit points.
		if (fighters[0].Mana >= 73)
		{
			vector<Fighter> newFighters = fighters;
			newFighters[0].Mana -= 73;
			newFighters[0].HitPoints += 2;
			newFighters[1].HitPoints -= 2;
			int64_t candidateCost = MinimumManaForWin(newFighters, 1, difficulty, currentManaSpend + 73, globalManaMinimum);
			minimumMana = min(minimumMana, candidateCost);
		}

		// Shield costs 113 mana. It starts an effect that lasts for 6 turns. While it is active, your armor is increased by 7.
		if ((fighters[0].Mana >= 113) && (fighters[0].EffectTimers[EffectType::Shield] == 0))
		{
			vector<Fighter> newFighters = fighters;
			newFighters[0].Mana -= 113;
			newFighters[0].EffectTimers[EffectType::Shield] = 6;
			int64_t candidateCost = MinimumManaForWin(newFighters, 1, difficulty, currentManaSpend + 113, globalManaMinimum);
			minimumMana = min(minimumMana, candidateCost);
		}

		// Poison costs 173 mana. It starts an effect that lasts for 6 turns. At the start of each turn while it is active, it deals the boss 3 damage.
		if ((fighters[0].Mana >= 173) && (fighters[0].EffectTimers[EffectType::Poison] == 0))
		{
			vector<Fighter> newFighters = fighters;
			newFighters[0].Mana -= 173;
			newFighters[0].EffectTimers[EffectType::Poison] = 6;
			int64_t candidateCost = MinimumManaForWin(newFighters, 1, difficulty, currentManaSpend + 173, globalManaMinimum);
			minimumMana = min(minimumMana, candidateCost);
		}

		// Recharge costs 229 mana. It starts an effect that lasts for 5 turns. At the start of each turn while it is active, it gives you 101 new mana.
		if ((fighters[0].Mana >= 229) && (fighters[0].EffectTimers[EffectType::Recharge] == 0))
		{
			vector<Fighter> newFighters = fighters;
			newFighters[0].Mana -= 229;
			newFighters[0].EffectTimers[EffectType::Recharge] = 5;
			int64_t candidateCost = MinimumManaForWin(newFighters, 1, difficulty, currentManaSpend + 229, globalManaMinimum);
			minimumMana = min(minimumMana, candidateCost);
		}
	}

	// Boss: Deal damage
	if (activePlayer == 1)
	{
		vector<Fighter> newFighters = fighters;
		int64_t damageDealt = max(fighters[1].Damage - fighters[0].Armour, 1ll);
		newFighters[0].HitPoints -= damageDealt;
		int64_t candidateCost = MinimumManaForWin(newFighters, 0, difficulty, currentManaSpend, globalManaMinimum);
		minimumMana = min(minimumMana, candidateCost);
	}

	return minimumMana;
}

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Fighter> fighters;
	fighters.push_back(CreatePlayer());
	fighters.push_back(ParseBoss(input));

	int64_t minimumMana = numeric_limits<int64_t>::max();
	int64_t answer = MinimumManaForWin(fighters, 0, Difficulty::Easy, 0, &minimumMana);

	printf("[2015] Puzzle22_A: %" PRId64 "\n", answer);
}


static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Fighter> fighters;
	fighters.push_back(CreatePlayer());
	fighters.push_back(ParseBoss(input));

	int64_t minimumMana = numeric_limits<int64_t>::max();
	int64_t answer = MinimumManaForWin(fighters, 0, Difficulty::Hard, 0, &minimumMana);

	printf("[2015] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2015(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
