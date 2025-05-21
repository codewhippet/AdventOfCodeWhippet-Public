#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2023_Types
{
	struct Brick
	{
		int Id;
		vector<Vector3> Cubes;
		int64_t StartingHeight;
	};
}

using namespace Puzzle22_2023_Types;

static vector<Brick> ReadBricks(istream& input)
{
	vector<Brick> bricks;

	auto lines = ReadAllLines(input);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Brick b;
		b.Id = (int)i;

		Vector3 start, stop;
		int scanned = sscanf(lines[i].c_str(), "%lld,%lld,%lld~%lld,%lld,%lld",
			&start.X,
			&start.Y,
			&start.Z,
			&stop.X,
			&stop.Y,
			&stop.Z);
		assert(scanned == 6);
		(void)scanned;

		if (start != stop)
		{
			b.Cubes = Enumerable::Line3D(start, stop)->ToVector();
		}
		else
		{
			b.Cubes.push_back(start);
		}
		assert(b.Cubes.size() > 0);
		b.StartingHeight = min(start.Z, stop.Z);

		bricks.push_back(b);
	}

	return bricks;
}

static int BricksCollapseIf(map<int, set<int>> brickStoppedBy, const map<int, set<int>>& brickSupports, int brickToRemove)
{
	set<int> bricksRemoved;
	vector<int> removalQueue{ brickToRemove };
	for (size_t i = 0; i < removalQueue.size(); i++)
	{
		int currentRemoval = removalQueue[i];
		bricksRemoved.insert(currentRemoval);
		if (brickSupports.contains(currentRemoval))
		{
			for (int nextRemoval : brickSupports.at(currentRemoval))
			{
				if (brickStoppedBy[nextRemoval].erase(currentRemoval) && brickStoppedBy[nextRemoval].empty())
				{
					removalQueue.push_back(nextRemoval);
				}
			}
		}
	}
	return (int)bricksRemoved.size() - 1;
}

static void Puzzle22_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto bricks = ReadBricks(input);
	sort(bricks.begin(), bricks.end(),
		[](const auto& a, const auto& b)
		{
			return a.StartingHeight < b.StartingHeight;
		});

	map<Vector3, int> brickMap;
	map<int, set<int>> brickStoppedBy;

	for (size_t brickIndex = 0; brickIndex < bricks.size(); brickIndex++)
	{
		const Brick& brick = bricks[brickIndex];

		Vector3 drop = { 0, 0, 0 };
		while (true)
		{
			Vector3 candidateDrop = drop + Vector3{ 0, 0, -1 };
			bool hitSomething = false;
			for (const auto& cube : brick.Cubes)
			{
				Vector3 candidateCube = cube + candidateDrop;

				// Stopped by ground
				if (candidateCube.Z <= 0)
				{
					hitSomething = true;
					break;
				}

				// Stopped by one or more cubes
				if (brickMap.contains(candidateCube))
				{
					hitSomething = true;
					brickStoppedBy[brick.Id].insert(brickMap[candidateCube]);
				}
			}

			if (hitSomething)
			{
				break;
			}

			drop = candidateDrop;
		}

		// Place the block in its final position
		for (const auto& cube : brick.Cubes)
		{
			brickMap[cube + drop] = brick.Id;
		}
	}

	// Assume all bricks can be disintegrated
	set<int> canBeDisintegrated;
	for (size_t i = 0; i < bricks.size(); i++)
	{
		canBeDisintegrated.insert((int)i);
	}

	// Can't disintegrate bricks that are the sole support for another brick
	for (const auto& brickStoppers : brickStoppedBy)
	{
		if (brickStoppers.second.size() == 1)
		{
			canBeDisintegrated.erase(*brickStoppers.second.begin());
		}
	}

	int64_t answer = canBeDisintegrated.size();

	printf("[2023] Puzzle22_A: %" PRId64 "\n", answer);
}

static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto bricks = ReadBricks(input);
	sort(bricks.begin(), bricks.end(),
		[](const auto& a, const auto& b)
		{
			return a.StartingHeight < b.StartingHeight;
		});

	map<Vector3, int> brickMap;
	map<int, set<int>> brickStoppedBy;

	for (size_t brickIndex = 0; brickIndex < bricks.size(); brickIndex++)
	{
		const Brick& brick = bricks[brickIndex];

		Vector3 drop = { 0, 0, 0 };
		while (true)
		{
			Vector3 candidateDrop = drop + Vector3{ 0, 0, -1 };
			bool hitSomething = false;
			for (const auto& cube : brick.Cubes)
			{
				Vector3 candidateCube = cube + candidateDrop;

				// Stopped by ground
				if (candidateCube.Z <= 0)
				{
					hitSomething = true;
					break;
				}

				// Stopped by one or more cubes
				if (brickMap.contains(candidateCube))
				{
					hitSomething = true;
					brickStoppedBy[brick.Id].insert(brickMap[candidateCube]);
				}
			}

			if (hitSomething)
			{
				break;
			}

			drop = candidateDrop;
		}

		// Place the block in its final position
		for (const auto& cube : brick.Cubes)
		{
			brickMap[cube + drop] = brick.Id;
		}
	}

	map<int, set<int>> brickSupports;
	for (const auto& stoppedBy : brickStoppedBy)
	{
		for (int stopper : stoppedBy.second)
		{
			brickSupports[stopper].insert(stoppedBy.first);
		}
	}

	int64_t answer = 0;
	for (int i = 0; i < bricks.size(); i++)
	{
		int numCollapsedBricks = BricksCollapseIf(brickStoppedBy, brickSupports, i);
		answer += numCollapsedBricks;
	}

	printf("[2023] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2023(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
