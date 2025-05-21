#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2022_Types
{
	struct Node
	{
		bool IsValue;
		int64_t Value = -1;
		vector<shared_ptr<Node>> List;
	};

	static regex StartsWithNumber{ R"((\d+).*)" };
}

using namespace Puzzle13_2022_Types;

static shared_ptr<Node> ParseNode(const string& input, size_t* pos)
{
	if (input[*pos] == '[')
	{
		shared_ptr<Node> newList{ new Node{ .IsValue = false } };
		(*pos)++;

		while (true)
		{
			if (input[*pos] == ']')
			{
				(*pos)++;
				return newList;
			}

			if (input[*pos] == ',')
				(*pos)++;

			newList->List.push_back(ParseNode(input, pos));
		}
	}
	else
	{
		shared_ptr<Node> newValue{ new Node{ .IsValue = true } };

		smatch m;
		string restOfString = input.substr(*pos);
		bool matched = regex_match(restOfString, m, StartsWithNumber);
		assert(matched);
		(void)matched;

		newValue->Value = atoll(m[1].str().c_str());

		*pos += m[1].str().size();

		return newValue;
	}
}

static int64_t Compare(const shared_ptr<Node>& left, const shared_ptr<Node>& right)
{
	if (left->IsValue && right->IsValue)
	{
		return left->Value - right->Value;
	}

	if (left->IsValue == false && right->IsValue == false)
	{
		for (size_t i = 0; i < max(left->List.size(), right->List.size()); i++)
		{
			// Left ran out of elements?
			if (i >= left->List.size())
				return -1;

			// Right ran out of elements?
			if (i >= right->List.size())
				return 1;

			// Diff the elements
			int64_t compareElement = Compare(left->List[i], right->List[i]);
			if (compareElement != 0)
				return compareElement;
		}

		return 0;
	}

	if (left->IsValue)
	{
		shared_ptr<Node> newLeft{ new Node{ .IsValue = false } };
		newLeft->List.push_back(left);
		return Compare(newLeft, right);
	}

	assert(right->IsValue);
	shared_ptr<Node> newRight{ new Node{ .IsValue = false } };
	newRight->List.push_back(right);
	return Compare(left, newRight);
}

static string NodeToString(const shared_ptr<Node>& n)
{
	if (n->IsValue)
	{
		return std::to_string(n->Value);
	}

	string output = "[";

	for (int64_t i = 0; i < (int64_t)n->List.size() - 1; i++)
	{
		output += NodeToString(n->List[i]);
		output += ',';
	}

	if (n->List.size() > 0)
	{
		output += NodeToString(n->List.back());
	}

	return output + "]";
}

static void Puzzle13_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> lines = ReadAllLines(input);
	for (size_t i = 0; i < lines.size(); i += 3)
	{
		const size_t pairIndex = (i / 3) + 1;

		size_t pos = 0;
		shared_ptr<Node> left = ParseNode(lines[i + 0], &pos);

        pos = 0;
        shared_ptr<Node> right = ParseNode(lines[i + 1], &pos);

		if (Compare(left, right) < 0)
		{
			answer += pairIndex;
		}
    }

	printf("[2022] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	size_t pos = 0;

	vector<shared_ptr<Node>> nodes;
	for (const string& line : ReadEachLine(input))
	{
		if (line.size() > 0)
		{
			pos = 0;
			nodes.push_back(ParseNode(line, &pos));
		}
	}

	pos = 0;
	nodes.push_back(ParseNode("[[2]]", &pos));

	pos = 0;
	nodes.push_back(ParseNode("[[6]]", &pos));

	ranges::sort(nodes, [](const shared_ptr<Node> &a, const shared_ptr<Node> &b) { return Compare(a, b) < 0; });

	vector<string> printedNodes = nodes | views::transform(NodeToString) | ranges::to<vector>();

	auto packetOne = ranges::find(printedNodes, "[[2]]");
	auto packetTwo = ranges::find(printedNodes, "[[6]]");

	int64_t answer = (distance(printedNodes.begin(), packetOne) + 1) * (distance(printedNodes.begin(), packetTwo) + 1);

	printf("[2022] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2022(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
