#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2020_Types
{
}

using namespace Puzzle17_2020_Types;

static set<Vector3> ReadStartingConfiguration(istream& input)
{
	set<Vector3> config;

	vector<string> lines = ReadAllLines(input);
	for (size_t y = 0; y < lines.size(); y++)
	{
		for (size_t x = 0; x < lines[0].size(); x++)
		{
			if (lines[y][x] == '#')
			{
				config.insert({ (int64_t)x, (int64_t)y, 0 });
			}
		}
	}

	return config;
}

static set<Vector4> ReadStartingConfiguration4D(istream& input)
{
	set<Vector4> config;

	vector<string> lines = ReadAllLines(input);
	for (size_t y = 0; y < lines.size(); y++)
	{
		for (size_t x = 0; x < lines[0].size(); x++)
		{
			if (lines[y][x] == '#')
			{
				config.insert({ (int64_t)x, (int64_t)y, 0, 0 });
			}
		}
	}

	return config;
}

static vector<Vector3> Generate3DNeighbours()
{
	vector<Vector3> neighbours;
	neighbours.reserve((3 * 3 * 3) - 1);
	for (int64_t z : { -1, 0, 1 })
	{
		for (int64_t y : { -1, 0, 1 })
		{
			for (int64_t x : { -1, 0, 1 })
			{
				Vector3 neighbour{ x, y, z };
				if (neighbour != Vector3{ 0, 0, 0 })
				{
					neighbours.push_back({ x, y, z });
				}
			}
		}
	}
	return neighbours;
}

static vector<Vector4> Generate4DNeighbours()
{
	vector<Vector4> neighbours;
	neighbours.reserve((3 * 3 * 3 * 3) - 1);
	for (int64_t w : { -1, 0, 1 })
	{
		for (int64_t z : { -1, 0, 1 })
		{
			for (int64_t y : { -1, 0, 1 })
			{
				for (int64_t x : { -1, 0, 1 })
				{
					Vector4 neighbour{ x, y, z, w };
					if (neighbour != Vector4{ 0, 0, 0, 0 })
					{
						neighbours.push_back({ x, y, z, w });
					}
				}
			}
		}
	}
	return neighbours;
}

template <typename VEC>
static set<VEC> Step(const set<VEC> &current, const vector<VEC> &offsets)
{
	set<VEC> next;

	map<VEC, int64_t> inactive;
	for (const VEC& cube : current)
	{
		inactive[cube] += 10;

		int neighbourCount = 0;
		for (const VEC& neighbourOffset : offsets)
		{
			VEC neighbour = cube + neighbourOffset;
			if (current.contains(neighbour))
			{
				neighbourCount++;
			}
			inactive[neighbour]++;
		}
		if ((neighbourCount == 2) || (neighbourCount == 3))
		{
			next.insert(cube);
		}
	}

	for (typename map<VEC, int64_t>::const_reference inactiveCount : inactive)
	{
		if (inactiveCount.second == 3)
		{
			next.insert(inactiveCount.first);
		}
	}

	return next;
}

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<Vector3> conway = ReadStartingConfiguration(input);
	vector<Vector3> neighbours = Generate3DNeighbours();

	for (int i = 0; i < 6; i++)
	{
		conway = Step(conway, neighbours);
	}

	int64_t answer = conway.size();

	printf("[2020] Puzzle17_A: %" PRId64 "\n", answer);
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<Vector4> conway = ReadStartingConfiguration4D(input);
	vector<Vector4> neighbours = Generate4DNeighbours();

	for (int i = 0; i < 6; i++)
	{
		conway = Step(conway, neighbours);
	}

	int64_t answer = conway.size();

	printf("[2020] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2020(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
