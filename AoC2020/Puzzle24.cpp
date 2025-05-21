#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2020_Types
{
	struct StateTransform
	{
		int NewState;
		Vector3 (*Transform)(const Vector3&);
	};
}

using namespace Puzzle24_2020_Types;

static Vector3 Noop(const Vector3& v)
{
	return v;
}

static Vector3 East(const Vector3& v)
{
	return { v.X, v.Y, v.Z + 1 };
}

static Vector3 SouthEast(const Vector3& v)
{
	return { 1 - v.X, v.Y + v.X, v.Z + v.X };
}

static Vector3 SouthWest(const Vector3& v)
{
	return { 1 - v.X, v.Y + v.X, v.Z - (1 - v.X) };
}

static Vector3 West(const Vector3& v)
{
	return { v.X, v.Y, v.Z - 1 };
}

static Vector3 NorthWest(const Vector3& v)
{
	return { 1 - v.X, v.Y - (1 - v.X), v.Z - (1 - v.X) };
}

static Vector3 NorthEast(const Vector3& v)
{
	return { 1 - v.X, v.Y - (1 - v.X), v.Z + v.X };
}

static void Puzzle24_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<pair<int, char>, StateTransform> stateMachine;
	
	stateMachine[{0, 'e'}] = { 0, &East };
	stateMachine[{0, 'w'}] = { 0, &West };

	stateMachine[{0, 's'}] = { 1, &Noop };
	stateMachine[{1, 'e'}] = { 0, &SouthEast };
	stateMachine[{1, 'w'}] = { 0, &SouthWest };

	stateMachine[{0, 'n'}] = { 2, &Noop };
	stateMachine[{2, 'e'}] = { 0, &NorthEast };
	stateMachine[{2, 'w'}] = { 0, &NorthWest };


	set<Vector3> blackTiles;
	for (const string& line : ReadAllLines(input))
	{
		int currentState = 0;
		Vector3 currentPos{ 0, 0, 0 };

		for (char c : line)
		{
			const StateTransform& transform = stateMachine.at({ currentState, c });
			currentPos = transform.Transform(currentPos);
			currentState = transform.NewState;
		}

		if (blackTiles.erase(currentPos) == 0)
		{
			blackTiles.insert(currentPos);
		}
	}

	int64_t answer = blackTiles.size();

	printf("[2020] Puzzle24_A: %" PRId64 "\n", answer);
}

static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<pair<int, char>, StateTransform> stateMachine;

	stateMachine[{0, 'e'}] = { 0, &East };
	stateMachine[{0, 'w'}] = { 0, &West };

	stateMachine[{0, 's'}] = { 1, &Noop };
	stateMachine[{1, 'e'}] = { 0, &SouthEast };
	stateMachine[{1, 'w'}] = { 0, &SouthWest };

	stateMachine[{0, 'n'}] = { 2, &Noop };
	stateMachine[{2, 'e'}] = { 0, &NorthEast };
	stateMachine[{2, 'w'}] = { 0, &NorthWest };


	set<Vector3> blackTiles;
	for (const string& line : ReadAllLines(input))
	{
		int currentState = 0;
		Vector3 currentPos{ 0, 0, 0 };

		for (char c : line)
		{
			const StateTransform& transform = stateMachine.at({ currentState, c });
			currentPos = transform.Transform(currentPos);
			currentState = transform.NewState;
		}

		if (blackTiles.erase(currentPos) == 0)
		{
			blackTiles.insert(currentPos);
		}
	}

	const vector<Vector3(*)(const Vector3&)> neighbourTransforms = { &East, &SouthEast, &SouthWest, &West, &NorthWest, &NorthEast };

	// Game of life
	for (int i = 0; i < 100; i++)
	{
		//printf("Day %d: %" PRIu64 "\n", i, blackTiles.size());

		map<Vector3, int> whiteTileAdjacencyCounts;

		set<Vector3> newBlackTiles;
		for (const Vector3& current : blackTiles)
		{
			int blackNeighbours = 0;
			for (Vector3(*neighbourTransform)(const Vector3&) : neighbourTransforms)
			{
				Vector3 neighbour = neighbourTransform(current);
				if (blackTiles.contains(neighbour))
				{
					blackNeighbours++;
				}
				else
				{
					whiteTileAdjacencyCounts[neighbour]++;
				}
			}

			if ((blackNeighbours == 1) || (blackNeighbours == 2))
			{
				newBlackTiles.insert(current);
			}
		}

		for (map<Vector3, int>::const_reference whiteTileAdjacency : whiteTileAdjacencyCounts)
		{
			if (whiteTileAdjacency.second == 2)
			{
				newBlackTiles.insert(whiteTileAdjacency.first);
			}
		}

		blackTiles = move(newBlackTiles);
	}

	int64_t answer = blackTiles.size();

	printf("[2020] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2020(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
