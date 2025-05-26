#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2019_Types
{
}

using namespace Puzzle24_2019_Types;

static ArrayMap2D Step(const ArrayMap2D& oldEris)
{
	ArrayMap2D newEris(oldEris);
	for (const auto& p : oldEris.Grid())
	{
		int64_t bugCount = ranges::count(Point2::CardinalDirections() | views::transform([&](const Point2& dir) { return oldEris(p.first + dir); }), '#');

		// A bug dies (becoming an empty space) unless there is exactly one bug adjacent to it.
		if ((p.second == '#') && (bugCount != 1))
			newEris(p.first) = '.';

		// An empty space becomes infested with a bug if exactly one or two bugs are adjacent to it.
		if ((p.second == '.') && ((bugCount == 1) || (bugCount == 2)))
			newEris(p.first) = '#';
	}
	return newEris;
}

static uint32_t CalculateBiodiversity(const ArrayMap2D& eris)
{
	uint32_t biodiversity = 0;
	for (size_t i = 0; i < eris.GetData().size(); i++)
	{
		if (eris.GetData()[i] == '#')
		{
			biodiversity |= 1 << i;
		}
	}
	return biodiversity;
}

static int64_t FindFirstRepeat(ArrayMap2D eris)
{
	set<uint32_t> seen;
	while (true)
	{
		uint32_t biodiversity = CalculateBiodiversity(eris);
		if (seen.insert(biodiversity).second == false)
			return biodiversity;

		eris = Step(eris);
	}
}

static set<Vector3> MakeEris(const ArrayMap2D& erisSlice)
{
	set<Vector3> eris;
	ranges::copy(erisSlice.Grid()
		| views::filter([&](const auto& p)
			{
				return p.second == '#';
			})
		| views::transform([](const auto& p)
			{
				return Vector3{ p.first.X, p.first.Y, 0 };
			}),
		inserter(eris, eris.end()));
	return eris;
}

static size_t GridIndex(const Vector3& v)
{
	// Map to the same grid index used in the puzzle text for convenience
	return (v.Y * 5) + v.X + 1;
}

static const vector<vector<Vector3>> BuildNeighbours()
{
	vector<vector<Vector3>> neighbours{ 26 };

	constexpr Vector3 G01{ 0, 0, 0 };
	constexpr Vector3 G02{ 1, 0, 0 };
	constexpr Vector3 G03{ 2, 0, 0 };
	constexpr Vector3 G04{ 3, 0, 0 };
	constexpr Vector3 G05{ 4, 0, 0 };

	constexpr Vector3 G06{ 0, 1, 0 };
	constexpr Vector3 G07{ 1, 1, 0 };
	constexpr Vector3 G08{ 2, 1, 0 };
	constexpr Vector3 G09{ 3, 1, 0 };
	constexpr Vector3 G10{ 4, 1, 0 };

	constexpr Vector3 G11{ 0, 2, 0 };
	constexpr Vector3 G12{ 1, 2, 0 };
	// G13
	constexpr Vector3 G14{ 3, 2, 0 };
	constexpr Vector3 G15{ 4, 2, 0 };

	constexpr Vector3 G16{ 0, 3, 0 };
	constexpr Vector3 G17{ 1, 3, 0 };
	constexpr Vector3 G18{ 2, 3, 0 };
	constexpr Vector3 G19{ 3, 3, 0 };
	constexpr Vector3 G20{ 4, 3, 0 };

	constexpr Vector3 G21{ 0, 4, 0 };
	constexpr Vector3 G22{ 1, 4, 0 };
	constexpr Vector3 G23{ 2, 4, 0 };
	constexpr Vector3 G24{ 3, 4, 0 };
	constexpr Vector3 G25{ 4, 4, 0 };

	constexpr Vector3 Inner{ 0, 0, 1 };
	constexpr Vector3 Outer{ 0, 0, -1 };

	vector<pair<Vector3, vector<Vector3>>> samePlaneAdjacencies =
	{
		{ G01, { G02, G06 } },
		{ G02, { G03, G07 } },
		{ G03, { G04, G08 } },
		{ G04, { G05, G09 } },
		{ G05, { G10 } },

		{ G06, { G07, G11 } },
		{ G07, { G08, G12 } },
		{ G08, { G09 } },
		{ G09, { G10, G14 } },
		{ G10, { G15 } },

		{ G11, { G12, G16 } },
		{ G12, { G17 } },
		// G13
		{ G14, { G15, G19 } },
		{ G15, { G20 } },

		{ G16, { G17, G21 } },
		{ G17, { G18, G22 } },
		{ G18, { G19, G23 } },
		{ G19, { G20, G24 } },
		{ G20, { G25 } },

		{ G21, { G22 } },
		{ G22, { G23 } },
		{ G23, { G24 } },
		{ G24, { G25 } },
	};

	for (const auto& samePlane : samePlaneAdjacencies)
	{
		for (const auto& link : samePlane.second)
		{
			neighbours[GridIndex(samePlane.first)].push_back(link);
			neighbours[GridIndex(link)].push_back(samePlane.first);
		}
	}

	vector<pair<Vector3, vector<Vector3>>> innerOuterAdjacencies =
	{
		{ G08, { G01, G02, G03, G04, G05 } },
		{ G12, { G01, G06, G11, G16, G21 } },
		{ G14, { G05, G10, G15, G20, G25 } },
		{ G18, { G21, G22, G23, G24, G25 } },
	};

	for (const auto& differentPlane : innerOuterAdjacencies)
	{
		for (const auto& link : differentPlane.second)
		{
			neighbours[GridIndex(differentPlane.first)].push_back(link + Inner);
			neighbours[GridIndex(link)].push_back(differentPlane.first + Outer);
		}
	}

	return neighbours;
}

static vector<Vector3> Neighbours(const Vector3& v)
{
	static const vector<vector<Vector3>> baseNeighbours = BuildNeighbours();
	vector<Vector3> neighbours = baseNeighbours[GridIndex(v)];
	ranges::for_each(neighbours, [&](Vector3& n) { n.Z += v.Z; });
	return neighbours;
}

static set<Vector3> Step(const set<Vector3>& oldEris)
{
	map<Vector3, int64_t> bugsAdjacent;
	auto bugAdjacentPoints = views::join(oldEris | views::transform(Neighbours));
	ranges::for_each(bugAdjacentPoints, [&](const Vector3& v) { bugsAdjacent[v]++; });

	set<Vector3> newEris;

	// A bug dies (becoming an empty space) unless there is exactly one bug adjacent to it
	ranges::copy(oldEris | views::filter([&](const Vector3& bug) { return bugsAdjacent[bug] == 1; }), inserter(newEris, newEris.end()));

	// An empty space becomes infested with a bug if exactly one or two bugs are adjacent to it.
	ranges::copy(bugsAdjacent
		| views::filter([&](const auto& p)
			{
				return !oldEris.contains(p.first) && ((p.second == 1) || (p.second == 2));
			})
		| views::keys,
		inserter(newEris, newEris.end()));

	return newEris;
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D eris = ReadArrayMap(input);
	int64_t answer = FindFirstRepeat(eris);

	printf("[2019] Puzzle24_A: %" PRId64 "\n", answer);
}


static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<Vector3> eris = MakeEris(ReadArrayMap(input));

	const int simulationSteps = 200;

	for (int i = 0; i < simulationSteps; i++)
	{
		eris = Step(eris);
	}

	int64_t answer = (int64_t)eris.size();

	printf("[2019] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2019(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
