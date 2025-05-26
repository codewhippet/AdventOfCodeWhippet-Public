#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2018_Types
{
	struct Node
	{
		vector<shared_ptr<Node>> Children;
		vector<int64_t> Metadata;
	};
}

using namespace Puzzle08_2018_Types;

static pair<size_t, int64_t> CountMetadata(const vector<int64_t>& chunks, size_t offset)
{
	int64_t numChildren = chunks[offset++];
	int64_t numMetadataChunks = chunks[offset++];

	int64_t metadataSum = 0;
	for (int64_t i = 0; i < numChildren; i++)
	{
		auto [newOffset, childSum] = CountMetadata(chunks, offset);
		metadataSum += childSum;
		offset = newOffset;
	}

	for (int64_t i = 0; i < numMetadataChunks; i++)
	{
		metadataSum += chunks[offset++];
	}

	return { offset, metadataSum };
}

static shared_ptr<Node> ParseNodes(const vector<int64_t>& chunks, size_t *offset)
{
	shared_ptr<Node> node = make_shared<Node>();

	int64_t numChildren = chunks[(*offset)++];
	int64_t numMetadataChunks = chunks[(*offset)++];

	for (int64_t i = 0; i < numChildren; i++)
	{
		node->Children.push_back(ParseNodes(chunks, offset));
	}

	for (int64_t i = 0; i < numMetadataChunks; i++)
	{
		node->Metadata.push_back(chunks[(*offset)++]);
	}

	return node;
}

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> chunks = ReadAsVectorOfNumbers(ReadSingleLine(input));
	auto [endingOffset, numMetadataChunks] = CountMetadata(chunks, 0);
	assert(endingOffset == chunks.size());

	int64_t answer = numMetadataChunks;

	printf("[2018] Puzzle08_A: %" PRId64 "\n", answer);
}


static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> chunks = ReadAsVectorOfNumbers(ReadSingleLine(input));

	size_t offset = 0;
	shared_ptr<Node> rootNode = ParseNodes(chunks, &offset);

	function<int64_t(shared_ptr<Node>)> scoreNode =
		[&](shared_ptr<Node> node)
		{
			if (node->Children.empty())
			{
				return accumulate(node->Metadata.begin(), node->Metadata.end(), 0ll);
			}

			int64_t score = 0;
			for (int64_t m : node->Metadata)
			{
				if ((m > 0) && (m <= (int64_t)node->Children.size()))
				{
					score += scoreNode(node->Children[m - 1]);
				}
			}
			return score;
		};

	int64_t answer = scoreNode(rootNode);

	printf("[2018] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2018(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
