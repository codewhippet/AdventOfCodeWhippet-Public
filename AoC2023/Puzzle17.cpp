#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2023_Types
{
	static Vector4 Left{ -1, 0, 0, 0 };
	static Vector4 Right{ 1, 0, 1, 0 };
	static Vector4 Up{ 0, -1, 2, 0 };
	static Vector4 Down{ 0, 1, 3, 0 };

	static vector<vector<Vector4>> Directions =
	{
		{ Left, Up, Down },
		{ Right, Up, Down },
		{ Up, Left, Right },
		{ Down, Left, Right },
		{ { 0, 0, 0, 0 }, Down, Right },
	};
}

using namespace Puzzle17_2023_Types;

static void Puzzle17_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap field = ReadPointMap(input);

	Vector4 start{ 0, 0, 4, 3 };
	Point2 end{ field.Size.X - 1, field.Size.Y - 1 };

	multiset<pair<int64_t, Vector4>> openSet{ { 0, start } };
	unordered_map<Vector4, Vector4> cameFrom;
	unordered_map<Vector4, int64_t> score;

	auto h = [&field](const Vector4& p) { return abs((field.Size.X - 1) - p.X) + abs((field.Size.Y - 1) - p.Y); };

	auto neighbourFunction = [&field](const Vector4& p) -> vector<Vector4>
		{
			vector<Vector4> ret;

			const vector<Vector4>& possibleNeighbours = Directions[p.Z];
			if ((p.W < 3) && IsInside(Point2{ p.X + possibleNeighbours[0].X, p.Y + possibleNeighbours[0].Y }, field))
			{
				ret.push_back(Vector4{
					p.X + possibleNeighbours[0].X,
					p.Y + possibleNeighbours[0].Y,
					p.Z,
					p.W + 1 });
			}
			for (int i = 1; i < 3; i++)
			{
				if (IsInside(Point2{ p.X + possibleNeighbours[i].X, p.Y + possibleNeighbours[i].Y }, field))
				{
					ret.push_back(Vector4{
						p.X + possibleNeighbours[i].X,
						p.Y + possibleNeighbours[i].Y,
						possibleNeighbours[i].Z,
						1 });
				}
			}

			return ret;
		};

	score[start] = 0;

	Vector4 last;
	while (openSet.empty() == false)
	{
		Vector4 current = openSet.begin()->second;
		openSet.erase(openSet.begin());

		if (Point2{ current.X, current.Y } == end)
		{
			last = current;
			break;
		}

		const vector<Vector4> neighbours = neighbourFunction(current);
		for (const Vector4& neighbour : neighbours)
		{
			int64_t tentativeScore = score[current] + (field.Data[Point2{ neighbour.X, neighbour.Y }] - '0');

			auto neighbourScoreIt = score.find(neighbour);
			if ((neighbourScoreIt == score.end()) || (tentativeScore < neighbourScoreIt->second))
			{
				cameFrom[neighbour] = current;
				score[neighbour] = tentativeScore;
				int64_t searchPriority = tentativeScore + h(neighbour);
				openSet.insert({ searchPriority, neighbour });
			}
		}
	}

	vector<Vector4> path;
	Vector4 backtrack = last;
	while (cameFrom.contains(backtrack))
	{
		path.push_back(backtrack);
		backtrack = cameFrom[backtrack];
	}

	reverse(path.begin(), path.end());

	int64_t answer = 0;
	for (const Vector4& p : path)
	{
		answer += (field.Data[Point2{ p.X, p.Y }] - '0');
	}

	printf("[2023] Puzzle17_A: %" PRId64 "\n", answer);
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap field = ReadPointMap(input);

	Vector4 start{ 0, 0, 4, 10 };
	Point2 end{ field.Size.X - 1, field.Size.Y - 1 };

	multiset<pair<int64_t, Vector4>> openSet{ { 0, start } };
	unordered_map<Vector4, Vector4> cameFrom;
	unordered_map<Vector4, int64_t> score;

	cameFrom.reserve(1000000);
	score.reserve(1000000);

	auto h = [&field](const Vector4& p) { return abs((field.Size.X - 1) - p.X) + abs((field.Size.Y - 1) - p.Y); };

	auto neighbourFunction = [&field](const Vector4& p) -> vector<Vector4>
		{
			vector<Vector4> ret;

			const vector<Vector4>& possibleNeighbours = Directions[p.Z];
			if ((p.W < 10) && IsInside(Point2{ p.X + possibleNeighbours[0].X, p.Y + possibleNeighbours[0].Y }, field))
			{
				ret.push_back(Vector4{
					p.X + possibleNeighbours[0].X,
					p.Y + possibleNeighbours[0].Y,
					p.Z,
					p.W + 1 });
			}
			if (p.W >= 4)
			{
				for (int i = 1; i < 3; i++)
				{
					if (IsInside(Point2{ p.X + possibleNeighbours[i].X, p.Y + possibleNeighbours[i].Y }, field))
					{
						ret.push_back(Vector4{
							p.X + possibleNeighbours[i].X,
							p.Y + possibleNeighbours[i].Y,
							possibleNeighbours[i].Z,
							1 });
					}
				}
			}

			return ret;
		};

	score[start] = 0;

	Vector4 last;
	while (openSet.empty() == false)
	{
		Vector4 current = openSet.begin()->second;
		openSet.erase(openSet.begin());

		if (Point2{ current.X, current.Y } == end)
		{
			last = current;
			break;
		}

		const vector<Vector4> neighbours = neighbourFunction(current);
		for (const Vector4& neighbour : neighbours)
		{
			int64_t tentativeScore = score[current] + (field.Data[Point2{ neighbour.X, neighbour.Y }] - '0');

			auto neighbourScoreIt = score.find(neighbour);
			if ((neighbourScoreIt == score.end()) || (tentativeScore < neighbourScoreIt->second))
			{
				cameFrom[neighbour] = current;
				score[neighbour] = tentativeScore;
				int64_t searchPriority = tentativeScore + h(neighbour);
				openSet.insert({ searchPriority, neighbour });
			}
		}
	}

	vector<Vector4> path;
	Vector4 backtrack = last;
	while (cameFrom.contains(backtrack))
	{
		path.push_back(backtrack);
		backtrack = cameFrom[backtrack];
	}

	reverse(path.begin(), path.end());

	int64_t answer = 0;
	for (const Vector4& p : path)
	{
		answer += (field.Data[Point2{ p.X, p.Y }] - '0');
	}

	printf("[2023] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2023(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
