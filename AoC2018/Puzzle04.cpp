#include "stdafx.h"
#include <chrono>

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2018_Types
{
}

using namespace Puzzle04_2018_Types;

static vector<pair<int64_t, vector<int64_t>>> ParseShifts(istream& input)
{
	vector<string> calendar = ReadAllLines(input);
	ranges::sort(calendar);

	const regex beginShiftPattern{ R"(\[\d\d\d\d-(\d\d)-(\d\d) (\d\d):(\d\d)\] Guard #(\d+) begins shift)" };
	const regex fallsAsleepPattern{ R"(\[\d\d\d\d-(\d\d)-(\d\d) (\d\d)\:(\d\d)\] falls asleep)" };
	const regex wakesUpPattern{ R"(\[\d\d\d\d-(\d\d)-(\d\d) (\d\d)\:(\d\d)\] wakes up)" };

	auto mm_dd_hh_mm = [](const smatch& m) -> tuple<int64_t, int64_t, int64_t, int64_t>
		{
			return {
				atoll(m[1].str().c_str()),
				atoll(m[2].str().c_str()),
				atoll(m[3].str().c_str()),
				atoll(m[4].str().c_str())
			};
		};

	vector<pair<int64_t, vector<int64_t>>> shifts;
	for (const string& line : calendar)
	{
		smatch m;
		if (regex_match(line, m, beginShiftPattern))
		{
			auto [month, day, hour, min] = mm_dd_hh_mm(m);

			if (hour == 23)
			{
				hour = 0;
				min = 0;
			}

			int64_t newGuard = atoll(m[5].str().c_str());
			shifts.push_back({ newGuard, { min } });
		}
		else if (regex_match(line, m, fallsAsleepPattern))
		{
			auto [month, day, hour, min] = mm_dd_hh_mm(m);
			shifts.back().second.push_back(min);
		}
		else if (regex_match(line, m, wakesUpPattern))
		{
			auto [month, day, hour, min] = mm_dd_hh_mm(m);
			shifts.back().second.push_back(min);
		}
		else
		{
			assert(false);
		}
	}

	return shifts;
}

static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, vector<int64_t>>> shifts = ParseShifts(input);

	MostCounter<int64_t> guardSleepMinutes;
	map<int64_t, MostCounter<int64_t>> guardSleepPerMinuteCount;
	for (const auto& guardShift : shifts)
	{
		int64_t guard = guardShift.first;
		const vector<int64_t>& shift = guardShift.second;

		bool awake = false;
		size_t eventIndex = 0;
		for (int64_t min = 0; min < 60; min++)
		{
			if ((eventIndex < shift.size()) && (min == shift[eventIndex]))
			{
				eventIndex++;
				awake = !awake;
			}

			if (!awake)
			{
				guardSleepMinutes.AddCount(guard);
				guardSleepPerMinuteCount[guard].AddCount(min);
			}
		}
	}

	int64_t sleepiestGuard = guardSleepMinutes.GetMostId();
	int64_t sleepiestMinute = guardSleepPerMinuteCount.at(sleepiestGuard).GetMostId();

	int64_t answer = sleepiestGuard * sleepiestMinute;

	printf("[2018] Puzzle04_A: %" PRId64 "\n", answer);
}


static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<int64_t, vector<int64_t>>> shifts = ParseShifts(input);

	MostCounter<pair<int64_t, int64_t>> perGuardPerMinuteSleep;
	for (const auto& guardShift : shifts)
	{
		int64_t guard = guardShift.first;
		const vector<int64_t>& shift = guardShift.second;

		bool awake = false;
		size_t eventIndex = 0;
		for (int64_t min = 0; min < 60; min++)
		{
			if ((eventIndex < shift.size()) && (min == shift[eventIndex]))
			{
				eventIndex++;
				awake = !awake;
			}

			if (!awake)
			{
				perGuardPerMinuteSleep.AddCount({ guard, min });
			}
		}
	}

	int64_t answer = perGuardPerMinuteSleep.GetMostId().first * perGuardPerMinuteSleep.GetMostId().second;

	printf("[2018] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2018(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
