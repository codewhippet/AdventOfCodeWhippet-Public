#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2015_Types
{
	struct Reindeer
	{
		string Name;
		int64_t Speed;
		int64_t DistancePerQuantum;
		int64_t QuantumDuration;
	};
}

using namespace Puzzle14_2015_Types;

static vector<Reindeer> ParseReindeers(istream& input)
{
	vector<Reindeer> reindeers;
	for (const string& line : ReadEachLine(input))
	{
		Reindeer r;

		char name[64] = { 0 };
		int64_t flyDuration;
		int64_t restDuration;
		int scanned = sscanf(line.c_str(), "%s can fly %" SCNd64 " km/s for %" SCNd64 " seconds, but then must rest for %" SCNd64 " seconds.",
			name,
			&r.Speed,
			&flyDuration,
			&restDuration);
		assert(scanned == 4);
		(void)scanned;

		r.Name = name;
		r.DistancePerQuantum = flyDuration * r.Speed;
		r.QuantumDuration = flyDuration + restDuration;

		reindeers.push_back(move(r));

	}
	return reindeers;
}

static int64_t DistanceTravelled(const Reindeer& reindeer, int64_t time)
{
	int64_t fullQuanta = time / reindeer.QuantumDuration;
	int64_t partialQuantum = time % reindeer.QuantumDuration;

	int64_t fullDurationDistances = fullQuanta * reindeer.DistancePerQuantum;
	int64_t partialDurationDistance = min(reindeer.Speed * partialQuantum, reindeer.DistancePerQuantum);

	return fullDurationDistances + partialDurationDistance;
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t raceDuration = 2503;

	vector<Reindeer> reindeers = ParseReindeers(input);

	vector<pair<int64_t, string>> reindeerDistances;
	ranges::copy(reindeers
		| views::transform([&](const Reindeer& r)
			{
				return make_pair(DistanceTravelled(r, raceDuration), r.Name);
			}),
		back_inserter(reindeerDistances));
	ranges::sort(reindeerDistances);

	int64_t answer = reindeerDistances.back().first;

	printf("[2015] Puzzle14_A: %" PRId64 "\n", answer);
}


static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t raceDuration = 2503;

	vector<Reindeer> reindeers = ParseReindeers(input);

	map<string, int64_t> reindeerScores;

	vector<int64_t> reindeerDistances(reindeers.size());
	for (int64_t i = 0; i < raceDuration; i++)
	{
		int64_t leadingDistance = 0;
		for (size_t r = 0; r < reindeers.size(); r++)
		{
			 int64_t distance = DistanceTravelled(reindeers[r], i + 1);
			 reindeerDistances[r] = distance;
			 leadingDistance = max(distance, leadingDistance);
		}

		for (size_t r = 0; r < reindeers.size(); r++)
		{
			if (reindeerDistances[r] == leadingDistance)
			{
				reindeerScores[reindeers[r].Name]++;
			}
		}
	}

	int64_t answer = ranges::max(reindeerScores | views::transform([](const auto& kvp) { return kvp.second; }));

	printf("[2015] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2015(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
