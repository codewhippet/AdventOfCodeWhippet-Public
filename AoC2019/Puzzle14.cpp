#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2019_Types
{
	struct Reaction
	{
		string Chemical;
		int64_t Created;
		vector<pair<string, int64_t>> Requires;
	};
}

using namespace Puzzle14_2019_Types;

static map<string, Reaction> ParseReactions(istream& input)
{
	const regex reaction{ R"(([^=]+) => (\d+) (\w+))" };
	const regex chemical{ R"((\d+) (\w+))" };

	map<string, Reaction> reactions;
	for (const auto& lineMatch : ReadEachLine(input, reaction))
	{
		Reaction r;
		r.Chemical = lineMatch[3].str();
		r.Created = atoll(lineMatch[2].str().c_str());

		string precursors = lineMatch[1].str();
		auto precursors_begin = sregex_iterator(precursors.begin(), precursors.end(), chemical);
		for (sregex_iterator chem = precursors_begin; chem != sregex_iterator{}; ++chem)
		{
			r.Requires.push_back({ (*chem)[2].str(), atoll((*chem)[1].str().c_str()) });
		}
		reactions[r.Chemical] = r;
	}

	return reactions;
}

static int64_t OreRequiredForFuel(int64_t fuel, const map<string, Reaction>& reactions)
{
	map<string, int64_t> required;
	map<string, int64_t> used;
	map<string, int64_t> overspill;

	required["FUEL"] = fuel;

	int64_t oreNeeded = 0;
	while (required.empty() == false)
	{
		auto [chemical, amountNeeded] = *required.begin();
		required.erase(required.begin());

		if (chemical == "ORE"sv)
		{
			oreNeeded += amountNeeded;
			continue;
		}

		const Reaction& reaction = reactions.at(chemical);

		int64_t numReactions = ((amountNeeded % reaction.Created == 0) ? amountNeeded : (amountNeeded + reaction.Created)) / reaction.Created;
		int64_t amountCreated = numReactions * reaction.Created;

		// Sort out the precursors
		for (const auto& p : reaction.Requires)
		{
			int64_t precursorAmount = p.second * numReactions;

			// Use up overspill first
			int64_t overspillUsed = min(precursorAmount, overspill[p.first]);
			overspill[p.first] -= overspillUsed;
			precursorAmount -= overspillUsed;

			// Put in an order for the rest
			if (precursorAmount > 0)
			{
				required[p.first] += precursorAmount;
			}
		}

		// Top up the overspill with the excess
		overspill[chemical] += amountCreated - amountNeeded;
	}
	return oreNeeded;
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, Reaction> reactions = ParseReactions(input);
	int64_t answer = OreRequiredForFuel(1, reactions);

	printf("[2019] Puzzle14_A: %" PRId64 "\n", answer);
}


static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t maximumOre = 1000000000000;

	map<string, Reaction> reactions = ParseReactions(input);

	int64_t answer = 0;

	int64_t lowerFuel = 1;
	int64_t upperFuel = maximumOre;
	while (true)
	{
		int64_t midpointFuel = (lowerFuel + upperFuel) / 2;
		int64_t oreAtMidpoint = OreRequiredForFuel(midpointFuel, reactions);
		int64_t oreAboveMidpoint = OreRequiredForFuel(midpointFuel + 1, reactions);

		if ((oreAtMidpoint <= maximumOre) && (oreAboveMidpoint > maximumOre))
		{
			answer = midpointFuel;
			break;
		}

		if (oreAtMidpoint > maximumOre)
		{
			upperFuel = midpointFuel;
		}
		else
		{
			lowerFuel = midpointFuel;
		}
	}

	printf("[2019] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2019(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
