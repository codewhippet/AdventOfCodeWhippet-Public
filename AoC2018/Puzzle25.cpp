#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2018_Types
{
	struct Constellation
	{
		Constellation* Parent = nullptr;
		Vector4 Star;

		Constellation* GetRepresentative()
		{
			Constellation* representative = this;
			while (representative->Parent)
				representative = representative->Parent;
			return representative;
		}
	};
}

using namespace Puzzle25_2018_Types;

static bool TryMerge(Constellation* a, Constellation* b)
{
	a = a->GetRepresentative();
	b = b->GetRepresentative();
	if (a == b)
	{
		return false;
	}

	b->Parent = a;
	return true;
}

static int64_t ManhattanDistance(const Vector4& a, const Vector4& b)
{
	return abs(b.X - a.X) + abs(b.Y - a.Y) + abs(b.Z - a.Z) + abs(b.W - a.W);
}

static void Puzzle25_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Vector4> points;
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t>(input, "%lld,%lld,%lld,%lld"))
	{
		points.push_back({ get<0>(line), get<1>(line), get<2>(line), get<3>(line) });
	}

	vector<Constellation> stars;
	stars.resize(points.size());
	for (size_t i = 0; i < points.size(); i++)
	{
		stars[i].Star = points[i];
	}

	const int64_t maximumDistance = 3;
	bool merged = false;
	do
	{
		merged = false;
		for (const auto& p : AllUnorderedPairs(points.size()))
		{
			if (ManhattanDistance(stars[p.first].Star, stars[p.second].Star) <= maximumDistance)
			{
				merged = TryMerge(&stars[p.first], &stars[p.second]) || merged;
			}
		}

	} while (merged);

	set<Constellation*> constellations;
	ranges::copy(stars | views::transform([](Constellation& c) { return c.GetRepresentative(); }), inserter(constellations, constellations.end()));

	int64_t answer = constellations.size();

	printf("[2018] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2018(const string& filename)
{
	Puzzle25_A(filename);
}
