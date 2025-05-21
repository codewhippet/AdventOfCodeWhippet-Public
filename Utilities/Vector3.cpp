#include "stdafx.h"
#include "Vector3.h"

const std::vector<Vector3>& Vector3::CardinalDirections()
{
    static const std::vector<Vector3> directions =
    {
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
        {  0,  0,  1 },
        {  0,  0, -1 },
    };
    return directions;
}

Vector3 Vector3::MinElements(const Vector3& a, const Vector3& b)
{
    return { std::min(a.X, b.X), std::min(a.Y, b.Y), std::min(a.Z, b.Z) };
}

Vector3 Vector3::MaxElements(const Vector3& a, const Vector3& b)
{
    return { std::max(a.X, b.X), std::max(a.Y, b.Y), std::max(a.Z, b.Z) };
}

int64_t ManhattanDistance(const Vector3& a, const Vector3& b)
{
	return abs(b.X - a.X) + abs(b.Y - a.Y) + abs(b.Z - a.Z);
}
