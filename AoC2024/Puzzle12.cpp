#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2024_Types
{
}

using namespace Puzzle12_2024_Types;

static void MarkRegion(Point2 start, ArrayMap2D* fields, char marker)
{
	char regionId = (*fields)(start);

	vector<Point2> searchQueue{ start };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 current = searchQueue[i];
		if ((*fields)(current) == regionId)
		{
			(*fields)(current) = marker;
			for (const Point2& dir : Point2::CardinalDirections())
			{
				Point2 next = current + dir;
				if ((*fields)(next) == regionId)
				{
					searchQueue.push_back(next);
				}
			}
		}
	}
}

static pair<int64_t, int64_t> GetAreaAndPerimeter(char marker, const ArrayMap2D& fields)
{
	int64_t area = 0;
	int64_t perimeter = 0;

	for (int64_t y : fields.AxisRangeY())
	{
		for (int64_t x : fields.AxisRangeX())
		{
			Point2 current{ x, y };
			if (fields(current) == marker)
			{
				area++;
				perimeter += 4;
				for (const Point2& dir : Point2::CardinalDirections())
				{
					if (fields(current + dir) == marker)
					{
						perimeter--;
					}
				}
			}
		}
	}

	return { area, perimeter };
}

static pair<int64_t, int64_t> GetAreaAndSides(char marker, const ArrayMap2D& fields)
{
	int64_t area = 0;
	int64_t sides = 0;

	for (int64_t y = -1; y < fields.GetHeight() + 1; y++)
	{
		for (int64_t x = -1; x < fields.GetWidth() + 1; x++)
		{
			Point2 current{ x, y };

			bool different[4];
			if (fields(current) == marker)
			{
				area++;

				different[0] = (fields(current + Point2::North()) != marker);
				different[1] = (fields(current + Point2::East()) != marker);
				different[2] = (fields(current + Point2::South()) != marker);
				different[3] = (fields(current + Point2::West()) != marker);
			}
			else
			{
				different[0] = (fields(current + Point2::North()) == marker);
				different[1] = (fields(current + Point2::East()) == marker);
				different[2] = (fields(current + Point2::South()) == marker);
				different[3] = (fields(current + Point2::West()) == marker);
			}

			for (int i = 0; i < 4; i++)
			{
				if (different[i] && different[(i + 1) % 4])
				{
					sides++;
				}
			}
		}
	}

	// Remove double-counting of diagonal junctions
	for (int64_t y : fields.AxisRangeY())
	{
		for (int64_t x : fields.AxisRangeX())
		{
			Point2 current{ x, y };
			bool nw = (fields(current) == marker);
			bool ne = (fields(current + Point2::East()) == marker);
			bool se = (fields(current + Point2::SouthEast()) == marker);
			bool sw = (fields(current + Point2::South()) == marker);

			if ((nw && !ne && se && !sw) ||
				(!nw && ne && !se && sw))
			{
				sides -= 2;
			}
		}
	}

	return { area, sides };
}

static int64_t CostRegion(Point2 start, ArrayMap2D* fields)
{
	MarkRegion(start, fields, '*');
	pair<int64_t, int64_t> areaAndPerimeter = GetAreaAndPerimeter('*', *fields);
	fields->Replace('*', fields->GetInvalidCharacter());

	return areaAndPerimeter.first * areaAndPerimeter.second;
}

static int64_t CostRegion2(Point2 start, ArrayMap2D* fields)
{
	MarkRegion(start, fields, '*');
	pair<int64_t, int64_t> areaAndSides = GetAreaAndSides('*', *fields);
	fields->Replace('*', fields->GetInvalidCharacter());

	return areaAndSides.first * areaAndSides.second;
}

static void Puzzle12_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D fields = ReadArrayMap(input);

	int64_t answer = 0;

	for (int64_t y : fields.AxisRangeY())
	{
		for (int64_t x : fields.AxisRangeX())
		{
			if (fields({ x, y }) != '.')
			{
				answer += CostRegion({ x, y }, &fields);
			}
		}
	}

	printf("[2024] Puzzle12_A: %" PRId64 "\n", answer);
}

static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D fields = ReadArrayMap(input);

	int64_t answer = 0;

	for (int64_t y : fields.AxisRangeY())
	{
		for (int64_t x : fields.AxisRangeX())
		{
			if (fields({ x, y }) != '.')
			{
				answer += CostRegion2({ x, y }, &fields);
			}
		}
	}

	printf("[2024] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2024(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
