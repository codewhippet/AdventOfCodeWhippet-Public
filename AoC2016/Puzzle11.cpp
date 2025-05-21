#include "stdafx.h"


using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2016_Types
{
	enum : size_t
	{
		Generator = 0,
		Microchip = 1
	};

	struct Puzzle
	{
		NameDictionary Elements;
		vector<char> State;
		int64_t TargetFloor;
	};
}

using namespace Puzzle11_2016_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle p;

	regex microchips(R"(([a-z]+)-compatible microchip)");
	regex generators(R"(([a-z]+) generator)");

	char floor = 1;
	for (const string& line : ReadEachLine(input))
	{
		for (sregex_iterator generatorIt(line.begin(), line.end(), generators); generatorIt != sregex_iterator{}; ++generatorIt)
		{
			int64_t elementId = p.Elements.IdFromName((*generatorIt)[1].str());
			p.State.resize(max(static_cast<size_t>((elementId + 1) * 2), p.State.size()));
			p.State[Generator + (elementId * 2)] = floor;
		}
		for (sregex_iterator microchipIt(line.begin(), line.end(), microchips); microchipIt != sregex_iterator{}; ++microchipIt)
		{
			int64_t elementId = p.Elements.IdFromName((*microchipIt)[1].str());
			p.State.resize(max(static_cast<size_t>((elementId + 1) * 2), p.State.size()));
			p.State[Microchip + (elementId * 2)] = floor;
		}

		floor++;
	}

	// We start on floor 1
	p.State.push_back(1);

	return p;
}

static bool IsStateValid(const Puzzle&, const vector<char>& state)
{
	uint64_t unconnectedChips = 0;
	uint64_t generators = 0;
	for (int64_t i = 0; (i + 1) < (int64_t)state.size(); i += 2)
	{
		if (state[Generator + i] != state[Microchip + i])
		{
			unconnectedChips |= 1ll << state[Microchip + i];
		}
		generators |= 1ll << state[Generator + i];
	}

	// Is there a generator on the same floor as an unshielded chip?
	return (unconnectedChips & generators) == 0;
}

static vector<int64_t> GetSameFloorItems(const vector<char>& state, char floor)
{
	vector<int64_t> items = { -1 };
	for (int64_t i = 0; i < (int64_t)(state.size() - 1); i++)
	{
		if (state[i] == floor)
		{
			items.push_back(i);
		}
	}
	return items;
}

static vector<vector<char>> GetNextStates(const Puzzle &p, const vector<char>& state)
{
	vector<vector<char>> nextStates;

	auto moveFromTo = [&](char from, char to)
		{
			// Take up to 2 items with you
			vector<int64_t> items = GetSameFloorItems(state, from);
			assert(items.size() > 1);
			for (size_t i = 0; i < items.size(); i++)
			{
				for (size_t j = i + 1; j < items.size(); j++)
				{
					vector<char> nextState = state;
					if (items[i] != -1)
					{
						nextState[items[i]] = to;
					}
					nextState[items[j]] = to;
					nextState.back() = to;
					nextStates.push_back(move(nextState));
				}
			}
		};

	// Go up
	if (state.back() < p.TargetFloor)
	{
		moveFromTo(state.back(), state.back() + 1);
	}

	// Go down
	if (state.back() > 1)
	{
		moveFromTo(state.back(), state.back() - 1);
	}

	return nextStates;
}

static int64_t ShortestPathToTargetFloor(const Puzzle& p, int64_t targetFloor)
{
	list<pair<int64_t, vector<char>>> searchQueue = { { 0, p.State } };

	set<vector<char>> queued = { p.State };
	while (searchQueue.empty() == false)
	{
		vector<char> currentState = move(searchQueue.front().second);
		int64_t currentSteps = searchQueue.front().first;
		searchQueue.pop_front();

		if (ranges::count(currentState, targetFloor) == (int64_t)currentState.size())
		{
			return currentSteps;
		}

		vector<vector<char>> nextStates = GetNextStates(p, currentState);
		for (auto& next : nextStates)
		{
			if (IsStateValid(p, next) && !queued.contains(next))
			{
				searchQueue.push_back({ currentSteps + 1, next });
				queued.insert(move(next));
			}
		}
	}

	return -1;
}

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);
	p.TargetFloor = 4;

	assert(IsStateValid(p, p.State));

	int64_t answer = ShortestPathToTargetFloor(p, p.TargetFloor);

	printf("[2016] Puzzle11_A: %" PRId64 "\n", answer);
}


static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);
	p.State.push_back(1);
	p.State.push_back(1);
	p.State.push_back(1);
	p.State.push_back(1);
	p.TargetFloor = 4;

	assert(IsStateValid(p, p.State));

	int64_t answer = ShortestPathToTargetFloor(p, p.TargetFloor);

	printf("[2016] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2016(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
