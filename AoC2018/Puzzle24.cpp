#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2018_Types
{
	struct Group
	{
		int64_t System;
		int64_t Group;

		int64_t Units;
		int64_t HitPoints;
		set<string> ImuneTo;
		set<string> WeakAgainst;
		string DamageType;
		int64_t DamageAmount;
		int64_t Initiative;

		int64_t EffectivePower() const
		{
			return Units * DamageAmount;
		}
	};

	struct System
	{
		vector<shared_ptr<Group>> Groups;
	};

	struct Simulation
	{
		vector<shared_ptr<System>> Systems;
	};

	struct AttackChoice
	{
		int64_t DamageFactor;
		int64_t EffectivePowerFactor;
		int64_t InitiativeFactor;

		auto operator<=>(const AttackChoice&) const = default;
	};

	enum class SimulationResult
	{
		Completed,
		Aborted,
	};
}

using namespace Puzzle24_2018_Types;

static set<string> ParseMultipliersFor(const string& type, const string& description)
{
	if (description.empty())
	{
		return {};
	}

	set<string> multipliers;

	auto damageTypes = StringSplitTrimmed(description.substr(1, description.size() - 2), ';')
		| views::filter([&](const string& s) { return s.starts_with(type); })
		| views::transform([&](const string& s) { return s.substr(type.size()); })
		| views::transform([](const string& s) { return StringSplitTrimmed(s, ','); })
		| views::join
		| views::common;
	ranges::copy(damageTypes, inserter(multipliers, multipliers.end()));

	return multipliers;
}

static Simulation ParsePuzzle(istream& input)
{
	Simulation sim;

	regex unitPattern(R"((\d+) units each with (\d+) hit points (\([^\)]+\))? ?with an attack that does (\d+) (\w+) damage at initiative (\d+))");

	sim.Systems.push_back(make_shared<System>());
	for (const string& line : ReadEachLine(input))
	{
		if (line.empty())
		{
			sim.Systems.push_back(make_shared<System>());
			continue;
		}

		smatch m;
		if (regex_match(line, m, unitPattern) == false)
		{
			continue;
		}

		shared_ptr<Group> newGroup = make_shared<Group>();

		newGroup->System = sim.Systems.size();
		newGroup->Group = sim.Systems.back()->Groups.size() + 1;

		newGroup->Units = atoll(m[1].str().c_str());
		newGroup->HitPoints = atoll(m[2].str().c_str());
		newGroup->ImuneTo = ParseMultipliersFor("immune to", m[3].str());
		newGroup->WeakAgainst = ParseMultipliersFor("weak to", m[3].str());
		newGroup->DamageType = m[5].str();
		newGroup->DamageAmount = atoll(m[4].str().c_str());
		newGroup->Initiative = atoll(m[6].str().c_str());

		sim.Systems.back()->Groups.push_back(newGroup);
	}

	return sim;
}

static Simulation CloneSimulation(const Simulation& sim)
{
	Simulation clone;

	for (const shared_ptr<System> system : sim.Systems)
	{
		shared_ptr<System> newSystem = make_shared<System>();
		for (const shared_ptr<Group>& group : system->Groups)
		{
			shared_ptr<Group> newGroup = make_shared<Group>(*group);
			newSystem->Groups.push_back(newGroup);
		}
		clone.Systems.push_back(newSystem);
	}

	return clone;
}

static bool GroupHasUnits(const shared_ptr<Group>& g)
{
	return g->Units > 0;
}

static bool UnitsLeftToFight(const Simulation& sim)
{
	return ranges::any_of(sim.Systems[0]->Groups, GroupHasUnits) && ranges::any_of(sim.Systems[1]->Groups, GroupHasUnits);
}

static int64_t CountUnitsInSystem(const Simulation& sim, size_t system)
{
	auto unitCount = sim.Systems[system]->Groups | views::transform([](const shared_ptr<Group>& g) { return g->Units; });
	return accumulate(unitCount.begin(), unitCount.end(), 0ll);
}

static set<shared_ptr<Group>> GetAllAliveGroups(const Simulation& sim)
{
	set<shared_ptr<Group>> aliveGroups;

	auto units = sim.Systems
		| views::transform([](const shared_ptr<System>& s) { return s->Groups; })
		| views::join
		| views::filter(GroupHasUnits);
	ranges::copy(units, inserter(aliveGroups, aliveGroups.end()));

	return aliveGroups;
}

static int64_t MaximumDamageDealt(const shared_ptr<Group>& attacker, const shared_ptr<Group>& target)
{
	int64_t damageMultiplier = 1;
	if (target->ImuneTo.contains(attacker->DamageType))
	{
		damageMultiplier = 0;
	}
	if (target->WeakAgainst.contains(attacker->DamageType))
	{
		assert(damageMultiplier == 1);
		damageMultiplier = 2;
	}
	return attacker->EffectivePower() * damageMultiplier;
}

