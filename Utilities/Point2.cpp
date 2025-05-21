#include "stdafx.h"
#include "Point2.h"

const std::vector<Point2>& Point2::CardinalDirections()
{
    static const std::vector<Point2> directions =
    {
        Point2::North(),
        Point2::East(),
        Point2::South(),
        Point2::West()
    };
    return directions;
}

const std::vector<Point2>& Point2::CardinalAndDiagonalDirections()
{
    static const std::vector<Point2> directions =
    {
        Point2::North(),
        Point2::North() + Point2::East(),
        Point2::East(),
        Point2::South() + Point2::East(),
        Point2::South(),
        Point2::South() + Point2::West(),
        Point2::West(),
        Point2::North() + Point2::West()
    };
    return directions;
}

const std::map<char, Point2>& Point2::DirectionFromChar()
{
    static const std::map<char, Point2> directions =
    {
        { '^', Point2::North() },
        { '>', Point2::East() },
        { 'v', Point2::South() },
        { '<', Point2::West() }
    };
    return directions;
}

Point2 Point2::RotateClockwise(const Point2& direction)
{
    return { -direction.Y, direction.X };
}

Point2 Point2::RotateAnticlockwise(const Point2& direction)
{
    return { direction.Y, -direction.X };
}

Point2 Point2::MinElements(const Point2& a, const Point2& b)
{
    return { std::min(a.X, b.X), std::min(a.Y, b.Y) };
}

Point2 Point2::MaxElements(const Point2& a, const Point2& b)
{
    return { std::max(a.X, b.X), std::max(a.Y, b.Y) };
}

int64_t ManhattanDistance(const Point2& a, const Point2& b)
{
	return abs(b.X - a.X) + abs(b.Y - a.Y);
}
