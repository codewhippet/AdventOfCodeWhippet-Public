#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2021_Types
{
	enum class CaveType
	{
		Start,
		End,
		Small,
		Big
	};

	struct Cave
	{
		string Id;
		CaveType Type;
		vector<string> Neighbours;
	};
}

using namespace Puzzle12_2021_Types;

static CaveType Categorise(const string& s)
{
	if (s == "start") return CaveType::Start;
	if (s == "end") return CaveType::End;
	if (ranges::all_of(s, [](char c) { return islower(c); })) return CaveType::Small;
	return CaveType::Big;
}

static unordered_map<string, Cave> ReadCaveSystem(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	regex lineReg(R"((\w+)-(\w+))");

	unordered_map<string, Cave> caveSystem;
	for (const string& line : lines)
	{
		smatch match;
		regex_match(line, match, lineReg);

		Cave caveA{ match[1].str(), Categorise(match[1].str()), {} };
		Cave caveB{ match[2].str(), Categorise(match[2].str()), {} };

		caveSystem.insert(make_pair(caveA.Id, caveA));
		caveSystem.insert(make_pair(caveB.Id, caveB));

		caveSystem[caveA.Id].Neighbours.push_back(caveB.Id);
		caveSystem[caveB.Id].Neighbours.push_back(caveA.Id);
	}

	return caveSystem;
}

static int Explore(const unordered_map<string, Cave>& caveSystem, const string& currentId, set<string> visitedSmallCaves)
{
	const Cave& currentCave = caveSystem.at(currentId);
	if (currentCave.Type == CaveType::End)
	{
		return 1;
	}

	if ((currentCave.Type == CaveType::Small) || (currentCave.Type == CaveType::Start))
	{
		if (visitedSmallCaves.insert(currentCave.Id).second == false)
		{
			return 0;
		}
	}

	int explorationValue = 0;
	for (const string& neighbour : currentCave.Neighbours)
	{
		explorationValue += Explore(caveSystem, neighbour, visitedSmallCaves);
	}
	return explorationValue;
}

static int Explore2(const unordered_map<string, Cave>& caveSystem, const string& currentId, map<string, int> visitedSmallCaves, bool usedJoker)
{
	const Cave& currentCave = caveSystem.at(currentId);
	if (currentCave.Type == CaveType::End)
	{
		// We have found a valid path
		return 1;
	}

	if (currentCave.Type == CaveType::Start)
	{
		// We can only visit the starting cave once
		if (visitedSmallCaves.insert(make_pair(currentCave.Id, 1)).second == false)
		{
			return 0;
		}
	}

	if (currentCave.Type == CaveType::Small)
	{
		int visitCount = ++visitedSmallCaves[currentCave.Id];

		// We can visit one small cave twice
		if (visitCount > 1)
		{
			if (usedJoker)
			{
				return 0;
			}
			else
			{
				usedJoker = true;
			}
		}

		// But no small cave more than twice
		if (visitCount > 2)
		{
			return 0;
		}
	}

	int explorationValue = 0;
	for (const string& neighbour : currentCave.Neighbours)
	{
		explorationValue += Explore2(caveSystem, neighbour, visitedSmallCaves, usedJoker);
	}
	return explorationValue;
}

static void Puzzle12_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	unordered_map<string, Cave> caveSystem = ReadCaveSystem(input);

	answer = Explore(caveSystem, "start", {});

	printf("[2021] Puzzle12_A: %" PRId64 "\n", answer);
}

static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	unordered_map<string, Cave> caveSystem = ReadCaveSystem(input);

	answer = Explore2(caveSystem, "start", {}, false);

	printf("[2021] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2021(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
