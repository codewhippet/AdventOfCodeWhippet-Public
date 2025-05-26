#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2017_Types
{
}

using namespace Puzzle10_2017_Types;

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> lengths;

	string line = ReadSingleLine(input);
	char* token = strtok(line.data(), ", ");
	while (token)
	{
		lengths.push_back(atoll(token));
		token = strtok(nullptr, ", ");
	}

	const size_t numElements = 256;

	vector<int64_t> nodes(numElements);
	iota(nodes.begin(), nodes.end(), 0);

	int64_t currentPosition = 0;
	int64_t skipSize = 0;
	for (int64_t length : lengths)
	{
		assert(length <= (int64_t)nodes.size());

		int64_t reverseBegin = currentPosition;
		int64_t reverseEnd = currentPosition + length - 1;

		while (reverseEnd > reverseBegin)
		{
			swap(nodes[reverseBegin % nodes.size()], nodes[reverseEnd % nodes.size()]);

			reverseBegin++;
			reverseEnd--;
		}

		currentPosition = (currentPosition + length + skipSize) % nodes.size();
		skipSize++;
	}

	int64_t answer = nodes[0] * nodes[1];

	printf("[2017] Puzzle10_A: %" PRId64 "\n", answer);
}


static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> lengths;

	string line = ReadSingleLine(input);
	for (char c : line)
	{
		lengths.push_back(c);
	}
	for (int i : { 17, 31, 73, 47, 23 })
	{
		lengths.push_back(i);
	}

	const size_t numElements = 256;

	vector<int64_t> nodes(numElements);
	iota(nodes.begin(), nodes.end(), 0);

	int64_t currentPosition = 0;
	int64_t skipSize = 0;
	for (int64_t round = 0; round < 64; round++)
	{
		for (int64_t length : lengths)
		{
			assert(length <= (int64_t)nodes.size());

			int64_t reverseBegin = currentPosition;
			int64_t reverseEnd = currentPosition + length - 1;

			while (reverseEnd > reverseBegin)
			{
				swap(nodes[reverseBegin % nodes.size()], nodes[reverseEnd % nodes.size()]);

				reverseBegin++;
				reverseEnd--;
			}

			currentPosition = (currentPosition + length + skipSize) % nodes.size();
			skipSize++;
		}
	}

	vector<int64_t> denseHash(16);
	for (size_t i = 0; i < nodes.size(); i++)
	{
		denseHash[i / 16] ^= nodes[i];
	}

	char buffer[128] = { 0 };
	sprintf(buffer, "%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx%02llx",
		denseHash[0], denseHash[1], denseHash[2], denseHash[3],
		denseHash[4], denseHash[5], denseHash[6], denseHash[7],
		denseHash[8], denseHash[9], denseHash[10], denseHash[11],
		denseHash[12], denseHash[13], denseHash[14], denseHash[15]);

	string answer = buffer;

	printf("[2017] Puzzle10_B: %s\n", answer.c_str());
}

void Puzzle10_2017(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
