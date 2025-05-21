#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2015_Types
{
}

using namespace Puzzle20_2015_Types;

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t presentsPerElf = 10;

	string targetAsString;
	getline(input, targetAsString);

	int64_t target = atoll(targetAsString.c_str());

	vector<int64_t> houses;
	houses.resize(target / presentsPerElf);

	for (size_t elf = 1; elf < houses.size(); elf++)
	{
		for (size_t house = elf; house < houses.size(); house += elf)
		{
			houses[house] += elf * presentsPerElf;
		}
	}

	auto firstHouse = find_if(houses.begin(), houses.end(),
		[&](int64_t h)
		{
			return h >= target;
		});

	int64_t answer = distance(houses.begin(), firstHouse);

	printf("[2015] Puzzle20_A: %" PRId64 "\n", answer);
}


static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t presentsPerElf = 11;
	const int64_t elfStopAfter = 50;

	string targetAsString;
	getline(input, targetAsString);

	int64_t target = atoll(targetAsString.c_str());

	vector<int64_t> houses;
	houses.resize(target / presentsPerElf);

	for (size_t elf = 1; elf < houses.size(); elf++)
	{
		for (int64_t presentStop = 1; presentStop <= elfStopAfter; presentStop++)
		{
			size_t house = elf * presentStop;
			if (house < houses.size())
			{
				houses[house] += elf * presentsPerElf;
			}
		}
	}

	auto firstHouse = find_if(houses.begin(), houses.end(),
		[&](int64_t h)
		{
			return h >= target;
		});

	int64_t answer = distance(houses.begin(), firstHouse);

	printf("[2015] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2015(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
