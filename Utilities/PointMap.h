#pragma once

#include "Point2.h"
#include <map>
#include <istream>

struct PointMap
{
	std::map<Point2, char> Data;
	Point2 Size{ 0, 0 };
};

PointMap ReadPointMap(std::istream& input, char emptyChar = '.');
void PrintPointMap(const PointMap& map, char emptyChar = '.');
bool IsInside(const Point2& p, const PointMap& map);
bool IsOutside(const Point2& p, const PointMap& map);

PointMap RotateClockwise(const PointMap& map);