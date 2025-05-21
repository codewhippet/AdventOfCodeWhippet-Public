#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2015_Types
{
	struct Roadmap
	{
		map<char, string> Destinations;
		map<string, char> DestinationCodes;
		map<pair<char, char>, int64_t> Distances;
	};
}

using namespace Puzzle09_2015_Types;

static char AddOrGetDestination(const char* parsedBuffer, Roadmap *roadmap)
{
	string destination{ parsedBuffer };
	auto existingDestination = roadmap->DestinationCodes.find(destination);
	if (existingDestination == roadmap->DestinationCodes.end())
	{
		char newCode = 'a' + (char)roadmap->DestinationCodes.size();
		roadmap->Destinations[newCode] = destination;
		existingDestination = roadmap->DestinationCodes.insert({ destination, newCode }).first;
	}
	return existingDestination->second;
}

static Roadmap ParseRoads(istream& input)
{
	Roadmap roadmap;
	for (const string& line : ReadEachLine(input))
	{
		char from[64];
		char to[64];
		int64_t distance;
		int scanned = sscanf(line.c_str(), "%s to %s = %" SCNd64,
			from,
			to,
			&distance);
		assert(scanned == 3);
		(void)scanned;

		char fromCode = AddOrGetDestination(from, &roadmap);
		char toCode = AddOrGetDestination(to, &roadmap);

		roadmap.Distances.insert({ { fromCode, toCode }, distance });
		roadmap.Distances.insert({ { toCode, fromCode }, distance });
	}
	return roadmap;
}

static int64_t RouteDistance(const string& route, const Roadmap& roadmap)
{
	int64_t distance = 0;
	for (size_t i = 0; i + 1 < route.size(); i++)
	{
		distance += roadmap.Distances.at({ route[i + 0], route[i + 1] });
	}
	return distance;
}

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Roadmap roadmap = ParseRoads(input);

	string route;
	ranges::copy(roadmap.Destinations
		| views::transform([](const auto& kvp)
			{
				return kvp.first;
			}),
		back_inserter(route));

	int64_t answer = numeric_limits<int64_t>::max();
	do
	{
		answer = min(answer, RouteDistance(route, roadmap));

	} while (next_permutation(route.begin(), route.end()));

	printf("[2015] Puzzle09_A: %" PRId64 "\n", answer);
}


static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Roadmap roadmap = ParseRoads(input);

	string route;
	ranges::copy(roadmap.Destinations
		| views::transform([](const auto& kvp)
			{
				return kvp.first;
			}),
		back_inserter(route));

	int64_t answer = 0;
	do
	{
		answer = max(answer, RouteDistance(route, roadmap));

	} while (next_permutation(route.begin(), route.end()));

	printf("[2015] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2015(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
