#include "stdafx.h"
#include "PointMap.h"

PointMap ReadPointMap(std::istream& input, char emptyChar)
{
	PointMap p;
	std::vector<std::string> lines = ReadAllLines(input);
	p.Size.X = lines[0].size();
	p.Size.Y = lines.size();

	for (int64_t y = 0; y < p.Size.Y; y++)
	{
		for (int64_t x = 0; x < p.Size.X; x++)
		{
			char c = lines[y][x];
			if (c != emptyChar)
			{
				p.Data[{x, y}] = c;
			}
		}
	}

	return p;
}

void PrintPointMap(const PointMap& map, char emptyChar)
{
	for (int64_t y = 0; y < map.Size.Y; y++)
	{
		for (int64_t x = 0; x < map.Size.X; x++)
		{
			std::map<Point2, char>::const_iterator entry = map.Data.find({ x, y });
			char c = (entry == map.Data.end()) ? emptyChar : entry->second;
			printf("%c", c);
		}
		printf("\n");
	}
}

bool IsInside(const Point2& p, const PointMap& map)
{
	return (p.X >= 0) && (p.Y >= 0) && (p.X < map.Size.X) && (p.Y < map.Size.Y);
}

bool IsOutside(const Point2& p, const PointMap& map)
{
	return (p.X < 0) || (p.Y < 0) || (p.X >= map.Size.X) || (p.Y >= map.Size.Y);
}

PointMap RotateClockwise(const PointMap& map)
{
	PointMap newMap;
	for (std::map<Point2, char>::const_reference point : map.Data)
	{
		newMap.Data[Point2{ -point.first.Y, point.first.X }] = point.second;
	}
	return newMap;
}

