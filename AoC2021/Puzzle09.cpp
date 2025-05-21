#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2021_Types
{
	struct Board
	{
		map<Point2, int> Heights;
		int Width;
		int Height;
	};
}

using namespace Puzzle09_2021_Types;

static Board ReadBoard(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	Board b;
	b.Width = (int)lines[0].size();
	b.Height = (int)lines.size();

	for (int row = 0; row < b.Height; row++)
	{
		const string& line = lines[row];
		for (int column = 0; column < b.Width; column++)
		{
			b.Heights[Point2{ column, row }] = line[column] - '0';
		}
	}

	return b;
}

static int GetHeight(Point2 p, const Board& b)
{
	map<Point2, int>::const_iterator heightIt = b.Heights.find(p);
	if (heightIt != b.Heights.end())
	{
		return heightIt->second;
	}
	return INT_MAX;
}

static vector<Point2> GetNeighbours(Point2 p, const Board& b)
{
	vector<Point2> neighbours;

	if (p.X - 1 >= 0) neighbours.push_back(Point2{ p.X - 1, p.Y });
	if (p.X + 1 < b.Width) neighbours.push_back(Point2{ p.X + 1, p.Y });
	if (p.Y - 1 >= 0) neighbours.push_back(Point2{ p.X, p.Y - 1 });
	if (p.Y + 1 < b.Height) neighbours.push_back(Point2{ p.X, p.Y + 1 });

	return neighbours;
}

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Board b = ReadBoard(input);

	int64_t answer = 0;
	for (map<Point2, int>::const_reference point : b.Heights)
	{
		int thisHeight = point.second;
		int lowestNeighbour = MakeEnumerator(GetNeighbours(point.first, b))
			->Select<int>([thisHeight, &b](const Point2& neighbour) { return GetHeight(neighbour, b); })
			->Min();
		if (thisHeight < lowestNeighbour)
		{
			answer += thisHeight + 1;
		}
	}

	printf("[2021] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Board b = ReadBoard(input);

	set<Point2> lowPoints;
	for (map<Point2, int>::const_reference point : b.Heights)
	{
		int thisHeight = point.second;
		int lowestNeighbour = MakeEnumerator(GetNeighbours(point.first, b))
			->Select<int>([thisHeight, &b](const Point2& neighbour) { return GetHeight(neighbour, b); })
			->Min();
		if (thisHeight < lowestNeighbour)
		{
			lowPoints.insert(point.first);
		}
	}

	vector<int> basinSizes;
	for (const Point2& point : lowPoints)
	{
		set<Point2> basin;
		set<Point2> visited;
		vector<Point2> searchQueue;

		searchQueue.push_back(point);
		for (size_t searchPos = 0; searchPos < searchQueue.size(); searchPos++)
		{
			Point2 candidate = searchQueue[searchPos];

			// Skip if we've visited this already
			if (visited.insert(candidate).second == false)
				continue;

			// Skip if this isn't part of our basin
			if (GetHeight(candidate, b) >= 9)
				continue;

			// This is part of our basin
			basin.insert(candidate);

			// Surrounding points
			for (const Point2& neighbour : GetNeighbours(candidate, b))
			{
				if (visited.find(neighbour) == visited.end())
				{
					searchQueue.push_back(neighbour);
				}
			}
		}

		basinSizes.push_back((int)basin.size());
	}

	sort(basinSizes.rbegin(), basinSizes.rend());

	int64_t answer = basinSizes[0] * basinSizes[1] * basinSizes[2];

	printf("[2021] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2021(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
