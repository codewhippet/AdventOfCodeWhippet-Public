#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2023_Types
{
	struct EdgeId
	{
		int64_t A;
		int64_t B;

		EdgeId(int64_t a, int64_t b)
			: A(min(a, b))
			, B(max(a, b))
		{
			assert(A != B);
		}

		auto operator<=>(const EdgeId&) const = default;
	};

	struct Graph
	{
		vector<string> Vertices;
		vector<vector<int64_t>> Edges;
	};
}

using namespace Puzzle25_2023_Types;

static Graph ParseGraph(istream& input)
{
	set<string> vertices;
	map<string, set<string>> edges;
	for (const string& line : ReadAllLines(input))
	{
		size_t colonPos = line.find(':');
		assert(colonPos != string::npos);

		string v = line.substr(0, colonPos);
		vertices.insert(v);

		vector<string> targetVeritices = StringSplit(line.substr(colonPos + 1), ' ');
		for (const string& to : targetVeritices)
		{
			vertices.insert(to);
			edges[v].insert(to);
			edges[to].insert(v);
		}
	}

	Graph g;

	map<string, int64_t> vertexIds;
	for (const string& v : vertices)
	{
		vertexIds.insert({ v, (int64_t)g.Vertices.size() });
		g.Vertices.push_back(v);
	}

	g.Edges.resize(g.Vertices.size());
	for (const auto& connections : edges)
	{
		const string& from = connections.first;
		for (const string& to : connections.second)
		{
			g.Edges[vertexIds.at(from)].push_back(vertexIds.at(to));
			g.Edges[vertexIds.at(to)].push_back(vertexIds.at(from));
		}
	}

	ranges::for_each(g.Edges, [](auto& edges)
		{
			ranges::sort(edges);
			auto makeUnique = ranges::unique(edges);
			edges.erase(makeUnique.begin(), makeUnique.end());
		});

	return g;
}

static void FischerYatesShuffle(vector<int64_t>* v)
{
	for (size_t i = 0; i < (v->size() - 2); i++)
	{
		swap((*v)[i], (*v)[(rand() % (v->size() - i)) + i]);
	}
}

static vector<int64_t> FindShortestRoute(Graph* graph, int64_t from, int64_t to)
{
	vector<pair<int64_t, int64_t>> searchQueue{ { from, -1 } };
	set<int64_t> visited;

	for (int64_t queueIndex = 0; queueIndex < (int64_t)searchQueue.size(); queueIndex++)
	{
		auto [currentVertex, cameFrom] = searchQueue[queueIndex];
		if (currentVertex == to)
		{
			vector<int64_t> route;
			while (queueIndex != -1)
			{
				route.push_back(searchQueue[queueIndex].first);
				queueIndex = searchQueue[queueIndex].second;
			}
			return route;
		}

		if (visited.insert(currentVertex).second == false)
			continue;

		FischerYatesShuffle(&graph->Edges.at(currentVertex));
		for (int64_t neighbour : graph->Edges.at(currentVertex))
		{
			searchQueue.push_back({ neighbour, queueIndex });
		}
	}

	return {};
}

static void FindConnected(const int64_t& v, const Graph& g, set<int64_t>* subgraph)
{
	subgraph->insert(v);
	for (int64_t connection : g.Edges.at(v))
	{
		if (subgraph->contains(connection) == false)
		{
			FindConnected(connection, g, subgraph);
		}
	}
}

static vector<size_t> CountSubgraphSizes(const Graph& g)
{
	vector<size_t> subgraphSizes;

	set<int64_t> allVisited;
	for (int64_t v : ranges::iota_view(0, (int64_t)g.Vertices.size()))
	{
		if (allVisited.contains(v) == false)
		{
			set<int64_t> subgraph;
			FindConnected(v, g, &subgraph);
			subgraphSizes.push_back(subgraph.size());
			allVisited.insert(subgraph.begin(), subgraph.end());
		}
	}

	return subgraphSizes;
}

static void Puzzle25_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int numberOfTrials = 1000;

	Graph g = ParseGraph(input);

	map<EdgeId, double> voting;
	for (int trial = 0; trial < numberOfTrials; trial++)
	{
		int64_t from = rand() % g.Vertices.size();
		int64_t to = rand() % g.Vertices.size();
		if (from == to)
			continue;

		const vector<int64_t> selectedRoute = FindShortestRoute(&g, from, to);

		double routeContribution = 1.0 / selectedRoute.size();
		for (size_t routeIndex = 0; (routeIndex + 1) < selectedRoute.size(); routeIndex++)
		{
			voting[EdgeId{ selectedRoute[routeIndex], selectedRoute[routeIndex + 1] }] += routeContribution;
		}
	}

	vector<pair<double, EdgeId>> votes;
	ranges::copy(voting | views::transform([](const auto& p) { return pair<double, EdgeId>{ p.second, p.first }; }),  back_inserter(votes));
	ranges::sort(votes);
	ranges::reverse(votes);

	vector<EdgeId> cutPairs;
	ranges::copy(votes | views::take(3) | views::values, back_inserter(cutPairs));

	for (const EdgeId& c : cutPairs)
	{
		erase(g.Edges[c.A], c.B);
		erase(g.Edges[c.B], c.A);
	}

	vector<size_t> sizes = CountSubgraphSizes(g);
	assert(sizes.size() == 2);

	int64_t answer = accumulate(sizes.begin(), sizes.end(), 1ll, multiplies{});

	printf("[2023] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2023(const string& filename)
{
	Puzzle25_A(filename);
}
