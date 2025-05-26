#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2021_Types
{
	struct Board
	{
		map<Point2, int> Octos;
		int Width;
		int Height;
	};
}

using namespace Puzzle11_2021_Types;

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
			b.Octos[Point2{ column, row }] = line[column] - '0';
		}
	}

	return b;
}

static vector<Point2> GetNeighbours(Point2 p, const Board& b)
{
	vector<Point2> neighbours;

	bool left = (p.X - 1 >= 0);
	bool right = (p.X + 1 < b.Width);
	bool up = (p.Y - 1 >= 0);
	bool down = (p.Y + 1 < b.Height);

	if (left) neighbours.push_back(Point2{ p.X - 1, p.Y });
	if (right) neighbours.push_back(Point2{ p.X + 1, p.Y });
	if (up) neighbours.push_back(Point2{ p.X, p.Y - 1 });
	if (down) neighbours.push_back(Point2{ p.X, p.Y + 1 });

	if (left && up) neighbours.push_back(Point2{ p.X - 1, p.Y - 1 });
	if (right && up) neighbours.push_back(Point2{ p.X + 1, p.Y - 1 });
	if (left && down) neighbours.push_back(Point2{ p.X - 1, p.Y + 1 });
	if (right && down) neighbours.push_back(Point2{ p.X + 1, p.Y + 1 });

	return neighbours;
}

static vector<Point2> IncreaseEnergy(Board* pBoard)
{
	vector<Point2> flashers;
	for (map<Point2, int>::reference octo : pBoard->Octos)
	{
		if (++octo.second > 9)
		{
			flashers.push_back(octo.first);
		}
	}
	return flashers;
}

static void Puzzle11_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Board b = ReadBoard(input);

	size_t flashes = 0;

	for (int i = 0; i < 100; i++)
	{
		set<Point2> flashed;

		vector<Point2> flashQueue = IncreaseEnergy(&b);
		for (size_t pos = 0; pos < flashQueue.size(); pos++)
		{
			Point2 octo = flashQueue[pos];

			if (flashed.insert(octo).second == false)
			{
				continue;
			}

			for (Point2 neighbour : GetNeighbours(octo, b))
			{
				if (flashed.find(neighbour) != flashed.end())
				{
					continue;
				}

				if (++b.Octos[neighbour] > 9)
				{
					flashQueue.push_back(neighbour);
				}
			}
		}

		for (Point2 flasher : flashed)
		{
			b.Octos[flasher] = 0;
		}

		flashes += flashed.size();
	}

	int64_t answer = flashes;

	printf("[2021] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Board b = ReadBoard(input);

	int64_t answer = 0;

	while (true)
	{
		set<Point2> flashed;

		vector<Point2> flashQueue = IncreaseEnergy(&b);
		for (size_t pos = 0; pos < flashQueue.size(); pos++)
		{
			Point2 octo = flashQueue[pos];

			if (flashed.insert(octo).second == false)
			{
				continue;
			}

			for (Point2 neighbour : GetNeighbours(octo, b))
			{
				if (flashed.find(neighbour) != flashed.end())
				{
					continue;
				}

				if (++b.Octos[neighbour] > 9)
				{
					flashQueue.push_back(neighbour);
				}
			}
		}

		for (Point2 flasher : flashed)
		{
			b.Octos[flasher] = 0;
		}

		answer++;

		if (flashed.size() == b.Octos.size())
			break;
	}

	printf("[2021] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2021(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
