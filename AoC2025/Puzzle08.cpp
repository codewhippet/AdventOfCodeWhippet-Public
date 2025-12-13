#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2025_Types
{
	struct Vertex
	{
		int64_t GroupSize = 1;
		Vertex* Parent = nullptr;
		Vector3 Position;

		Vertex* GetRoot();

		auto operator<=>(const Vertex&) const = default;
	};

	struct Edge
	{
		int64_t Distance = -1;
		Vertex* A = nullptr;
		Vertex* B = nullptr;

		auto operator<=>(const Edge&) const = default;
	};
}

using namespace Puzzle08_2025_Types;

Vertex* Puzzle08_2025_Types::Vertex::GetRoot()
{
	Vertex* root = this;
	while (root->Parent)
	{
		root = root->Parent;
		Parent = root;
	}

	return root;
}

static int64_t Join(const Edge& e)
{
	Vertex* aRoot = e.A->GetRoot();
	Vertex* bRoot = e.B->GetRoot();
	if (aRoot == bRoot)
		return aRoot->GroupSize;

	aRoot->GroupSize += bRoot->GroupSize;
	bRoot->GroupSize = 0;
	bRoot->Parent = aRoot;

	return aRoot->GroupSize;
}

static map<int64_t, size_t> GetShortestEdges(int64_t numEdges, const vector<Edge>& edges)
{
	map<int64_t, size_t> shortEdges;

	int64_t longestShortEdge = numeric_limits<int64_t>::max();
	for (size_t i = 0; i < edges.size(); i++)
	{
		const Edge& e = edges[i];
		if (e.Distance > longestShortEdge)
			continue;

		shortEdges.insert({ e.Distance, i });
		if ((int64_t)shortEdges.size() > numEdges)
		{
			shortEdges.erase(prev(shortEdges.end()));
			longestShortEdge = prev(shortEdges.end())->first;
		}
	}

	return shortEdges;
}

static set<int64_t> LargestGroups(int64_t numGroups, const vector<Vertex>& vertices)
{
	set<int64_t> groups;

	int64_t smallestLargeGroup = 0;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		const Vertex& v = vertices[i];
		if (v.GroupSize < smallestLargeGroup)
			continue;

		groups.insert(v.GroupSize);
		if ((int64_t)groups.size() > numGroups)
		{
			groups.erase(groups.begin());
			smallestLargeGroup = *groups.begin();
		}
	}

	return groups;
}

static int64_t KthLongestEdge(int64_t numEdges, const vector<Edge>& edges)
{
	set<int64_t> shortEdges;

	int64_t longestShortEdge = numeric_limits<int64_t>::max();
	for (size_t i = 0; i < edges.size(); i++)
	{
		const Edge& e = edges[i];
		if (e.Distance > longestShortEdge)
			continue;

		shortEdges.insert(e.Distance);
		if ((int64_t)shortEdges.size() > numEdges)
		{
			shortEdges.erase(prev(shortEdges.end()));
			longestShortEdge = *prev(shortEdges.end());
		}
	}

	return *prev(shortEdges.end());
}

static void Puzzle08_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Vertex> vertices;
	vertices.reserve(1024);
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t>(input, "%lld,%lld,%lld"))
	{
		vertices.push_back({ 1, nullptr, { get<0>(line), get<1>(line), get<2>(line) } });
	}

	vector<Edge> edges;
	edges.reserve((vertices.size() * (vertices.size() + 1)) / 2);
	for (const auto& p : AllUnorderedPairs(vertices.size()))
	{
		const Vector3 diff = vertices[p.second].Position - vertices[p.first].Position;
		const int64_t distance = (diff.X * diff.X) + (diff.Y * diff.Y) + (diff.Z * diff.Z);
		edges.push_back({ distance, &vertices[p.first], &vertices[p.second] });
	}

	map<int64_t, size_t> shortestEdges = GetShortestEdges(1000, edges);
	for (const auto& e : shortestEdges)
	{
		Join(edges[e.second]);
	}

	set<int64_t> largestGroups = LargestGroups(3, vertices);
	int64_t answer = ranges::fold_left(largestGroups, 1, multiplies{});

	printf("[2025] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Vertex> vertices;
	vertices.reserve(1024);
	for (const auto& line : ScanfEachLine<int64_t, int64_t, int64_t>(input, "%lld,%lld,%lld"))
	{
		vertices.push_back({ 1, nullptr, { get<0>(line), get<1>(line), get<2>(line) } });
	}

	int64_t maximumDistance = 0;

	vector<Edge> edges;
	edges.reserve((vertices.size() * (vertices.size() + 1)) / 2);
	for (const auto& p : AllUnorderedPairs(vertices.size()))
	{
		const Vector3 diff = vertices[p.second].Position - vertices[p.first].Position;
		const int64_t distance = (diff.X * diff.X) + (diff.Y * diff.Y) + (diff.Z * diff.Z);
		edges.push_back({ distance, &vertices[p.first], &vertices[p.second] });
		maximumDistance = max(distance, maximumDistance);
	}

	// Guess at the maximum length we'll want
	const int64_t searchBucketSize = KthLongestEdge(vertices.size(), edges);

	int64_t answer = -1;

	int64_t upperSearchDistance = 0;
	vector<Edge>::iterator chunkBegin = edges.begin();
	vector<Edge>::iterator chunkEnd = chunkBegin;
	while (answer == -1)
	{
		chunkBegin = chunkEnd;
		upperSearchDistance += searchBucketSize;
		chunkEnd = partition(chunkBegin, edges.end(), [&](const Edge& e) { return e.Distance <= upperSearchDistance; });
		if (chunkEnd == chunkBegin)
			continue;

		sort(chunkBegin, chunkEnd);
		for (vector<Edge>::iterator e = chunkBegin; e < chunkEnd; e++)
		{
			const Edge& edgeToJoin = *e;
			const int64_t groupSize = Join(edgeToJoin);
			if (groupSize == (int64_t)vertices.size())
			{
				answer = edgeToJoin.A->Position.X * edgeToJoin.B->Position.X;
				break;
			}
		}
	}

	printf("[2025] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2025(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
