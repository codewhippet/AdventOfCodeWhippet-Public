#include "stdafx.h"
#include <bit>

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2015_Types
{
}

using namespace Puzzle24_2015_Types;

static vector<int64_t> ParseWeights(istream& input)
{
	vector<int64_t> weights;
	for (const string& line : ReadEachLine(input))
	{
		weights.push_back(atoll(line.c_str()));
	}
	return weights;
}

static void FormGroupsOfTargetSize(const vector<int64_t>& weights,
	int64_t targetGroupWeight,
	set<uint64_t> *groupsOfTargetWeight,
	set<pair<uint64_t, uint64_t>> *groupSizes)
{
	uint64_t combinationCount = 1ll << weights.size();

	for (uint64_t group = 1; group < combinationCount; group++)
	{
		int64_t groupWeight = 0;
		for (size_t package = 0; package < weights.size(); package++)
		{
			if ((group >> package) & 0x1)
			{
				groupWeight += weights[package];
				if (groupWeight > targetGroupWeight)
				{
					continue;
				}
			}
		}
		if (groupWeight == targetGroupWeight)
		{
			groupsOfTargetWeight->insert(group);
			groupSizes->insert({ popcount(group), group });
		}
	}
}

static vector<int64_t> CalculateQuantumEntanglements(const vector<int64_t>& weights, const set<pair<uint64_t, uint64_t>>& smallestValidGroups)
{
	vector<int64_t> quantumEntanglements;
	ranges::copy(smallestValidGroups |
		views::transform([&](const auto& kvp)
			{
				uint64_t quantumEntanglement = 1;
				for (size_t package = 0; package < weights.size(); package++)
				{
					if ((kvp.second >> package) & 0x1)
					{
						quantumEntanglement *= weights[package];
					}
				}
				return quantumEntanglement;
			}),
		back_inserter(quantumEntanglements));
	return quantumEntanglements;
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> weights = ParseWeights(input);

	int64_t totalWeight = accumulate(weights.begin(), weights.end(), 0ll);
	int64_t targetGroupWeight = totalWeight / 3;

	set<uint64_t> groupsOfTargetWeight;
	set<pair<uint64_t, uint64_t>> groupSizes;
	FormGroupsOfTargetSize(weights, targetGroupWeight, &groupsOfTargetWeight, &groupSizes);

	uint64_t combinationMask = (1ll << weights.size()) - 1;
	set<pair<uint64_t, uint64_t>> smallestValidGroups;
	for (const auto& kvp : groupSizes)
	{
		if ((smallestValidGroups.empty() == false) && (kvp.first > smallestValidGroups.begin()->first))
		{
			break;
		}

		// Look for a non-overlapping second group where the remaining bits are also a possible group
		uint64_t candidatePassengerGroup = kvp.second;
		for (uint64_t secondGroup : groupsOfTargetWeight)
		{
			// Non-overlapping group
			if ((secondGroup & candidatePassengerGroup) != 0)
			{
				continue;
			}

			uint64_t thirdGroup = combinationMask & ~candidatePassengerGroup & ~secondGroup;
			if (groupsOfTargetWeight.contains(thirdGroup))
			{
				smallestValidGroups.insert(kvp);
				break;
			}
		}
	}

	vector<int64_t> quantumEntanglements = CalculateQuantumEntanglements(weights, smallestValidGroups);

	int64_t answer = ranges::min(quantumEntanglements);

	printf("[2015] Puzzle24_A: %" PRId64 "\n", answer);
}


static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> weights = ParseWeights(input);

	int64_t totalWeight = accumulate(weights.begin(), weights.end(), 0ll);
	int64_t targetGroupWeight = totalWeight / 4;

	set<uint64_t> groupsOfTargetWeight;
	set<pair<uint64_t, uint64_t>> groupSizes;
	FormGroupsOfTargetSize(weights, targetGroupWeight, &groupsOfTargetWeight, &groupSizes);

	uint64_t combinationMask = (1ll << weights.size()) - 1;
	set<pair<uint64_t, uint64_t>> smallestValidGroups;
	for (const auto& kvp : groupSizes)
	{
		if ((smallestValidGroups.empty() == false) && (kvp.first > smallestValidGroups.begin()->first))
		{
			break;
		}

		size_t startingSmallestValidGroupsSize = smallestValidGroups.size();

		// Look for non-overlapping second and third groups where the remaining bits are also a possible group
		uint64_t candidatePassengerGroup = kvp.second;
		for (uint64_t secondGroup : groupsOfTargetWeight)
		{
			// Non-overlapping group
			if ((secondGroup & candidatePassengerGroup) != 0)
			{
				continue;
			}

			for (uint64_t thirdGroup : groupsOfTargetWeight)
			{
				// Non-overlapping group
				if (((thirdGroup & candidatePassengerGroup) | (thirdGroup & secondGroup)) != 0)
				{
					continue;
				}

				uint64_t fourthGroup = combinationMask & ~candidatePassengerGroup & ~secondGroup & ~thirdGroup;
				if (groupsOfTargetWeight.contains(fourthGroup))
				{
					smallestValidGroups.insert(kvp);
					break;
				}
			}

			if (smallestValidGroups.size() != startingSmallestValidGroupsSize)
			{
				break;
			}
		}
	}

	vector<int64_t> quantumEntanglements = CalculateQuantumEntanglements(weights, smallestValidGroups);

	int64_t answer = ranges::min(quantumEntanglements);

	printf("[2015] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2015(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
