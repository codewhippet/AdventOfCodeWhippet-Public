#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2024_Types
{
	struct OrderedPair
	{
		string A;
		string B;

		OrderedPair(const string& a, const string& b)
		{
			A = min(a, b);
			B = max(a, b);
		}

		OrderedPair(const pair<string, string>& p)
		{
			A = min(p.first, p.second);
			B = max(p.first, p.second);
		}
	};
}

using namespace Puzzle23_2024_Types;

static void Puzzle23_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<string> tNodes;
	multimap<string, string> connections;

	for (const string& line : ReadAllLines(input))
	{
		char aNode[8] = { 0 };
		char bNode[8] = { 0 };
		int parsed = sscanf(line.c_str(), "%7[^-]-%7[^-]", &aNode, &bNode);
		assert(parsed == 2);
		(void)parsed;

		if (aNode[0] == 't')
			tNodes.insert(aNode);
		if (bNode[0] == 't')
			tNodes.insert(bNode);

		connections.insert({ aNode, bNode });
		connections.insert({ bNode, aNode });
	}

	set<vector<string>> triples;

	for (const string& tNode : tNodes)
	{
		auto connectedRangeForT = connections.equal_range(tNode);
		set<string> connectedNodes;
		ranges::copy(ranges::subrange(connectedRangeForT.first, connectedRangeForT.second)
			| views::transform([](const auto& connection)
				{
					return connection.second;
				}),
			inserter(connectedNodes, connectedNodes.end()));

		for (const string& nodeA : connectedNodes)
		{
			auto connectedRangeForA = connections.equal_range(nodeA);
			for (const auto& nodeAConnection : ranges::subrange(connectedRangeForA.first, connectedRangeForA.second))
			{
				const string& nodeB = nodeAConnection.second;
				if (connectedNodes.contains(nodeB))
				{
					vector<string> triple{ tNode, nodeA, nodeB };
					ranges::sort(triple);
					triples.insert(triple);
				}
			}
		}
	}

	int64_t answer = triples.size();

	printf("[2024] Puzzle23_A: %" PRId64 "\n", answer);
}

static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, size_t> nodeNames;
	map<size_t, string> unmapNames;

	set<size_t> nodes;
	set<pair<size_t, size_t>> connections;

	for (const string& line : ReadAllLines(input))
	{
		char aNode[8] = { 0 };
		char bNode[8] = { 0 };
		int parsed = sscanf(line.c_str(), "%7[^-]-%7[^-]", &aNode, &bNode);
		assert(parsed == 2);
		(void)parsed;

		size_t nodeA = nodeNames.contains(aNode) ? nodeNames.at(aNode) : nodeNames.size();
		nodeNames[aNode] = nodeA;
		unmapNames[nodeA] = aNode;

		size_t nodeB = nodeNames.contains(bNode) ? nodeNames.at(bNode) : nodeNames.size();
		nodeNames[bNode] = nodeB;
		unmapNames[nodeB] = bNode;

		nodes.insert(nodeA);
		nodes.insert(nodeB);

		connections.insert({ nodeA, nodeB });
		connections.insert({ nodeB, nodeA });
	}

	size_t largestClique = 0;
	set<size_t> cliqueAnswer;
	function<void(size_t, set<size_t>*)> FindLargestClique = [&](size_t node, set<size_t>* clique) -> void
		{
			if (node == nodes.size())
			{
				if (clique->size() > largestClique)
				{
					largestClique = clique->size();
					cliqueAnswer = *clique;
				}
				return;
			}

			// Early out if we don't have enough nodes for a larger clique

			// Check to see if we're fully connected to the existing clique
			bool isNodeFullyConnected = true;
			for (size_t otherNode : *clique)
			{
				if (connections.contains({ node, otherNode }) == false)
				{
					isNodeFullyConnected = false;
					break;
				}
			}

			if (isNodeFullyConnected)
			{
				clique->insert(node);
				FindLargestClique(node + 1, clique);
				clique->erase(node);
			}

			FindLargestClique(node + 1, clique);
		};

	set<size_t> clique;
	FindLargestClique(0, &clique);

	set<string> cliqueNodes;
	for (size_t n : cliqueAnswer)
	{
		cliqueNodes.insert(unmapNames.at(n));
	}

	string answer;
	for (const string& n : cliqueNodes)
	{
		answer += n + ",";
	}

	printf("[2024] Puzzle23_B: %s\n", answer.substr(0, answer.size() - 1).c_str());
}

void Puzzle23_2024(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
