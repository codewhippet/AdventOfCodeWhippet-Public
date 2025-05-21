#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2023_Types
{
	struct Module
	{
		string ModuleId;
		char Type;
		vector<pair<string, size_t>> ConnectedTo;
		vector<bool> History;
	};

	struct Pulse
	{
		string From;
		string To;
		bool Value;
		size_t ConnectionIndex;
	};
}

using namespace Puzzle20_2023_Types;

static map<string, Module> ReadModules(istream& input)
{
	map<string, Module> modules;

	vector<string> lines = ReadAllLines(input);
	for (const string& line : lines)
	{
		Module m;
		char moduleId[64];
		char connections[64];
		int scanned = sscanf(line.c_str(), R"(%c%63s -> %63[abcdefghijklmnopqrstuvwxyz, ])",
			&m.Type,
			&moduleId[0],
			&connections[0]);
		assert(scanned == 3);
		(void)scanned;

		m.ModuleId = &moduleId[0];
		m.ConnectedTo = MakeEnumerator(StringSplit(&connections[0], ','))
			->Select<pair<string, size_t>>([](const string& s) { return make_pair<string, size_t>(Trim(s), 0); })
			->ToVector();

		modules[m.ModuleId] = m;
	}

	for (map<string, Module>::reference module : modules)
	{
		if (module.second.Type == '%')
		{
			module.second.History.resize(1, false);
		}

		for (pair<string, size_t>& connection : module.second.ConnectedTo)
		{
			if (modules[connection.first].Type == '&')
			{
				connection.second = modules[connection.first].History.size();
				modules[connection.first].History.push_back(false);
			}
		}
	}

	return modules;
}

static void GeneratePulses(Module& module, const Pulse& pulseIn, list<Pulse>* pulsesOut)
{
	switch (module.Type)
	{
	case 'b':
		for (const pair<string, size_t>& connection : module.ConnectedTo)
		{
			pulsesOut->push_back({ module.ModuleId, connection.first, pulseIn.Value, connection.second });
		}
		break;

	case '%':
		if (pulseIn.Value == false)
		{
			module.History[0] = !module.History[0];
			for (const pair<string, size_t>& connection : module.ConnectedTo)
			{
				pulsesOut->push_back({ module.ModuleId, connection.first, module.History[0], connection.second });
			}
		}
		break;

	case '&':
		{
			module.History[pulseIn.ConnectionIndex] = pulseIn.Value;
			bool allTrue = count(module.History.begin(), module.History.end(), true) == (int64_t)module.History.size();
			for (const pair<string, size_t>& connection : module.ConnectedTo)
			{
				pulsesOut->push_back({ module.ModuleId, connection.first, !allTrue, connection.second });
			}
		}
		break;
	}
}

static void Puzzle20_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, Module> modules = ReadModules(input);

	int64_t numHigh = 0;
	int64_t numLow = 0;
	for (int i = 0; i < 1000; i++)
	{
		list<Pulse> pulses = { { "button", "roadcaster", false, 0 } };
		for (list<Pulse>::const_iterator pulse = pulses.begin(); pulse != pulses.end(); ++pulse)
		{
			GeneratePulses(modules[pulse->To], *pulse, &pulses);
		}

		int64_t high = count_if(pulses.begin(), pulses.end(), [](const Pulse& p) { return p.Value; });
		int64_t low = pulses.size() - high;

		numHigh += high;
		numLow += low;
	}

	int64_t answer = numHigh * numLow;

	printf("[2023] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, Module> modules = ReadModules(input);

	const string targetModule{ "rx" };

	auto penultimateModuleFilter = modules
		| views::filter([&](const auto& kvp)
			{
				auto moduleConnections = kvp.second.ConnectedTo | views::elements<0>;
				return ranges::find(moduleConnections, targetModule) != moduleConnections.end();
			})
		| views::keys;
	const string penultimateModule = *penultimateModuleFilter.begin();

	map<size_t, int> subgraphPulses;
	for (int buttonPress = 1; buttonPress < 5000; buttonPress++)
	{
		list<Pulse> pulses = { { "button", "roadcaster", false, 0 } };
		for (list<Pulse>::const_iterator pulse = pulses.begin(); pulse != pulses.end(); ++pulse)
		{
			GeneratePulses(modules[pulse->To], *pulse, &pulses);
		}

		// Look for a high pulse going to the conjunction node feeding rx
		for (const Pulse& p : pulses)
		{
			if (p.To == penultimateModule && p.Value)
			{
				subgraphPulses.insert({ p.ConnectionIndex, buttonPress });
			}
		}

		// There are 4 subgraphs generating pulses
		if (subgraphPulses.size() == 4)
			break;
	}

	// rx will pulse when all subgraphs coincide
	auto cycleLength = subgraphPulses | views::values;
	int64_t answer = accumulate(cycleLength.begin(), cycleLength.end(), 1ll, [](int64_t a, int64_t b) { return a * b; });

	printf("[2023] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2023(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
