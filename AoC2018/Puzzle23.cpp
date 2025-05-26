#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2018_Types
{
	struct Octahedron
	{
		int64_t PlanesBegin[4] = {};
		int64_t PlanesEnd[4] = {};
	};

	struct Nanobot
	{
		Vector3 Position;
		int64_t Radius = -1;

		Octahedron Bounds;
	};
}

using namespace Puzzle23_2018_Types;

static vector<Nanobot> ParseNanobots(istream& input)
{
	vector<Nanobot> nanobots;
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t>(input, "pos=<%lld,%lld,%lld>, r=%lld"))
	{
		nanobots.push_back({ { get<0>(line), get<1>(line), get<2>(line) }, get<3>(line) });
	}
	return nanobots;
}

static bool TryIntersect(const Octahedron& a, Octahedron* b)
{
	Octahedron c = a;

	for (int i = 0; i < 4; i++)
	{
		c.PlanesBegin[i] = max(c.PlanesBegin[i], b->PlanesBegin[i]);
		c.PlanesEnd[i] = min(c.PlanesEnd[i], b->PlanesEnd[i]);
		if (c.PlanesEnd[i] < c.PlanesBegin[i])
		{
			return false;
		}
	}

	*b = c;
	return true;
}

static void Puzzle23_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Nanobot> nanobots = ParseNanobots(input);
	ranges::sort(nanobots, [](const Nanobot& a, const Nanobot& b) { return a.Radius > b.Radius; });
	int64_t answer = ranges::count_if(nanobots, [&](const Nanobot& n) { return ManhattanDistance(nanobots[0].Position, n.Position) <= nanobots[0].Radius; });

	printf("[2018] Puzzle23_A: %" PRId64 "\n", answer);
}


static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Nanobot> nanobots = ParseNanobots(input);

	const Vector3 topNormals[] =
	{
		{  1,  1,  1 },
		{ -1,  1,  1 },
		{  1, -1,  1 },
		{ -1, -1,  1 },
	};

	for (Nanobot& n : nanobots)
	{
		Vector3 top = n.Position + (Vector3{ 0, 0, 1 } * n.Radius);
		Vector3 bottom = n.Position + (Vector3{ 0, 0, -1 } * n.Radius);

		for (int i = 0; i < 4; i++)
		{
			n.Bounds.PlanesBegin[i] = Dot(topNormals[i], bottom);
			n.Bounds.PlanesEnd[i] = Dot(topNormals[i], top);
			assert(n.Bounds.PlanesBegin[i] < n.Bounds.PlanesEnd[i]);
		}
	}

	enum class Side
	{
		Begin,
		End
	};

	set<tuple<int64_t, Side, size_t>> edges[4];
	for (size_t botIndex = 0; botIndex < nanobots.size(); botIndex++)
	{
		Nanobot& n = nanobots[botIndex];
		for (int i = 0; i < 4; i++)
		{
			edges[i].insert({ n.Bounds.PlanesBegin[i], Side::Begin, botIndex });
			edges[i].insert({ n.Bounds.PlanesEnd[i], Side::End, botIndex });
		}
	}

	vector<int64_t> setSizes[4];
	set<size_t> biggestSet[4];
	for (int i = 0; i < 4; i++)
	{
		set<size_t> activeRanges;
		for (const auto& kvp : edges[i])
		{
			size_t bot = get<2>(kvp);
			if (get<1>(kvp) == Side::Begin)
			{
				assert(activeRanges.contains(bot) == false);
				activeRanges.insert(bot);
			}
			else
			{
				assert(activeRanges.contains(bot));
				activeRanges.erase(bot);
			}
			setSizes[i].push_back(activeRanges.size());
			if (activeRanges.size() > biggestSet[i].size())
			{
				biggestSet[i] = activeRanges;
			}
		}
	}

	set<size_t> candidateSet = biggestSet[0];
	for (int i = 1; i < 4; i++)
	{
		set<size_t> intersection;
		ranges::set_intersection(candidateSet, biggestSet[i], inserter(intersection, intersection.end()));
		candidateSet.swap(intersection);
	}

	Octahedron candidateBounds =
	{
		{ numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min() },
		{ numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max() },
	};
	for (size_t botIndex : candidateSet)
	{
		bool intersected = TryIntersect(nanobots[botIndex].Bounds, &candidateBounds);
		assert(intersected);
		(void)intersected;
	}

	int64_t answer = 0;
	for (int i = 0; i < 4; i++)
	{
		if (candidateBounds.PlanesEnd[i] < 0)
		{
			answer = max(answer, -candidateBounds.PlanesEnd[i]);
		}
		else if (candidateBounds.PlanesBegin[i] > 0)
		{
			answer = max(answer, candidateBounds.PlanesBegin[i]);
		}
	}

	printf("[2018] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2018(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
