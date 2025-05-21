#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2021_Types
{
	struct Population
	{
		int64_t Timers[9];
	};
}

using namespace Puzzle06_2021_Types;

static int64_t LanternfishAfter(istringstream& input, int days)
{
	Population lanternfishTotals;
	memset(&lanternfishTotals, 0, sizeof(lanternfishTotals));

	int timer;
	while (input >> timer)
	{
		lanternfishTotals.Timers[timer]++;
	}

	for (int day = 0; day < days; day++)
	{
		Population newPopulation;
		newPopulation.Timers[8] = lanternfishTotals.Timers[0];
		newPopulation.Timers[7] = lanternfishTotals.Timers[8];
		newPopulation.Timers[6] = lanternfishTotals.Timers[7] + lanternfishTotals.Timers[0];
		newPopulation.Timers[5] = lanternfishTotals.Timers[6];
		newPopulation.Timers[4] = lanternfishTotals.Timers[5];
		newPopulation.Timers[3] = lanternfishTotals.Timers[4];
		newPopulation.Timers[2] = lanternfishTotals.Timers[3];
		newPopulation.Timers[1] = lanternfishTotals.Timers[2];
		newPopulation.Timers[0] = lanternfishTotals.Timers[1];
		lanternfishTotals = newPopulation;
	}

	int64_t fastAnswer = accumulate(&lanternfishTotals.Timers[0], &lanternfishTotals.Timers[9], 0ll);
	return fastAnswer;
}

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> lanternfish;

	string initialPopulationString;
	getline(input, initialPopulationString);
	for (char& c : initialPopulationString)
	{
		if (c == ',')
			c = ' ';
	}
	istringstream initialPopulationStream(initialPopulationString);

	int64_t answer = LanternfishAfter(initialPopulationStream, 80);

	printf("[2021] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int> lanternfish;

	string initialPopulationString;
	getline(input, initialPopulationString);
	for (char& c : initialPopulationString)
	{
		if (c == ',')
			c = ' ';
	}
	istringstream initialPopulationStream(initialPopulationString);

	int64_t answer = LanternfishAfter(initialPopulationStream, 256);

	printf("[2021] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2021(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
