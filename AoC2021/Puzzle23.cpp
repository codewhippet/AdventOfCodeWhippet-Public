#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2021_Types
{
	struct State
	{
		string Corridor;
		string Bedrooms[4];

		static size_t EmergesAt[4];
	};

	bool operator<(const State& a, const State& b)
	{
		if (a.Corridor != b.Corridor)
		{
			return a.Corridor < b.Corridor;
		}

		for (int i = 0; i < 4; i++)
		{
			if (a.Bedrooms[i] != b.Bedrooms[i])
			{
				return a.Bedrooms[i] < b.Bedrooms[i];
			}
		}

		return false;
	}

	size_t State::EmergesAt[4] = { 2, 4, 6, 8 };

	static map<string, int64_t> MinimumCost;
	static map<State, int64_t> MinimumCost2;

	static char TargetOccupant[8] = { 'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D' };
	static size_t EmergesAt[8] = { 2, 2, 4, 4, 6, 6, 8, 8 };
	static size_t TargetBedroomForType[4] = { 0, 2, 4, 6 };
	static const char* EmptyBedroom = "..";
	static const char* HalfEmptyBedroom[8] = { ".A", ".A", ".B", ".B", ".C", ".C", ".D", ".D" };
	static int64_t CostMultipliers[4] = { 1, 10, 100, 1000 };

	static set<string> BedroomReadyStates[4] =
	{
		{ "....", "...A", "..AA", ".AAA" },
		{ "....", "...B", "..BB", ".BBB" },
		{ "....", "...C", "..CC", ".CCC" },
		{ "....", "...D", "..DD", ".DDD" },
	};

	static string BedroomCompleteState[4] =
	{
		"AAAA", "BBBB", "CCCC", "DDDD",
	};
}

using namespace Puzzle23_2021_Types;

static bool CanHeadOutTo(const string& state, size_t corridorIndex, size_t bedroomIndex)
{
	assert(state[11 + bedroomIndex] != '.');

	// No lingering outside of bedrooms
	switch (corridorIndex)
	{
	case 2:
	case 4:
	case 6:
	case 8:
		return false;
	}

	const string_view bedrooms(state.c_str() + 11, 8);

	// Don't leave if we're already home
	if ((bedroomIndex & 1) == 1)
	{
		if (bedrooms[bedroomIndex] == TargetOccupant[bedroomIndex])
		{
			return false;
		}
	}
	else
	{
		if ((bedrooms[bedroomIndex + 0] == TargetOccupant[bedroomIndex + 0]) &&
			(bedrooms[bedroomIndex + 1] == TargetOccupant[bedroomIndex + 1]))
		{
			return false;
		}
	}

	// Front occupant blocks rear occupant
	if (((bedroomIndex & 1) == 1) && (bedrooms[bedroomIndex - 1] != '.'))
	{
		return false;
	}

	// Is there a clear path?
	size_t startFrom = EmergesAt[bedroomIndex];
	size_t minCorridor = min(startFrom, corridorIndex);
	size_t maxCorridor = max(startFrom, corridorIndex);
	string_view travelPath(state.c_str() + minCorridor, maxCorridor - minCorridor + 1);
	for (char c : travelPath)
	{
		if (c != '.')
		{
			return false;
		}
	}

	return true;
}

static int64_t HeadOutTo(const string& oldState, size_t corridorIndex, size_t bedroomIndex, string* newState)
{
	assert(oldState[corridorIndex] == '.');
	assert(oldState[11 + bedroomIndex] != '.');

	*newState = oldState;
	(*newState)[corridorIndex] = oldState[11 + bedroomIndex];
	(*newState)[11 + bedroomIndex] = '.';

	int64_t bedroomExitCost = ((bedroomIndex & 1) == 1 ? 2 : 1);

	size_t startFrom = EmergesAt[bedroomIndex];
	size_t minCorridor = min(startFrom, corridorIndex);
	size_t maxCorridor = max(startFrom, corridorIndex);
	int64_t corridorCost = maxCorridor - minCorridor;

	char ourType = oldState[11 + bedroomIndex];

	return (bedroomExitCost + corridorCost) * CostMultipliers[ourType - 'A'];
}

