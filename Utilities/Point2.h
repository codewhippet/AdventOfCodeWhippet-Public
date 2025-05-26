#pragma once

#include <functional>
#include <vector>
#include <map>
#include <stdint.h>

struct Point2
{
	int64_t X = 0;
	int64_t Y = 0;

	auto operator<=>(const Point2&) const = default;
	bool operator==(const Point2&) const = default;

	static constexpr Point2 Up() { return Point2{ 0, -1 }; }
	static constexpr Point2 Down() { return Point2{ 0, 1 }; }
	static constexpr Point2 Left() { return Point2{ -1, 0 }; }
	static constexpr Point2 Right() { return Point2{ 1, 0 }; }

	static constexpr Point2 North() { return Point2{ 0, -1 }; }
	static constexpr Point2 South() { return Point2{ 0, 1 }; }
	static constexpr Point2 West() { return Point2{ -1, 0 }; }
	static constexpr Point2 East() { return Point2{ 1, 0 }; }

	static constexpr Point2 NorthEast() { return { 1, -1 }; }
	static constexpr Point2 NorthWest() { return { -1, -1 }; }
	static constexpr Point2 SouthEast() { return { 1, 1 }; }
	static constexpr Point2 SouthWest() { return { -1, 1 }; }

	static const std::vector<Point2>& CardinalDirections();
	static const std::vector<Point2>& CardinalAndDiagonalDirections();
	static const std::map<char, Point2>& DirectionFromChar();

	static Point2 RotateClockwise(const Point2& direction);
	static Point2 RotateAnticlockwise(const Point2& direction);

	static constexpr Point2 Min() { return Point2{ std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min() }; }
	static constexpr Point2 Max() { return Point2{ std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max() }; }

	static Point2 MinElements(const Point2& a, const Point2& b);
	static Point2 MaxElements(const Point2& a, const Point2& b);

	static int64_t GetX(const Point2& p)
	{
		return p.X;
	}

	static int64_t GetY(const Point2& p)
	{
		return p.Y;
	}

	Point2& operator+=(const Point2& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}

	Point2& operator-=(const Point2& other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}

	Point2& operator*=(int64_t scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}
};

inline Point2 operator+(const Point2& a, const Point2& b)
{
	return Point2{ a.X + b.X, a.Y + b.Y };
}

inline Point2 operator-(const Point2& a, const Point2& b)
{
	return Point2{ a.X - b.X, a.Y - b.Y };
}

inline Point2 operator-(const Point2& a)
{
	return Point2{ -a.X, -a.Y };
}

inline Point2 operator*(const Point2& a, int64_t scalar)
{
	return Point2{ a.X * scalar, a.Y * scalar };
}

inline Point2 operator*(int64_t scalar, const Point2& b)
{
	return Point2{ scalar * b.X, scalar * b.Y };
}

template <>
struct std::hash<Point2>
{
	std::size_t operator()(const Point2& v) const noexcept
	{
		size_t hash = std::hash<decltype(v.X)>{}(v.X);
		hash = std::hash<decltype(v.Y)>{}(v.Y) ^ (hash >> 16 | hash << 48);
		return hash;
	}
};

int64_t ManhattanDistance(const Point2& a, const Point2& b);
