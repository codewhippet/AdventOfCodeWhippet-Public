#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2017_Types
{
	struct Particle
	{
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Acceleration;
	};
}

using namespace Puzzle20_2017_Types;

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	int64_t particleIndex = 0;
	int64_t smallestAcceleration = numeric_limits<int64_t>::max();
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>(input, "p=<%lld,%lld,%lld>, v=<%lld,%lld,%lld>, a=<%lld,%lld,%lld>"))
	{
		int64_t acceleration = abs(get<6>(line)) + abs(get<7>(line)) + abs(get<8>(line));
		if (acceleration < smallestAcceleration)
		{
			smallestAcceleration = acceleration;
			answer = particleIndex;
		}
		particleIndex++;
	}

	printf("[2017] Puzzle20_A: %" PRId64 "\n", answer);
}


static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Particle> particles;
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>(input, "p=<%lld,%lld,%lld>, v=<%lld,%lld,%lld>, a=<%lld,%lld,%lld>"))
	{
		particles.push_back(Particle{
			Vector3{ get<0>(line), get<1>(line), get<2>(line) },
			Vector3{ get<3>(line), get<4>(line), get<5>(line) },
			Vector3{ get<6>(line), get<7>(line), get<8>(line) },
			});
	}

	for (int64_t i = 0; i < 1000; i++)
	{
		// Collide
		set<Vector3> collisionLocations;
		set<size_t> partsToRemove;
		for (const auto& p : AllUnorderedPairs(particles.size()))
		{
			if (particles[p.first].Position == particles[p.second].Position)
			{
				collisionLocations.insert(particles[p.first].Position);
				partsToRemove.insert(p.first);
				partsToRemove.insert(p.second);
			}
		}

		particles.erase(remove_if(particles.begin(), particles.end(), [&](const Particle& p) { return collisionLocations.contains(p.Position); }), particles.end());

		// Simulate
		for (Particle& p : particles)
		{
			p.Velocity = p.Velocity + p.Acceleration;
			p.Position = p.Position + p.Velocity;
		}
	}

	int64_t answer = particles.size();

	printf("[2017] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2017(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