static bool CanGoHome(const string& state, size_t corridorIndex)
{
	assert(state[corridorIndex] != '.');

	char ourType = state[corridorIndex];

	// What bedroom should we go in?
	size_t targetBedroomIndex = TargetBedroomForType[ourType - 'A'];
	const string_view bedroom(state.c_str() + 11 + targetBedroomIndex, 2);

	// Is the bedroom ready to go in?
	bool bedroomReady = ((bedroom == EmptyBedroom) || (bedroom == HalfEmptyBedroom[targetBedroomIndex]));
	if (bedroomReady == false)
	{
		return false;
	}

	// Is there a clear path?
	size_t headTo = EmergesAt[targetBedroomIndex];
	size_t minCorridor = min(headTo, corridorIndex);
	size_t maxCorridor = max(headTo, corridorIndex);
	string_view travelPath(state.c_str() + minCorridor, maxCorridor - minCorridor + 1);
	for (size_t i = minCorridor; i <= maxCorridor; i++)
	{
		if ((i != corridorIndex) && (state[i] != '.'))
		{
			return false;
		}
	}

	return true;
}

static int64_t GoHome(const string& oldState, size_t corridorIndex, string* newState)
{
	assert(oldState[corridorIndex] != '.');

	char ourType = oldState[corridorIndex];

	// What bedroom should we go in?
	size_t targetBedroomIndex = TargetBedroomForType[ourType - 'A'];
	const string_view bedroom(oldState.c_str() + 11 + targetBedroomIndex, 2);

	// First in?
	int64_t bedroomEntranceCost = 1;
	if (bedroom[1] == '.')
	{
		bedroomEntranceCost = 2;
		targetBedroomIndex++;
	}

	*newState = oldState;
	(*newState)[corridorIndex] = '.';
	(*newState)[11 + targetBedroomIndex] = ourType;

	size_t headTo = EmergesAt[targetBedroomIndex];
	size_t minCorridor = min(headTo, corridorIndex);
	size_t maxCorridor = max(headTo, corridorIndex);
	int64_t corridorCost = maxCorridor - minCorridor;

	return (bedroomEntranceCost + corridorCost) * CostMultipliers[ourType - 'A'];
}

static bool FindMinimumCost(const string& state, int64_t* globalMinCost)
{
	map<string, int64_t>::const_iterator previousSolution = MinimumCost.find(state);
	if (previousSolution != MinimumCost.end())
	{
		*globalMinCost = previousSolution->second;
		return previousSolution->second != numeric_limits<int64_t>::max();
	}

	int64_t minimumCost = numeric_limits<int64_t>::max();
	bool hasCost = false;

	// See if we can put someone home
	const string_view corridor(state.c_str(), 11);
	for (size_t i = 0; i < corridor.size(); i++)
	{
		if ((corridor[i] != '.') && CanGoHome(state, i))
		{
			string nextState;
			int64_t goHomeCost = GoHome(state, i, &nextState);
			int64_t cost;
			if (FindMinimumCost(nextState, &cost))
			{
				minimumCost = min(cost + goHomeCost, minimumCost);
				hasCost = true;
			}
		}
	}

	// See if someone can come out
	const string_view bedrooms(state.c_str() + 11, 8);
	for (size_t bedroomIndex = 0; bedroomIndex < bedrooms.size(); bedroomIndex++)
	{
		if (bedrooms[bedroomIndex] != '.')
		{
			for (size_t corridorIndex = 0; corridorIndex < corridor.size(); corridorIndex++)
			{
				if (CanHeadOutTo(state, corridorIndex, bedroomIndex))
				{
					string nextState;
					int64_t headOutCost = HeadOutTo(state, corridorIndex, bedroomIndex, &nextState);
					int64_t cost;
					if (FindMinimumCost(nextState, &cost))
					{
						minimumCost = min(cost + headOutCost, minimumCost);
						hasCost = true;
					}
				}
			}
		}
	}

	assert(minimumCost >= 0);
	MinimumCost[state] = minimumCost;
	if (hasCost)
	{
		*globalMinCost = minimumCost;
	}
	return hasCost;
}


