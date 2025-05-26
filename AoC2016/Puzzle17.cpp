#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2016_Types
{
}

using namespace Puzzle17_2016_Types;

static vector<char> OpenDoors(const string &route, const MD5& md5)
{
	uint8_t hash[16];
	md5.Hash(route, hash);

	vector<char> doors;
	if (((hash[0] >> 4) & 0xf) > 0xa)
	{
		doors.push_back('U');
	}
	if (((hash[0] >> 0) & 0xf) > 0xa)
	{
		doors.push_back('D');
	}
	if (((hash[1] >> 4) & 0xf) > 0xa)
	{
		doors.push_back('L');
	}
	if (((hash[1] >> 0) & 0xf) > 0xa)
	{
		doors.push_back('R');
	}
	return doors;
}

static string ShortestPathTo(const ArrayMap2D& rooms, const Point2& start, const Point2& end, const string& passcode)
{
	MD5 md5;

	const map<char, Point2> directions = { { 'U', Point2::Up() }, { 'D', Point2::Down() }, { 'L', Point2::Left() }, { 'R', Point2::Right() } };

	vector<pair<Point2, string>> searchQueue{ { start, passcode } };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		if (searchQueue[i].first == end)
		{
			return searchQueue[i].second.substr(passcode.size());
		}

		vector<char> openDoors = OpenDoors(searchQueue[i].second, md5);
		for (char c : openDoors)
		{
			Point2 nextPos = searchQueue[i].first + directions.at(c);
			if (rooms.IsInside(nextPos))
			{
				searchQueue.push_back({ nextPos, searchQueue[i].second + c });
			}
		}
	}

	return "";
}

static size_t LongestPathTo(const ArrayMap2D& rooms, const Point2& start, const Point2& end, const string& passcode)
{
	MD5 md5;

	const map<char, Point2> directions = { { 'U', Point2::Up() }, { 'D', Point2::Down() }, { 'L', Point2::Left() }, { 'R', Point2::Right() } };

	size_t longestPath = 0;

	list<pair<Point2, string>> searchQueue{ { start, passcode } };
	while (searchQueue.empty() == false)
	{
		pair<Point2, string> current = move(searchQueue.front());
		searchQueue.pop_front();

		if (current.first == end)
		{
			longestPath = max(longestPath, current.second.size() - passcode.size());
			continue;
		}

		vector<char> openDoors = OpenDoors(current.second, md5);
		for (char c : openDoors)
		{
			Point2 nextPos = current.first + directions.at(c);
			if (rooms.IsInside(nextPos))
			{
				searchQueue.push_back({ nextPos, current.second + c });
			}
		}
	}

	return longestPath;
}

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string passcode;
	getline(input, passcode);

	const ArrayMap2D rooms{ {}, 4, 4, '.' };

	string answer = ShortestPathTo(rooms, Point2{ 0, 0 }, Point2{ 3, 3 }, passcode);

	printf("[2016] Puzzle17_A: %s\n", answer.c_str());
}


static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string passcode;
	getline(input, passcode);

	const ArrayMap2D rooms{ {}, 4, 4, '.' };

	int64_t answer = LongestPathTo(rooms, Point2{ 0, 0 }, Point2{ 3, 3 }, passcode);

	printf("[2016] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2016(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