static SimulationResult SimulateFight(Simulation* sim)
{
	while (UnitsLeftToFight(*sim))
	{
		set<shared_ptr<Group>> allAliveGroups = GetAllAliveGroups(*sim);

		// Target selection
		set<shared_ptr<Group>> unselectedGroups = allAliveGroups;
		map<pair<int64_t, int64_t>, shared_ptr<Group>> selectionOrder;
		ranges::copy(unselectedGroups | views::transform(
			[](const shared_ptr<Group>& group) -> decltype(selectionOrder)::value_type
			{
				return { { -group->EffectivePower(), -group->Initiative }, group };
			}),
			inserter(selectionOrder, selectionOrder.end()));
		assert(selectionOrder.size() == allAliveGroups.size());

		map<shared_ptr<Group>, shared_ptr<Group>> attackTargets;
		set<shared_ptr<Group>> selectedGroups;
		for (const auto& kvp : selectionOrder)
		{
			shared_ptr<Group> attackingGroup = kvp.second;

			map<AttackChoice, shared_ptr<Group>> attackCandidates;
			ranges::copy(unselectedGroups
				| views::filter([&](const shared_ptr<Group>& g)
					{
						return g->System != attackingGroup->System;
					})
				| views::transform([&](const shared_ptr<Group>& target) -> decltype(attackCandidates)::value_type
					{
						return { { -MaximumDamageDealt(attackingGroup, target), -target->EffectivePower(), -target->Initiative }, target };
					})
				| views::filter([](const auto& candidate)
					{
						return (candidate.first.DamageFactor != 0);
					}),
				inserter(attackCandidates, attackCandidates.end()));

			if (attackCandidates.empty() == false)
			{
				shared_ptr<Group> target = attackCandidates.begin()->second;

				assert(attackTargets.contains(attackingGroup) == false);
				attackTargets[attackingGroup] = target;
				unselectedGroups.erase(target);
				selectedGroups.insert(target);
			}
		}

		assert(attackTargets.size() == selectedGroups.size());

		// Attack
		map<int64_t, shared_ptr<Group>> attackingOrder;
		ranges::copy(allAliveGroups
			| views::filter([&](const shared_ptr<Group>& g)
				{
					return attackTargets.contains(g);
				})
			| views::transform([](const shared_ptr<Group>& g) -> decltype(attackingOrder)::value_type
				{
					return { -g->Initiative, g };
				}),
			inserter(attackingOrder, attackingOrder.end()));

		int64_t totalUnitsKilled = 0;
		for (const auto& kvp : attackingOrder)
		{
			shared_ptr<Group> attackingGroup = kvp.second;
			if (attackingGroup->Units == 0)
				continue;

			shared_ptr<Group> targetGroup = attackTargets.at(attackingGroup);
			int64_t damageDealt = MaximumDamageDealt(attackingGroup, targetGroup);
			int64_t unitsKilled = min(damageDealt / targetGroup->HitPoints, targetGroup->Units);
			targetGroup->Units = targetGroup->Units - unitsKilled;

			totalUnitsKilled += unitsKilled;
		}
		if (totalUnitsKilled == 0)
		{
			return SimulationResult::Aborted;
		}
	}

	return SimulationResult::Completed;
}

static SimulationResult GetOutcomeOfFight(const Simulation& sim, int64_t boost, pair<int64_t, int64_t> *unitsLeft)
{
	Simulation boostedSim = CloneSimulation(sim);
	ranges::for_each(boostedSim.Systems[0]->Groups, [&](const shared_ptr<Group>& g) { g->DamageAmount += boost; });
	SimulationResult simResult = SimulateFight(&boostedSim);
	if (simResult == SimulationResult::Completed)
	{
		*unitsLeft = { CountUnitsInSystem(boostedSim, 0), CountUnitsInSystem(boostedSim, 1) };
	}
	return simResult;
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Simulation sim = ParsePuzzle(input);

	pair<int64_t, int64_t> unitsLeft = { 0, 0 };
	SimulationResult outcome = GetOutcomeOfFight(sim, 0, &unitsLeft);
	assert(outcome == SimulationResult::Completed);
	(void)outcome;
	int64_t answer = max(unitsLeft.first, unitsLeft.second);

	printf("[2018] Puzzle24_A: %" PRId64 "\n", answer);
}


static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Simulation startingSimulation = ParsePuzzle(input);

	int64_t answer = 0;
	for (size_t boost = 1; boost < 100; boost++)
	{
		pair<int64_t, int64_t> unitsLeft = { -1, -1 };
		SimulationResult outcome = GetOutcomeOfFight(startingSimulation, boost, &unitsLeft);
		if ((outcome == SimulationResult::Completed) && (unitsLeft.first > 0))
		{
			answer = unitsLeft.first;
			break;
		}
	}


	printf("[2018] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2018(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