static bool CanHeadOutTo(const State& state, size_t corridorIndex, size_t bedroomIndex)
{
	// Can't leave an emty bedroom
	if (state.Bedrooms[bedroomIndex] == "....")
		return false;

	// No lingering outside of bedrooms
	switch (corridorIndex)
	{
	case 2:
	case 4:
	case 6:
	case 8:
		return false;
	}

	// Don't leave an already ready bedroom
	if (BedroomReadyStates[bedroomIndex].find(state.Bedrooms[bedroomIndex]) != BedroomReadyStates[bedroomIndex].end())
	{
		return false;
	}

	// Don't leave a full bedroom
	if (state.Bedrooms[bedroomIndex] == BedroomCompleteState[bedroomIndex])
	{
		return false;
	}

	// Is there a clear path?
	size_t startFrom = State::EmergesAt[bedroomIndex];
	size_t minCorridor = min(startFrom, corridorIndex);
	size_t maxCorridor = max(startFrom, corridorIndex);
	string_view travelPath(state.Corridor.c_str() + minCorridor, maxCorridor - minCorridor + 1);
	for (char c : travelPath)
	{
		if (c != '.')
		{
			return false;
		}
	}

	return true;
}

static int64_t HeadOutTo(const State& oldState, size_t corridorIndex, size_t bedroomIndex, State* newState)
{
	assert(oldState.Corridor[corridorIndex] == '.');
	assert(oldState.Bedrooms[bedroomIndex] != "....");

	size_t bedroomLeader = oldState.Bedrooms[bedroomIndex].find_first_not_of('.');
	assert(bedroomLeader != string::npos);
	char ourType = oldState.Bedrooms[bedroomIndex][bedroomLeader];

	*newState = oldState;
	newState->Corridor[corridorIndex] = ourType;
	newState->Bedrooms[bedroomIndex][bedroomLeader] = '.';

	int64_t bedroomExitCost = bedroomLeader + 1;

	size_t startFrom = State::EmergesAt[bedroomIndex];
	size_t minCorridor = min(startFrom, corridorIndex);
	size_t maxCorridor = max(startFrom, corridorIndex);
	int64_t corridorCost = maxCorridor - minCorridor;

	return (bedroomExitCost + corridorCost) * CostMultipliers[ourType - 'A'];
}

static bool CanGoHome(const State& state, size_t corridorIndex)
{
	assert(state.Corridor[corridorIndex] != '.');

	char ourType = state.Corridor[corridorIndex];

	// What bedroom should we go in?
	size_t targetBedroomIndex = ourType - 'A';
	const string& bedroom = state.Bedrooms[targetBedroomIndex];

	// Is the bedroom ready to go in?
	if (BedroomReadyStates[targetBedroomIndex].find(bedroom) == BedroomReadyStates[targetBedroomIndex].end())
	{
		return false;
	}

	// Is there a clear path?
	size_t headTo = State::EmergesAt[targetBedroomIndex];
	size_t minCorridor = min(headTo, corridorIndex);
	size_t maxCorridor = max(headTo, corridorIndex);
	string_view travelPath(state.Corridor.c_str() + minCorridor, maxCorridor - minCorridor + 1);
	for (size_t i = minCorridor; i <= maxCorridor; i++)
	{
		if ((i != corridorIndex) && (state.Corridor[i] != '.'))
		{
			return false;
		}
	}

	return true;
}

