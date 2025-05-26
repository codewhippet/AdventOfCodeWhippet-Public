#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2022_Types
{
	enum ResourceType : int
	{
		Ore,
		Clay,
		Obsidian,
		Geode,

		COUNT
	};

	struct Blueprint
	{
		int Id = -1;

		int OreRobotCostsOre = -1;
		int ClayRobotCostsOre = -1;
		int ObsidianRobotCostsOre = -1;
		int ObsidianRobotCostsClay = -1;
		int GeodeRobotCostsOre = -1;
		int GeodeRobotCostsObsidian = -1;

		int MaximumOreRobots = -1;
		int MaximumClayRobots = -1;
		int MaximumObsidianRobots = -1;
	};

	struct State
	{
		int TimeLeft = 0;

		int OreInStock = 0;
		int ClayInStock = 0;
		int ObsidianInStock = 0;

		int OreRobots = 0;
		int ClayRobots = 0;
		int ObsidianRobots = 0;

		int GeodesInStock = 0;
		int GeodeRobots = 0;

		auto operator<=>(const State&) const = default;
	};
}

using namespace Puzzle19_2022_Types;

static vector<Blueprint> ReadBlueprints(istream &input)
{
	vector<Blueprint> blueprints;

	for (const auto &line : ScanfEachLine<int, int, int, int, int, int, int>(input,
		"Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian."))
	{
		Blueprint b;

		b.Id = get<0>(line);

		b.OreRobotCostsOre = get<1>(line);
		b.ClayRobotCostsOre = get<2>(line);
		b.ObsidianRobotCostsOre = get<3>(line);
		b.ObsidianRobotCostsClay = get<4>(line);
		b.GeodeRobotCostsOre = get<5>(line);
		b.GeodeRobotCostsObsidian = get<6>(line);

		b.MaximumOreRobots = max(max(max(b.OreRobotCostsOre, b.ClayRobotCostsOre), b.ObsidianRobotCostsOre), b.GeodeRobotCostsOre);
		b.MaximumClayRobots = b.ObsidianRobotCostsClay;
		b.MaximumObsidianRobots = b.GeodeRobotCostsObsidian;

		blueprints.push_back(b);
	}

	return blueprints;
}

static int MaxPossibleInTimeRemaining(int timeLeft)
{
	int timeAfterBuild = timeLeft - 1;
	if (timeAfterBuild < 1)
		return 0;

	return (timeAfterBuild * (timeAfterBuild + 1)) / 2;
}

