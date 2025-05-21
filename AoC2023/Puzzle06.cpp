#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle06_2023_Types
{
}

using namespace Puzzle06_2023_Types;

static string RemoveSpaces(string s)
{
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	return s;
}

static void Puzzle06_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> times = ReadAsVectorOfNumbers(ReadSingleLine(input).substr(sizeof("Time:")));
	vector<int64_t> distances = ReadAsVectorOfNumbers(ReadSingleLine(input).substr(sizeof("Distance:")));

	int64_t answer = 1;
	for (size_t race = 0; race < times.size(); race++)
	{
		int64_t raceTime = times[race];
		int64_t recordDistance = distances[race];

		int64_t winningCombinations = (int64_t)ranges::count_if(ranges::iota_view(1ll, raceTime),
			[&](int64_t holdTime)
			{
				int64_t timeLeft = raceTime - holdTime;
				int64_t distancePossible = timeLeft * holdTime;
				return distancePossible > recordDistance;
			});
		answer *= winningCombinations;
	}

	printf("[2023] Puzzle06_A: %" PRId64 "\n", answer);
}

static void Puzzle06_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t uberTime = atoll(RemoveSpaces(ReadSingleLine(input).substr(sizeof("Time:"))).c_str());
	int64_t uberDistance = atoll(RemoveSpaces(ReadSingleLine(input).substr(sizeof("Distance:"))).c_str());

	int64_t answer = (int64_t)ranges::count_if(ranges::iota_view(1ll, uberTime),
		[&](int64_t holdTime)
		{
			int64_t timeLeft = uberTime - holdTime;
			int64_t distancePossible = timeLeft * holdTime;
			return distancePossible > uberDistance;
		});

	printf("[2023] Puzzle06_B: %" PRId64 "\n", answer);
}

void Puzzle06_2023(const string& filename)
{
	Puzzle06_A(filename);
	Puzzle06_B(filename);
}
