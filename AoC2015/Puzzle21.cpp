#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2015_Types
{
	struct Fighter
	{
		int64_t HitPoints = 0;
		int64_t Damage = 0;
		int64_t Armour = 0;
	};

	struct Item
	{
		string Name;
		int64_t Cost = 0;
		int64_t Damage = 0;
		int64_t Amrour = 0;
	};
}

using namespace Puzzle21_2015_Types;

static Fighter ParseBoss(istream& input)
{
	vector<string> lines = ReadAllLines(input);
	assert(lines.size() == 3);

	Fighter boss;

	int scanned = 0;
	scanned += sscanf(lines[0].c_str(), "Hit Points: %lld", &boss.HitPoints);
	scanned += sscanf(lines[1].c_str(), "Damage: %lld", &boss.Damage);
	scanned += sscanf(lines[2].c_str(), "Armor: %lld", &boss.Armour);
	assert(scanned == 3);
	(void)scanned;

	return boss;
}

static Fighter CreatePlayer(int64_t hitPoints, const vector<Item>& items)
{
	Fighter player;
	player.HitPoints = hitPoints;

	for (const Item& item : items)
	{
		player.Damage += item.Damage;
		player.Armour += item.Amrour;
	}

	return player;
}

static size_t WhoWins(vector<Fighter> fighters)
{
	size_t currentAttacker = 0;
	while (true)
	{
		size_t opponent = 1 - currentAttacker;
		int64_t damageDealt = max(fighters[currentAttacker].Damage - fighters[opponent].Armour, 1ll);
		fighters[opponent].HitPoints -= damageDealt;
		if (fighters[opponent].HitPoints <= 0)
		{
			return currentAttacker;
		}
		currentAttacker = 1 - currentAttacker;
	}
}

static const vector<Item>& WeaponInventory()
{
	static const vector<Item> weapons =
	{
		{ "Dagger", 8, 4, 0 },
		{ "Shortsword", 10, 5, 0 },
		{ "Warhammer", 25, 6, 0 },
		{ "Longsword", 40, 7, 0 },
		{ "Greataxe", 74, 8, 0 }
	};
	return weapons;
}

static const vector<Item>& ArmourInventory()
{
	static const vector<Item> armoury =
	{
		{ "None", 0, 0, 0 },
		{ "Leather", 13, 0, 1 },
		{ "Chainmail", 31, 0, 2 },
		{ "Splintmail", 53, 0, 3 },
		{ "Bandedmail", 75, 0, 4 },
		{ "Platemail", 102, 0, 5 }
	};
	return armoury;
}

static const vector<Item>& RingInventory()
{
	static const vector<Item> rings =
	{
		{ "None #1", 0, 0, 0 },
		{ "None #2", 0, 0, 0 },
		{ "Damage +1", 25, 1, 0 },
		{ "Damage +2", 50, 2, 0 },
		{ "Damage +3", 100, 3, 0 },
		{ "Defense +1", 20, 0, 1 },
		{ "Defense +2", 40, 0, 2 },
		{ "Defense +3", 80, 0, 3 }
	};
	return rings;
}

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t playerHitpoints = 100;

	vector<Fighter> fighters;
	fighters.push_back({ 0, 0, 0 });
	fighters.push_back(ParseBoss(input));

	int64_t answer = numeric_limits<int64_t>::max();
	for (size_t weapon = 0; weapon < WeaponInventory().size(); weapon++)
	{
		for (size_t armour = 0; armour < ArmourInventory().size(); armour++)
		{
			for (size_t leftRing = 0; leftRing < RingInventory().size(); leftRing++)
			{
				for (size_t rightRing = leftRing + 1; rightRing < RingInventory().size(); rightRing++)
				{
					int64_t equipmentCost =
						WeaponInventory()[weapon].Cost +
						ArmourInventory()[armour].Cost +
						RingInventory()[leftRing].Cost +
						RingInventory()[rightRing].Cost;
					if (equipmentCost < answer)
					{
						fighters[0] = CreatePlayer(playerHitpoints,
							{
								WeaponInventory()[weapon],
								ArmourInventory()[armour],
								RingInventory()[leftRing],
								RingInventory()[rightRing]
							});

						size_t winner = WhoWins(fighters);
						if (winner == 0)
						{
							answer = equipmentCost;
						}
					}
				}
			}
		}
	}

	printf("[2015] Puzzle21_A: %" PRId64 "\n", answer);
}


static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t playerHitpoints = 100;

	vector<Fighter> fighters;
	fighters.push_back({ 0, 0, 0 });
	fighters.push_back(ParseBoss(input));

	int64_t answer = 0;
	for (size_t weapon = 0; weapon < WeaponInventory().size(); weapon++)
	{
		for (size_t armour = 0; armour < ArmourInventory().size(); armour++)
		{
			for (size_t leftRing = 0; leftRing < RingInventory().size(); leftRing++)
			{
				for (size_t rightRing = leftRing + 1; rightRing < RingInventory().size(); rightRing++)
				{
					int64_t equipmentCost =
						WeaponInventory()[weapon].Cost +
						ArmourInventory()[armour].Cost +
						RingInventory()[leftRing].Cost +
						RingInventory()[rightRing].Cost;
					if (equipmentCost > answer)
					{
						fighters[0] = CreatePlayer(playerHitpoints,
							{
								WeaponInventory()[weapon],
								ArmourInventory()[armour],
								RingInventory()[leftRing],
								RingInventory()[rightRing]
							});

						size_t winner = WhoWins(fighters);
						if (winner == 1)
						{
							answer = equipmentCost;
						}
					}
				}
			}
		}
	}

	printf("[2015] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2015(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