static int MaxGeodesCollected(const Blueprint& blueprint, const State& state, int maxSoFar, map<State, int>* cache)
{
	const int minimumTimeForCache = 7;

	if (state.TimeLeft <= 0)
	{
		return state.GeodesInStock;
	}

	// Check to see if we've already done the best we can
	int currentGeodes = state.GeodesInStock;
	int guaranteedHarvest = state.GeodeRobots * state.TimeLeft;
	int potentialHarvest = MaxPossibleInTimeRemaining(state.TimeLeft);
	if (maxSoFar >= (currentGeodes + guaranteedHarvest + potentialHarvest))
	{
		return maxSoFar;
	}

	if (state.TimeLeft >= minimumTimeForCache)
	{
		auto existingAnswer = cache->find(state);
		if (existingAnswer != cache->end())
		{
			return existingAnswer->second;
		}
	}

	int maximumCollected = state.GeodesInStock;

	// Can we build a geode robot?
	if ((state.OreInStock >= blueprint.GeodeRobotCostsOre) &&
		(state.ObsidianInStock >= blueprint.GeodeRobotCostsObsidian))
	{
		State newState = state;
		newState.TimeLeft = state.TimeLeft - 1;

		newState.OreInStock += state.OreRobots - blueprint.GeodeRobotCostsOre;
		newState.ClayInStock += state.ClayRobots;
		newState.ObsidianInStock += state.ObsidianRobots - blueprint.GeodeRobotCostsObsidian;
		newState.GeodesInStock += state.GeodeRobots;

		newState.GeodeRobots++;

		int candidateMaximum = MaxGeodesCollected(blueprint, newState, maximumCollected, cache);
		maximumCollected = max(maximumCollected, candidateMaximum);
		//return maximumCollected;
	}

	// Can we build an obsidian robot?
	if ((state.ObsidianRobots < blueprint.MaximumObsidianRobots) &&
		(state.OreInStock >= blueprint.ObsidianRobotCostsOre) &&
		(state.ClayInStock >= blueprint.ObsidianRobotCostsClay))
	{
		State newState = state;
		newState.TimeLeft = state.TimeLeft - 1;

		newState.OreInStock += state.OreRobots - blueprint.ObsidianRobotCostsOre;
		newState.ClayInStock += state.ClayRobots - blueprint.ObsidianRobotCostsClay;
		newState.ObsidianInStock += state.ObsidianRobots;
		newState.GeodesInStock += state.GeodeRobots;

		newState.ObsidianRobots++;

		int candidateMaximum = MaxGeodesCollected(blueprint, newState, maximumCollected, cache);
		maximumCollected = max(maximumCollected, candidateMaximum);
		//return maximumCollected;
	}

	// Can we build a clay robot?
	if ((state.ClayRobots < blueprint.MaximumClayRobots) &&
		(state.OreInStock >= blueprint.ClayRobotCostsOre))
	{
		State newState = state;
		newState.TimeLeft = state.TimeLeft - 1;

		newState.OreInStock += state.OreRobots - blueprint.ClayRobotCostsOre;
		newState.ClayInStock += state.ClayRobots;
		newState.ObsidianInStock += state.ObsidianRobots;
		newState.GeodesInStock += state.GeodeRobots;

		newState.ClayRobots++;

		int candidateMaximum = MaxGeodesCollected(blueprint, newState, maximumCollected, cache);
		maximumCollected = max(maximumCollected, candidateMaximum);
	}

	// Can we build an ore robot?
	if ((state.OreRobots < blueprint.MaximumOreRobots) &&
		(state.OreInStock >= blueprint.OreRobotCostsOre))
	{
		State newState = state;
		newState.TimeLeft = state.TimeLeft - 1;

		newState.OreInStock += state.OreRobots - blueprint.OreRobotCostsOre;
		newState.ClayInStock += state.ClayRobots;
		newState.ObsidianInStock += state.ObsidianRobots;
		newState.GeodesInStock += state.GeodeRobots;

		newState.OreRobots++;

		int candidateMaximum = MaxGeodesCollected(blueprint, newState, maximumCollected, cache);
		maximumCollected = max(maximumCollected, candidateMaximum);
	}

	// Try waiting instead
	if (state.TimeLeft > 1)
	{
		State newState = state;
		newState.TimeLeft = state.TimeLeft - 1;

		newState.OreInStock += state.OreRobots;
		newState.ClayInStock += state.ClayRobots;
		newState.ObsidianInStock += state.ObsidianRobots;
		newState.GeodesInStock += state.GeodeRobots;

		// If we've maxed out our robots, don't carry across excess stock
		// This helps cache efficiency
		if (newState.OreRobots == blueprint.MaximumOreRobots)
		{
			newState.OreInStock = blueprint.MaximumOreRobots;
		}
		if (newState.ClayRobots == blueprint.MaximumClayRobots)
		{
			newState.ClayInStock = blueprint.MaximumClayRobots;
		}

		int candidateMaximum = MaxGeodesCollected(blueprint, newState, maximumCollected, cache);
		maximumCollected = max(maximumCollected, candidateMaximum);
	}

	if (state.TimeLeft >= minimumTimeForCache)
	{
		cache->insert({ state, maximumCollected });
	}

	return maximumCollected;
}

static void Puzzle19_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Blueprint> blueprints = ReadBlueprints(input);

	int64_t answer = 0;
	for (const auto& blueprint : blueprints)
	{
		map<State, int> cache;
		int maximumGeodes = MaxGeodesCollected(blueprint,
			State{
			.TimeLeft = 24,
			.OreInStock = 0,
			.ClayInStock = 0,
			.ObsidianInStock = 0,
			.OreRobots = 1,
			.ClayRobots = 0,
			.ObsidianRobots = 0
			},
			0,
			&cache);
		int qualityLevel = blueprint.Id * maximumGeodes;

		answer += qualityLevel;
	}

	printf("[2022] Puzzle19_A: %" PRId64 "\n", answer);
}

static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Blueprint> blueprints = ReadBlueprints(input) | views::take(3) | ranges::to<vector>();

	int64_t answer = 1;
	for (const auto& blueprint : blueprints)
	{
		map<State, int> cache;
		int maximumGeodes = MaxGeodesCollected(blueprint,
			State{
			.TimeLeft = 32,
			.OreInStock = 0,
			.ClayInStock = 0,
			.ObsidianInStock = 0,
			.OreRobots = 1,
			.ClayRobots = 0,
			.ObsidianRobots = 0
			},
			0,
			&cache);

		answer *= maximumGeodes;
	}

	printf("[2022] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2022(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
