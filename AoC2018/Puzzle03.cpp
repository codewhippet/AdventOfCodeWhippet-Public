#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2018_Types
{
	struct Claim
	{
		int64_t Id;
		Point2 Origin;
		Point2 Size;
	};
}

using namespace Puzzle03_2018_Types;

static vector<Claim> ParseClaims(istream& input)
{
	vector<Claim> claims;
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t, int64_t, int64_t>(input, "#%lld @ %lld,%lld: %lldx%lld"))
	{
		Claim c;
		c.Id = get<0>(line);
		c.Origin = { get<1>(line), get<2>(line) };
		c.Size = { get<3>(line), get<4>(line) };
		claims.push_back(c);
	}
	return claims;
}

static bool DoesOverlap(const Claim& a, const Claim& b)
{
	bool disjoint =
		(a.Origin.X) > (b.Origin.X + b.Size.X - 1) ||
		(a.Origin.Y) > (b.Origin.Y + b.Size.Y - 1) ||
		(b.Origin.X) > (a.Origin.X + a.Size.X - 1) ||
		(b.Origin.Y) > (a.Origin.Y + a.Size.Y - 1);
	return !disjoint;
}

static void Puzzle03_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<Point2, int64_t> claimCounts;

	vector<Claim> claims = ParseClaims(input);
	for (const Claim& claim : claims)
	{
		for (const Point2& p : GridRange{ claim.Origin, claim.Origin + claim.Size })
		{
			claimCounts[p]++;
		}
	}

	int64_t answer = ranges::count_if(claimCounts, [](const auto& kvp) { return kvp.second > 1; });

	printf("[2018] Puzzle03_A: %" PRId64 "\n", answer);
}


static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Claim> claims = ParseClaims(input);

	set<size_t> overlaps;
	for (const auto& p : AllUnorderedPairs(claims.size()))
	{
		if (DoesOverlap(claims[p.first], claims[p.second]))
		{
			overlaps.insert(p.first);
			overlaps.insert(p.second);
		}
	}

	size_t nonOverlappingIndex = *(views::iota(0ull, claims.size()) | views::filter([&](const size_t i) { return !overlaps.contains(i); })).begin();
	int64_t answer = claims[nonOverlappingIndex].Id;

	printf("[2018] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2018(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
