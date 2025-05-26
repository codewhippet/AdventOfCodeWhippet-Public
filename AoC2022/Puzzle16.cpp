#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2022_Types
{
	struct Puzzle
	{
		int64_t NumberOfUsefulValves = 0;
		map<string, int64_t> ValvePatterns;
		vector<int64_t> ValveFlowRates;
		vector<int64_t> ValveToValveDistance;
	};

	struct SearchNode
	{
		int64_t CurrentValve;
		int64_t PressureReleased;
		int64_t MinutesRemaining;
		int64_t OpenValves;
	};
}

using namespace Puzzle16_2022_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle puzzle;

	const regex linePattern{ R"(Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? (.+))" };

	NameDictionary valveIds;
	vector<vector<int64_t>> edges;
	for (const auto& m : ReadEachLine(input, linePattern))
	{
		string valveName = m[1].str();
		int64_t valveId = valveIds.IdFromName(valveName);
		int64_t valveFlow = Utils::ToNumber(m[2]);
		
		assert((valveName != "AA"sv) || (valveFlow == 0));

		edges.resize(max<int64_t>(edges.size(), valveId + 1));
		edges[valveId] = m[3].str()
			| views::lazy_split(", "sv)
			| views::transform([](const auto &r) { return ranges::to<string>(r); })
			| views::transform([&](const auto &s) { return valveIds.IdFromName(s); })
			| ranges::to<vector>();

		if ((valveName == "AA"sv) || (valveFlow > 0))
		{
			int64_t valveBit = 1ll << puzzle.NumberOfUsefulValves++;
			puzzle.ValveFlowRates.resize(valveBit + 1);
			puzzle.ValveFlowRates[valveBit] = valveFlow;

			puzzle.ValvePatterns[valveName] = valveBit;
		}
	}

	//
	// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
	//

	vector<vector<int64_t>> distances{ edges.size() };
	ranges::for_each(distances,
		[&](auto& v)
		{
			v.resize(edges.size());
			ranges::fill(v, numeric_limits<int64_t>::max() / 2);
		});

	for (int64_t i = 0; i < (int64_t)edges.size(); i++)
	{
		for (int64_t j : edges[i])
		{
			distances[i][j] = 1;
		}
	}
	for (int64_t i = 0; i < (int64_t)edges.size(); i++)
	{
		distances[i][i] = 0;
	}

	for (int64_t k = 0; k < (int64_t)edges.size(); k++)
	{
		for (int64_t i = 0; i < (int64_t)edges.size(); i++)
		{
			for (int64_t j = 0; j < (int64_t)edges.size(); j++)
			{
				int64_t& dist_ij = distances[i][j];
				const int64_t dist_ik = distances[i][k];
				const int64_t dist_kj = distances[k][j];

				if (dist_ij > (dist_ik + dist_kj))
				{
					dist_ij = dist_ik + dist_kj;
				}
			}
		}
	}

	puzzle.ValveToValveDistance.resize(1ll << puzzle.NumberOfUsefulValves);
	ranges::fill(puzzle.ValveToValveDistance, numeric_limits<int64_t>::max());

	for (const auto &valve_i : puzzle.ValvePatterns)
	{
		for (const auto &valve_j : puzzle.ValvePatterns)
		{
			puzzle.ValveToValveDistance[valve_i.second | valve_j.second] = distances[valveIds.IdFromName(valve_i.first)][valveIds.IdFromName(valve_j.first)];
		}
	}

	return puzzle;
}

// I wasn't happy with the runtime of my original C# solution; it required a parallel for to explore combinations in part 2 in a reasonable time.
// I implemented the approach outlined here instead: https://www.reddit.com/r/adventofcode/comments/zo21au/comment/j0nz8df/
static vector<int64_t> FindMaximumPressureCombinations(const Puzzle& puzzle, const string& startingRoom, int64_t minutesLeft)
{
	const int64_t startingValve = puzzle.ValvePatterns.at(startingRoom);

	vector<int64_t> maximumPressures;
	maximumPressures.resize(1ll << puzzle.NumberOfUsefulValves);

	vector<SearchNode> searchQueue{{
			.CurrentValve = startingValve,
			.PressureReleased = 0,
			.MinutesRemaining = minutesLeft,
			.OpenValves = startingValve,
		}};
	searchQueue.reserve((1ull << puzzle.NumberOfUsefulValves) * 4);

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		const SearchNode current = searchQueue[i];

		// Have we found a new best combination?
		int64_t &existingMaximum = maximumPressures[current.OpenValves];
		if (current.PressureReleased > existingMaximum)
		{
			existingMaximum = current.PressureReleased;
		}

		// Go through the unopened valves
		for (int64_t valveIndex = 0; valveIndex < puzzle.NumberOfUsefulValves; valveIndex++)
		{
			int64_t nextValve = 1ll << valveIndex;
			if (current.OpenValves & nextValve)
				continue;

			int64_t travelAndOpeningTime = puzzle.ValveToValveDistance[current.CurrentValve | nextValve] + 1;
			int64_t timeAfterTravelAndOpening = current.MinutesRemaining - travelAndOpeningTime;
			if (timeAfterTravelAndOpening > 0)
			{
				int64_t openingWillReleasePressureOf = timeAfterTravelAndOpening * puzzle.ValveFlowRates[nextValve];
				searchQueue.push_back({
						.CurrentValve = nextValve,
						.PressureReleased = current.PressureReleased + openingWillReleasePressureOf,
						.MinutesRemaining = timeAfterTravelAndOpening,
						.OpenValves = current.OpenValves | nextValve,
					});
			}
		}
	}

	return maximumPressures;
}

static void Puzzle16_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);

	vector<int64_t> maximumCombinations = FindMaximumPressureCombinations(puzzle, "AA", 30);
	int64_t answer = ranges::max(maximumCombinations);

	printf("[2022] Puzzle16_A: %" PRId64 "\n", answer);
}

static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const string startingRoom = "AA";

	Puzzle puzzle = ReadPuzzle(input);
	vector<tuple<int64_t, int64_t>> maximumCombinations = FindMaximumPressureCombinations(puzzle, startingRoom, 26)
		| views::enumerate
		| views::filter([](const auto &t) { return get<1>(t) > 0; })
		| ranges::to<vector>();

	const int64_t startingValve = puzzle.ValvePatterns.at(startingRoom);

	int64_t answer = 0;

	for (int64_t me = 0; me < (int64_t)maximumCombinations.size(); me++)
	{
		int64_t myContribution = get<1>(maximumCombinations[me]);
		for (int64_t elephant = me + 1; elephant < (int64_t)maximumCombinations.size(); elephant++)
		{
			// Don't bother checking for distinct combination if it's not an improvement
			int64_t elephantContribution = get<1>(maximumCombinations[elephant]);
			if (myContribution + elephantContribution < answer)
				continue;

			// For all combinations where we're not touching the same valves
			// (starting valve is 'open' in all cases)
			int64_t checkDistinct = get<0>(maximumCombinations[me]) & get<0>(maximumCombinations[elephant]) & ~startingValve;
			if (checkDistinct == 0)
			{
				answer = myContribution + elephantContribution;
			}
		}
	}

	printf("[2022] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2022(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
