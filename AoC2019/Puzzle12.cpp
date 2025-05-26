#include "stdafx.h"
#include <numeric>

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2019_Types
{
	struct Moon
	{
		Vector3 Position;
		Vector3 Velocity;
	};
}

using namespace Puzzle12_2019_Types;

static int64_t PotentialEnergy(const Moon& m)
{
	return abs(m.Position.X) + abs(m.Position.Y) + abs(m.Position.Z);
}

static int64_t KineticEnergy(const Moon& m)
{
	return abs(m.Velocity.X) + abs(m.Velocity.Y) + abs(m.Velocity.Z);
}

static int64_t TotalEnergy(const Moon& m)
{
	return PotentialEnergy(m) * KineticEnergy(m);
}

static int64_t GetX(const Vector3& v) { return v.X; }
static int64_t GetY(const Vector3& v) { return v.Y; }
static int64_t GetZ(const Vector3& v) { return v.Z; }

static void SetX(Vector3* v, int64_t x) { v->X = x; }
static void SetY(Vector3* v, int64_t y) { v->Y = y; }
static void SetZ(Vector3* v, int64_t z) { v->Z = z; }

template<typename GETTER, typename SETTER>
void ApplyGravity(const pair<Moon*, Moon*>& p, const GETTER& get, const SETTER& set)
{
	Moon* a = p.first;
	Moon* b = p.second;

	if (get(a->Position) < get(b->Position))
	{
		set(&a->Velocity, get(a->Velocity) + 1);
		set(&b->Velocity, get(b->Velocity) - 1);
	}
	else if (get(a->Position) > get(b->Position))
	{
		set(&a->Velocity, get(a->Velocity) - 1);
		set(&b->Velocity, get(b->Velocity) + 1);
	}
}

static void ApplyGravityXYZ(const pair<Moon*, Moon*> &p)
{
	ApplyGravity(p, GetX, SetX);
	ApplyGravity(p, GetY, SetY);
	ApplyGravity(p, GetZ, SetZ);
}

template<typename GETTER, typename SETTER>
void ApplyVelocity(Moon* m, const GETTER& get, const SETTER& set)
{
	set(&m->Position, get(m->Position) + get(m->Velocity));
}

static void ApplyVelocityXYZ(Moon* m)
{
	ApplyVelocity(m, GetX, SetX);
	ApplyVelocity(m, GetY, SetY);
	ApplyVelocity(m, GetZ, SetZ);
}

static const Vector3& GetPosition(const Moon& m)
{
	return m.Position;
}

static const Vector3& GetVelocity(const Moon& m)
{
	return m.Velocity;
}

template<typename GETTER, typename SETTER>
static pair<int64_t, int64_t> FindCycle(vector<Moon> moons, const GETTER& get, const SETTER& set)
{
	int64_t cycleStart;
	int64_t cycleLength;

	map<vector<int64_t>, int64_t> states;
	for (int64_t frame = 0; frame < numeric_limits<int64_t>::max() - 1; frame++)
	{
		vector<int64_t> currentState;
		currentState.reserve(moons.size() * 2);
		ranges::copy(moons | views::transform(GetPosition) | views::transform(get), back_inserter(currentState));
		ranges::copy(moons | views::transform(GetVelocity) | views::transform(get), back_inserter(currentState));

		auto tryInsert = states.insert({ currentState, frame });
		if (tryInsert.second == false)
		{
			cycleStart = tryInsert.first->second;
			cycleLength = frame - cycleStart;
			break;
		}

		auto applyGravityAxis = [&](const pair<Moon*, Moon*>& p)
			{
				ApplyGravity(p, get, set);
			};

		ranges::for_each(AllUnorderedPairs(moons.size())
			| views::transform([&](const auto& p) -> pair<Moon*, Moon*>
				{
					return { &moons[p.first], &moons[p.second] };
				}),
			applyGravityAxis);

		auto applyVelocityAxis = [&](Moon* m)
			{
				ApplyVelocity(m, get, set);
			};

		ranges::for_each(moons | views::transform([](Moon& m) { return &m; }), applyVelocityAxis);
	}

	return { cycleStart, cycleLength };
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Moon> moons;
	ranges::copy(ScanfEachLine<int64_t, int64_t, int64_t>(input, "<x=%lld, y=%lld, z=%lld>")
		| views::transform([](const auto& l)
			{
				return Moon{ Vector3{ get<0>(l), get<1>(l), get<2>(l) } };
			}),
		back_inserter(moons));

	for (int64_t i = 0; i < 1000; i++)
	{
		ranges::for_each(AllUnorderedPairs(moons.size())
			| views::transform([&](const auto& p) -> pair<Moon*, Moon*>
				{
					return { &moons[p.first], &moons[p.second] };
				}),
			ApplyGravityXYZ);

		ranges::for_each(moons | views::transform([](Moon& m) { return &m; }), ApplyVelocityXYZ);
	}

	auto moonEnergy = moons | views::transform(TotalEnergy);
	int64_t answer = accumulate(moonEnergy.begin(), moonEnergy.end(), 0ll);

	printf("[2019] Puzzle12_A: %" PRId64 "\n", answer);
}


static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Moon> moons;
	ranges::copy(ScanfEachLine<int64_t, int64_t, int64_t>(input, "<x=%lld, y=%lld, z=%lld>")
		| views::transform([](const auto& l)
			{
				return Moon{ Vector3{ get<0>(l), get<1>(l), get<2>(l) } };
			}),
		back_inserter(moons));

	auto [cycleStartX, cycleLengthX] = FindCycle(moons, GetX, SetX);
	auto [cycleStartY, cycleLengthY] = FindCycle(moons, GetY, SetY);
	auto [cycleStartZ, cycleLengthZ] = FindCycle(moons, GetZ, SetZ);

	assert(cycleStartX == 0);
	assert(cycleStartY == 0);
	assert(cycleStartZ == 0);

	int64_t answer = lcm(lcm(cycleLengthX, cycleLengthY), cycleLengthZ);

	printf("[2019] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2019(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
