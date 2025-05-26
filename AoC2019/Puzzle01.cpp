#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2019_Types
{
}

using namespace Puzzle01_2019_Types;

static int64_t FuelRequired(int64_t mass)
{
	return (mass / 3) - 2;
}

static int64_t FuelRequiredNonZero(int64_t mass)
{
	return max(FuelRequired(mass), 0ll);
}

static int64_t FuelForMassAndFuel(int64_t mass)
{
	int64_t totalFuelNeeded = 0;
	while (mass > 0)
	{
		int64_t additionalFuel = FuelRequiredNonZero(mass);
		totalFuelNeeded += additionalFuel;
		mass = additionalFuel;
	}
	return totalFuelNeeded;
}

static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto fuel = ScanfEachLine<int64_t>(input, "%lld")
		| views::elements<0>
		| views::transform(FuelRequired)
		| views::common;
	int64_t answer = accumulate(fuel.begin(), fuel.end(), 0ll);

	printf("[2019] Puzzle01_A: %" PRId64 "\n", answer);
}


static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto fuel = ScanfEachLine<int64_t>(input, "%lld")
		| views::elements<0>
		| views::transform(FuelForMassAndFuel)
		| views::common;
	int64_t answer = accumulate(fuel.begin(), fuel.end(), 0ll);

	printf("[2019] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2019(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