static int64_t GoHome(const State& oldState, size_t corridorIndex, State* newState)
{
	assert(oldState.Corridor[corridorIndex] != '.');

	char ourType = oldState.Corridor[corridorIndex];

	// What bedroom should we go in?
	size_t targetBedroomIndex = ourType - 'A';

	size_t bedroomSlot = oldState.Bedrooms[targetBedroomIndex].find_last_of('.');
	assert(bedroomSlot != string::npos);

	// First in?
	int64_t bedroomEntranceCost = bedroomSlot + 1;

	*newState = oldState;
	newState->Corridor[corridorIndex] = '.';
	newState->Bedrooms[targetBedroomIndex][bedroomSlot] = ourType;

	size_t headTo = State::EmergesAt[targetBedroomIndex];
	size_t minCorridor = min(headTo, corridorIndex);
	size_t maxCorridor = max(headTo, corridorIndex);
	int64_t corridorCost = maxCorridor - minCorridor;

	return (bedroomEntranceCost + corridorCost) * CostMultipliers[ourType - 'A'];
}

static bool FindMinimumCost(const State& state, int64_t* globalMinCost)
{
	map<State, int64_t>::const_iterator previousSolution = MinimumCost2.find(state);
	if (previousSolution != MinimumCost2.end())
	{
		*globalMinCost = previousSolution->second;
		return previousSolution->second != numeric_limits<int64_t>::max();
	}

	int64_t minimumCost = numeric_limits<int64_t>::max();
	bool hasCost = false;

	// See if we can put someone home
	for (size_t i = 0; i < state.Corridor.size(); i++)
	{
		if ((state.Corridor[i] != '.') && CanGoHome(state, i))
		{
			State nextState;
			int64_t goHomeCost = GoHome(state, i, &nextState);
			int64_t cost;
			if (FindMinimumCost(nextState, &cost))
			{
				minimumCost = min(cost + goHomeCost, minimumCost);
				hasCost = true;
			}
		}
	}

	// See if someone can come out
	for (size_t bedroomIndex = 0; bedroomIndex < 4; bedroomIndex++)
	{
		if (state.Bedrooms[bedroomIndex] != "....")
		{
			for (size_t corridorIndex = 0; corridorIndex < state.Corridor.size(); corridorIndex++)
			{
				if (CanHeadOutTo(state, corridorIndex, bedroomIndex))
				{
					State nextState;
					int64_t headOutCost = HeadOutTo(state, corridorIndex, bedroomIndex, &nextState);
					int64_t cost;
					if (FindMinimumCost(nextState, &cost))
					{
						minimumCost = min(cost + headOutCost, minimumCost);
						hasCost = true;
					}
				}
			}
		}
	}

	assert(minimumCost >= 0);
	MinimumCost2[state] = minimumCost;
	if (hasCost)
	{
		*globalMinCost = minimumCost;
	}
	return hasCost;
}

static void Puzzle23_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	MinimumCost["...........AABBCCDD"] = 0;

	ArrayMap2D burrow = ReadArrayMap(input);
	string configuration = string{ "..........." }
		+ burrow(3, 2) + burrow(3, 3)
		+ burrow(5, 2) + burrow(5, 3)
		+ burrow(7, 2) + burrow(7, 3)
		+ burrow(9, 2) + burrow(9, 3);

	int64_t minimumCost;
	FindMinimumCost(configuration, &minimumCost);

	int64_t answer = minimumCost;

	printf("[2021] Puzzle23_A: %" PRId64 "\n", answer);
}

static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D burrow = ReadArrayMap(input);

	State endConfiguration;
	endConfiguration.Corridor = "...........";
	endConfiguration.Bedrooms[0] = "AAAA";
	endConfiguration.Bedrooms[1] = "BBBB";
	endConfiguration.Bedrooms[2] = "CCCC";
	endConfiguration.Bedrooms[3] = "DDDD";
	MinimumCost2[endConfiguration] = 0;

	State configuration;
	configuration.Corridor = "...........";
	configuration.Bedrooms[0] = burrow(3, 2) + string{ "DD" } + burrow(3, 3);
	configuration.Bedrooms[1] = burrow(5, 2) + string{ "CB" } + burrow(5, 3);
	configuration.Bedrooms[2] = burrow(7, 2) + string{ "BA" } + burrow(7, 3);
	configuration.Bedrooms[3] = burrow(9, 2) + string{ "AC" } + burrow(9, 3);

	int64_t cost;
	FindMinimumCost(configuration, &cost);

	int64_t answer = cost;

	printf("[2021] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2021(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
