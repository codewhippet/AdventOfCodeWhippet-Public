#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2017_Types
{
	using Port = pair<int64_t, int64_t>;
	using Component = pair<Port, uint64_t>;
}

using namespace Puzzle24_2017_Types;

static vector<vector<Component>> ParseComponents(istream &input)
{
	const size_t highestPinCount = 128;
	vector<vector<Component>> ports(highestPinCount);

	for (const auto& [line, lineNumber] : ReadEachLineWithNumber(input))
	{
		assert(lineNumber < 63);

		Port port;
		int scanned = sscanf(line.c_str(), "%lld/%lld", &port.first, &port.second);
		assert(scanned == 2);
		(void)scanned;

		ports[port.first].push_back({ { port.first, port.second }, 1ull << lineNumber });
		if (port.first != port.second)
		{
			ports[port.second].push_back({ { port.second, port.first }, 1ull << lineNumber });
		}
	}

	return ports;
}

static int64_t StrongestBridge(const vector<vector<Component>>& components, int64_t startingPins, uint64_t usedComponents)
{
	int64_t maxStrength = 0;

	for (const Component& component : components[startingPins])
	{
		// Can't re-use components
		if ((component.second & usedComponents) != 0)
			continue;

		int64_t componentStrength = component.first.first + component.first.second;
		int64_t strongestSubBridge = StrongestBridge(components, component.first.second, usedComponents | component.second);
		maxStrength = max(maxStrength, componentStrength + strongestSubBridge);
	}

	return maxStrength;
}

static void BuildAllBridges(const vector<vector<Component>>& components,
	int64_t startingPins,
	uint64_t usedComponents,
	int64_t currentStrength,
	vector<pair<int64_t, int64_t>> *bridges)
{
	bool managedToExtend = false;
	for (const Component& component : components[startingPins])
	{
		// Can't re-use components
		if ((component.second & usedComponents) != 0)
			continue;

		managedToExtend = true;

		int64_t componentStrength = component.first.first + component.first.second;
		BuildAllBridges(components,
			component.first.second,
			usedComponents | component.second,
			currentStrength + componentStrength,
			bridges);
	}

	if (!managedToExtend)
	{
		bridges->push_back({ -popcount(usedComponents), -currentStrength });
	}
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<vector<Component>> ports = ParseComponents(input);

	int64_t answer = StrongestBridge(ports, 0, 0);

	printf("[2017] Puzzle24_A: %" PRId64 "\n", answer);
}


static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<vector<Component>> ports = ParseComponents(input);

	vector<pair<int64_t, int64_t>> bridges;
	BuildAllBridges(ports, 0, 0, 0, &bridges);

	ranges::sort(bridges);
	int64_t answer = -bridges.front().second;

	printf("[2017] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2017(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
