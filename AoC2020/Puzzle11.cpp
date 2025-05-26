#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2020_Types
{
}

using namespace Puzzle11_2020_Types;

static map<Point2, vector<Point2>> GenerateVisibleNeighbours(const ArrayMap2D& seating)
{
	map<Point2, vector<Point2>> neighbours;

	// Left/right neighbours
	{
		for (int64_t y : seating.AxisRangeY())
		{
			optional<Point2> lastSeat;
			for (int64_t x : seating.AxisRangeX())
			{
				if (seating(x, y) == 'L')
				{
					if (lastSeat)
					{
						neighbours[Point2{ x, y }].push_back(*lastSeat);
						neighbours[*lastSeat].push_back(Point2{ x, y });
					}

					lastSeat = Point2{ x, y };
				}
			}
		}
	}

	// Up/down neighbours
	{
		for (int64_t x : seating.AxisRangeX())
		{
			optional<Point2> lastSeat;
			for (int64_t y : seating.AxisRangeY())
			{
				if (seating(x, y) == 'L')
				{
					if (lastSeat)
					{
						neighbours[Point2{ x, y }].push_back(*lastSeat);
						neighbours[*lastSeat].push_back(Point2{ x, y });
					}

					lastSeat = Point2{ x, y };
				}
			}
		}
	}

	// NW-SE Diagonal neighbours
	{
		int64_t width = seating.GetWidth();
		int64_t height = seating.GetHeight();

		for (int64_t w = -height; w < width; w++)
		{
			int64_t x = w;

			optional<Point2> lastSeat;
			for (int64_t y = 0; y < height; y++, x++)
			{
				if (seating(x, y) == 'L')
				{
					if (lastSeat)
					{
						neighbours[Point2{ x, y }].push_back(*lastSeat);
						neighbours[*lastSeat].push_back(Point2{ x, y });
					}

					lastSeat = Point2{ x, y };
				}
			}
		}
	}

	// NE-SW Diagonal neighbours
	{
		int64_t width = seating.GetWidth();
		int64_t height = seating.GetHeight();

		for (int64_t w = width + height; w > 0; w--)
		{
			int64_t x = w;

			optional<Point2> lastSeat;
			for (int64_t y = 0; y < height; y++, x--)
			{
				if (seating(x, y) == 'L')
				{
					if (lastSeat)
					{
						neighbours[Point2{ x, y }].push_back(*lastSeat);
						neighbours[*lastSeat].push_back(Point2{ x, y });
					}

					lastSeat = Point2{ x, y };
				}
			}
		}
	}

	return neighbours;
}

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D seating = ReadArrayMap(input);
	while (true)
	{
		ArrayMap2D newSeating(ArrayMap2DOptions::CloneAsInvalid, seating);

		int64_t swaps = 0;
		for (const auto &seat : seating.Grid())
		{
			auto [current, currentTile] = seat;
			if (currentTile == '.')
			{
				continue;
			}

			int occupiedCount = 0;
			for (const Point2& offset : Point2::CardinalAndDiagonalDirections())
			{
				if (seating(current + offset) == '#')
				{
					occupiedCount++;
				}
			}

			if (currentTile == 'L' && occupiedCount == 0)
			{
				newSeating(current) = '#';
				swaps++;
			}
			else if (currentTile == '#' && occupiedCount >= 4)
			{
				newSeating(current) = 'L';
				swaps++;
			}
			else
			{
				newSeating(current) = currentTile;
			}
		}

		if (swaps == 0)
		{
			break;
		}

		seating = move(newSeating);
	}

	int64_t answer = seating.Count('#');

	printf("[2020] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D seating = ReadArrayMap(input);

	map<Point2, vector<Point2>> visibleNeighbours = GenerateVisibleNeighbours(seating);
	while (true)
	{
		ArrayMap2D newSeating(ArrayMap2DOptions::CloneAsInvalid, seating);

		int64_t swaps = 0;
		for (const auto& seat : seating.Grid())
		{
			auto [current, currentTile] = seat;
			if (currentTile == '.')
			{
				continue;
			}

			int occupiedCount = 0;
			for (const Point2& neighbour : visibleNeighbours[current])
			{
				if (seating(neighbour) == '#')
				{
					occupiedCount++;
				}
			}

			if (currentTile == 'L' && occupiedCount == 0)
			{
				newSeating(current) = '#';
				swaps++;
			}
			else if (currentTile == '#' && occupiedCount >= 5)
			{
				newSeating(current) = 'L';
				swaps++;
			}
			else
			{
				newSeating(current) = currentTile;
			}
		}

		if (swaps == 0)
		{
			break;
		}

		seating = move(newSeating);
	}

	int64_t answer = seating.Count('#');

	printf("[2020] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2020(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
