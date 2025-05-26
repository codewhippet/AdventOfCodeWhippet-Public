#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2017_Types
{
	struct Node
	{
		string Id;
		int64_t OurWeight = 0;
		int64_t SubtreeWeight = 0;
		vector<shared_ptr<Node>> Children;
	};

}

using namespace Puzzle07_2017_Types;

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<string> parents;
	set<string> children;

	for (const auto& m : ReadEachLine(input, regex{ R"((\w+) \(\d+\) -> (.+))" }))
	{
		if (m.empty())
			continue;

		parents.insert(m[1].str());

		string listOfChildren = m[2].str();
		char* token = strtok(listOfChildren.data(), ", ");
		while (token)
		{
			children.insert(token);
			token = strtok(nullptr, ", ");
		}
	}

	string answer = (parents | views::filter([&](const string& s) { return !children.contains(s); })).front();

	printf("[2017] Puzzle07_A: %s\n", answer.c_str());
}


static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const regex leafNodePattern{ R"((\w+) \((\d+)\))" };
	const regex parentPattern{ R"((\w+) \((\d+)\) -> (.+))" };

	map<string, shared_ptr<Node>> nodes;
	multimap<string, string> parentToChildren;
	set<string> children;

	for (const string& line : ReadEachLine(input))
	{
		shared_ptr<Node> newNode{ new Node() };

		smatch m;
		if (regex_match(line, m, parentPattern))
		{
			newNode->Id = m[1].str();
			newNode->OurWeight = atoll(m[2].str().c_str());

			string listOfChildren = m[3].str();
			char* token = strtok(listOfChildren.data(), ", ");
			while (token)
			{
				parentToChildren.insert({ newNode->Id, token });
				children.insert(token);
				token = strtok(nullptr, ", ");
			}
		}
		else if (regex_match(line, m, leafNodePattern))
		{
			newNode->Id = m[1].str();
			newNode->OurWeight = atoll(m[2].str().c_str());
		}
		else
		{
			assert(false);
		}

		nodes[newNode->Id] = newNode;
	}

	string rootNodeName = (nodes | views::filter([&](const auto& kvp) { return !children.contains(kvp.first); })).front().first;

	function<shared_ptr<Node>(const shared_ptr<Node>&)> buildTree =
		[&](const shared_ptr<Node>& node) -> shared_ptr<Node>
		{
			auto nodeChildren = parentToChildren.equal_range(node->Id);
			for (auto childIt = nodeChildren.first; childIt != nodeChildren.second; ++childIt)
			{
				shared_ptr<Node> child = nodes.at(childIt->second);
				node->Children.push_back(child);
				buildTree(child);
				node->SubtreeWeight += child->SubtreeWeight;
			}
			node->SubtreeWeight += node->OurWeight;
			return node;
		};
	shared_ptr<Node> rootNode = buildTree(nodes.at(rootNodeName));

	function<int64_t(const shared_ptr<Node>&)> expectedWeightOfChildren =
		[&](const shared_ptr<Node>& node) -> int64_t
		{
			map<int64_t, int64_t> weights;
			for (const shared_ptr<Node> child : node->Children)
			{
				weights[child->SubtreeWeight]++;
			}

			assert(weights.empty() == false);

			if (weights.size() == 1)
			{
				return weights.begin()->first;
			}

			return (weights | views::filter([](const auto& kvp) { return kvp.second != 1; })).front().first;
		};

	function<int64_t(const shared_ptr<Node>&, int64_t)> findCorrectedWeight =
		[&](const shared_ptr<Node>& node, int64_t expectedWeight) -> int64_t
		{
			// Recurse to any unbalanced children
			int64_t expectedChildWeight = expectedWeightOfChildren(node);
			for (const shared_ptr<Node> child : node->Children)
			{
				if (child->SubtreeWeight != expectedChildWeight)
				{
					return findCorrectedWeight(child, expectedChildWeight);
				}
			}

			// It's us that's unbalanced!
			int64_t diffNeeded = expectedWeight - node->SubtreeWeight;
			return node->OurWeight + diffNeeded;
		};

	int64_t answer = findCorrectedWeight(rootNode, rootNode->SubtreeWeight);

	printf("[2017] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2017(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
