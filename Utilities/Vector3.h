#pragma once

#include <stdint.h>

struct Vector3
{
	int64_t X = 0;
	int64_t Y = 0;
	int64_t Z = 0;

	auto operator<=>(const Vector3&) const = default;
	bool operator==(const Vector3&) const = default;

	static const std::vector<Vector3>& CardinalDirections();

	static Vector3 MinElements(const Vector3& a, const Vector3& b);
	static Vector3 MaxElements(const Vector3& a, const Vector3& b);
};

inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3{ a.X + b.X, a.Y + b.Y, a.Z + b. Z };
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3{ a.X - b.X, a.Y - b.Y, a.Z - b.Z };
}

inline Vector3 operator*(const Vector3& a, int64_t b)
{
	return Vector3{ a.X * b, a.Y * b, a.Z * b };
}

inline Vector3 operator*(int64_t a, const Vector3& b)
{
	return Vector3{ a * b.X, a * b.Y, a * b.Z };
}

inline Vector3 operator-(const Vector3& a)
{
	return Vector3{ -a.X, -a.Y, -a.Z };
}

inline int64_t Dot(const Vector3& a, const Vector3& b)
{
	return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
}

inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return Vector3{
		(a.Y * b.Z) - (a.Z * b.Y),
		(a.Z * b.X) - (a.X * b.Z),
		(a.X * b.Y) - (a.Y * b.X)
		};
}

inline Vector3 Mul(const Vector3& a, const Vector3& b)
{
	return Vector3{ a.X * b.X, a.Y * b.Y, a.Z * b.Z };
}

int64_t ManhattanDistance(const Vector3& a, const Vector3& b);
