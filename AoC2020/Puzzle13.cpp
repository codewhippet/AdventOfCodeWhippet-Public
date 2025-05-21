#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2020_Types
{
}

using namespace Puzzle13_2020_Types;


static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> timetable = ReadAllLines(input);

	int64_t startingTime = stoll(timetable[0]);

	vector<int64_t> busses;
	const char *delims = ",x";
	for (char* bus = strtok(const_cast<char *>(timetable[1].c_str()), delims); bus; bus = strtok(nullptr, delims))
	{
		busses.push_back(stoll(bus));
	}

	vector<pair<int64_t, int64_t>> departureTimes = MakeEnumerator(busses)
		->Select<pair<int64_t, int64_t>>([startingTime](int64_t bus)
			{
				int64_t timeSinceLastDeparture = startingTime % bus;
				int64_t timeUntilNextDeparture = (bus - timeSinceLastDeparture) % bus;
				return make_pair(startingTime + timeUntilNextDeparture, bus);
			})
		->ToVector();

	pair<int64_t, int64_t> firstDeparture = MakeEnumerator(departureTimes)->Min();

	int64_t answer = (firstDeparture.first - startingTime) * firstDeparture.second;

	printf("[2020] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> timetable = ReadAllLines(input);

	vector<pair<int64_t, int64_t>> busses;
	const char* delims = ",";
	int64_t busConstraint = 0;
	for (char* bus = strtok(const_cast<char*>(timetable[1].c_str()), delims); bus; bus = strtok(nullptr, delims))
	{
		if (bus[0] != 'x')
		{
			busses.push_back(make_pair(stoll(bus), busConstraint));
		}
		busConstraint++;
	}

	int64_t t = 0;
	int64_t timeIncrement = busses[0].first;

	for (size_t i =  1; i < busses.size(); i++)
	{
		int64_t bus = busses[i].first;
		int64_t constraint = busses[i].second;
		while (((t + constraint) % bus) != 0)
		{
			t += timeIncrement;
		}
		timeIncrement *= bus;
	}

	int64_t answer = t;

	printf("[2020] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2020(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
