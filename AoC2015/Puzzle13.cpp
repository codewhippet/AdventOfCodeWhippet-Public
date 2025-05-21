#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2015_Types
{
	struct Party
	{
		map<char, string> People;
		map<string, char> PeopleCodes;
		map<pair<char, char>, int64_t> Happiness;
	};

}

using namespace Puzzle13_2015_Types;

static char AddOrGetPerson(const char* parsedBuffer, Party* roadmap)
{
	string destination{ parsedBuffer };
	auto existingDestination = roadmap->PeopleCodes.find(destination);
	if (existingDestination == roadmap->PeopleCodes.end())
	{
		char newCode = 'a' + (char)roadmap->PeopleCodes.size();
		roadmap->People[newCode] = destination;
		existingDestination = roadmap->PeopleCodes.insert({ destination, newCode }).first;
	}
	return existingDestination->second;
}

static Party ParseParty(istream& input)
{
	Party party;
	for (const string& line : ReadEachLine(input))
	{
		char from[64] = { 0 };
		char to[64] = { 0 };
		char action[64] = { 0 };
		int64_t happiness;
		int scanned = sscanf(line.c_str(), "%[A-Za-z] would %s %" SCNd64 " happiness units by sitting next to %[A-Za-z].",
			from,
			action,
			&happiness,
			to);
		assert(scanned == 4);
		(void)scanned;

		happiness = (strcmp(action, "lose") == 0) ? -happiness : happiness;

		char fromCode = AddOrGetPerson(from, &party);
		char toCode = AddOrGetPerson(to, &party);

		party.Happiness.insert({ { fromCode, toCode }, happiness });
	}
	return party;
}

static int64_t PartyHappiness(const string& route, const Party& party)
{
	int64_t happiness = 0;
	for (size_t i = 0; i < route.size(); i++)
	{
		happiness += party.Happiness.at({ route[i + 0], route[(i + 1) % route.size()] });
		happiness += party.Happiness.at({ route[(i + 1) % route.size()], route[i + 0] });
	}
	return happiness;
}

static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Party party = ParseParty(input);

	string arrangement;
	ranges::copy(party.People
		| views::transform([](const auto& kvp)
			{
				return kvp.first;
			}),
		back_inserter(arrangement));

	int64_t answer = 0;
	do
	{
		answer = max(answer, PartyHappiness(arrangement, party));

	} while (next_permutation(arrangement.begin(), arrangement.end()));

	printf("[2015] Puzzle13_A: %" PRId64 "\n", answer);
}


static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Party party = ParseParty(input);

	string arrangement;
	ranges::copy(party.People
		| views::transform([](const auto& kvp)
			{
				return kvp.first;
			}),
		back_inserter(arrangement));

	char myCode = AddOrGetPerson("me", &party);
	for (char c : arrangement)
	{
		party.Happiness.insert({ { c, myCode }, 0 });
		party.Happiness.insert({ { myCode, c }, 0 });
	}
	arrangement += myCode;

	int64_t answer = 0;
	do
	{
		answer = max(answer, PartyHappiness(arrangement, party));

	} while (next_permutation(arrangement.begin(), arrangement.end()));

	printf("[2015] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2015(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
