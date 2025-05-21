#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2017_Types
{
}

using namespace Puzzle12_2017_Types;

static set<int64_t> GetConnectedGroup(const map<int64_t, vector<int64_t>>& pipes, int64_t startFrom)
{
	vector<int64_t> searchQueue{ startFrom };
	set<int64_t> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		int64_t current = searchQueue[i];
		if (visited.contains(current))
			continue;

		visited.insert(current);
		searchQueue.insert(searchQueue.end(), pipes.at(current).begin(), pipes.at(current).end());
	}
	return visited;
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int64_t, vector<int64_t>> pipes;
	for (const auto& line : ScanfEachLine<int64_t, char[128]>(input, "%d <-> %[0123456789, ]s"))
	{
		pipes[get<0>(line)] = ReadAsVectorOfNumbers(&get<1>(line)[0]);
	}

	int64_t answer = GetConnectedGroup(pipes, 0).size();

	printf("[2017] Puzzle12_A: %" PRId64 "\n", answer);
}


static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int64_t, vector<int64_t>> pipes;
	for (const auto& line : ScanfEachLine<int64_t, char[128]>(input, "%d <-> %[0123456789, ]s"))
	{
		pipes[get<0>(line)] = ReadAsVectorOfNumbers(&get<1>(line)[0]);
	}

	int64_t answer = 0;
	while (pipes.empty() == false)
	{
		set<int64_t> group = GetConnectedGroup(pipes, pipes.begin()->first);
		for (int64_t node : group)
		{
			pipes.erase(node);
		}
		answer++;
	}

	printf("[2017] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2017(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
