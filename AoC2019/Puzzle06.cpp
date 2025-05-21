#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2019_Types
{
}

using namespace Puzzle06_2019_Types;

static void Puzzle06_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto xOrbitsY = ReadEachLine(input, regex{ R"((\w+)\)(\w+))" })
		| views::transform([](const auto& m) -> pair<string, string>
			{
				return { m[1].str(), m[2].str() }; 
			});

	multimap<string, string> edges;
	ranges::copy(xOrbitsY, inserter(edges, edges.end()));

	map<string, int64_t> visited;
	vector<pair<string, int64_t>> searchQueue{ { "COM", 0 } };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPlanet, currentSteps] = searchQueue[i];
		if (visited.insert({ currentPlanet, currentSteps }).second == false)
			continue;

		auto outerPlanets = edges.equal_range(currentPlanet);
		ranges::copy(ranges::subrange(outerPlanets.first, outerPlanets.second)
			| views::transform([&](const auto& kvp) -> pair<string, int64_t>
				{
					return { kvp.second, currentSteps + 1 };
				}),
			back_inserter(searchQueue));
	}

	auto allDistances = visited | views::values;
	int64_t answer = accumulate(allDistances.begin(), allDistances.end(), 0ll);

	printf("[2019] Puzzle06_A: %" PRId64 "\n", answer);
}


static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto xOrbitsY = ReadEachLine(input, regex{ R"((\w+)\)(\w+))" })
		| views::transform([](const auto& m) -> pair<string, string>
			{
				return { m[1].str(), m[2].str() };
			});

	multimap<string, string> edges;
	ranges::for_each(xOrbitsY, [&](const auto& kvp)
		{
			edges.insert({ kvp.first, kvp.second });
			edges.insert({ kvp.second, kvp.first });
		});

	int64_t answer = 0;

	map<string, int64_t> visited;
	vector<pair<string, int64_t>> searchQueue{ { "YOU", 0 } };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		auto [currentPlanet, currentSteps] = searchQueue[i];
		if (visited.insert({ currentPlanet, currentSteps }).second == false)
			continue;

		if (currentPlanet == "SAN")
		{
			answer = currentSteps - 2;
			break;
		}

		auto outerPlanets = edges.equal_range(currentPlanet);
		for (const auto& kvp : ranges::subrange(outerPlanets.first, outerPlanets.second))
		{
			searchQueue.push_back({ kvp.second, currentSteps + 1 });
		}
	}

	printf("[2019] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2019(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
