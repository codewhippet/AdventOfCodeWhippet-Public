#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2017_Types
{
	struct Hash
	{
		vector<uint8_t> Data = vector<uint8_t>(16);
	};
}

using namespace Puzzle14_2017_Types;

static Hash KnotHash(const string_view& s)
{
	vector<int64_t> lengths;
	for (char c : s)
	{
		lengths.push_back(c);
	}
	for (int i : { 17, 31, 73, 47, 23 })
	{
		lengths.push_back(i);
	}

	const size_t numElements = 256;

	vector<uint8_t> nodes(numElements);
	iota(nodes.begin(), nodes.end(), (uint8_t)0);

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

	Hash denseHash;
	for (size_t i = 0; i < nodes.size(); i++)
	{
		denseHash.Data[i / 16] ^= (uint8_t)nodes[i];
	}

	return denseHash;
}

static set<Point2> GetConnectedGroup(const set<Point2>& squares, Point2 startFrom)
{
	vector<Point2> searchQueue{ startFrom };
	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 current = searchQueue[i];
		if (visited.contains(current))
			continue;

		visited.insert(current);
		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = current + dir;
			if (squares.contains(neighbour))
			{
				searchQueue.push_back(neighbour);
			}
		}
	}
	return visited;
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secret = ReadSingleLine(input);

	int64_t answer = 0;
	for (int i = 0; i < 128; i++)
	{
		char buffer[64] = { 0 };
		sprintf(buffer, "%s-%d", secret.c_str(), i);
		Hash h = KnotHash(buffer);
		for (uint8_t byte : h.Data)
		{
			answer += popcount(byte);
		}
	}

	printf("[2017] Puzzle14_A: %" PRId64 "\n", answer);
}


static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secret = ReadSingleLine(input);

	set<Point2> squares;
	for (int y = 0; y < 128; y++)
	{
		char buffer[64] = { 0 };
		sprintf(buffer, "%s-%d", secret.c_str(), y);
		Hash h = KnotHash(buffer);
		for (int x = 0; x < 128; x++)
		{
			int byte = x / 8;
			int bitShift = x % 8;
			int bit = (h.Data[byte] >> (7 - bitShift)) & 0x1;
			if (bit)
			{
				squares.insert(Point2{ x, y });
			}
		}
	}

	int64_t answer = 0;
	while (squares.empty() == false)
	{
		set<Point2> group = GetConnectedGroup(squares, *squares.begin());
		for (Point2 square : group)
		{
			squares.erase(square);
		}
		answer++;
	}

	printf("[2017] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2017(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
