#pragma once

#include <functional>
#include <stdint.h>

struct Vector4
{
	int64_t X = 0;
	int64_t Y = 0;
	int64_t Z = 0;
	int64_t W = 0;
};

inline bool operator==(const Vector4& a, const Vector4& b)
{
	return (a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z) && (a.W == b.W);
}

inline bool operator!=(const Vector4& a, const Vector4& b)
{
	return !(a == b);
}

inline bool operator<(const Vector4& a, const Vector4& b)
{
	if (a.X != b.X) return a.X < b.X;
	if (a.Y != b.Y) return a.Y < b.Y;
	if (a.Z != b.Z) return a.Z < b.Z;
	return a.W < b.W;
}

inline Vector4 operator+(const Vector4& a, const Vector4& b)
{
	return Vector4{ a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W };
}

inline Vector4 operator-(const Vector4& a, const Vector4& b)
{
	return Vector4{ a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W };
}

inline Vector4 operator*(const Vector4& a, int64_t b)
{
	return Vector4{ a.X * b, a.Y * b, a.Z * b, a.W * b };
}

inline Vector4 operator*(int64_t a, const Vector4& b)
{
	return Vector4{ a * b.X, a * b.Y, a * b.Z, a * b.W };
}

inline Vector4 operator-(const Vector4& a)
{
	return Vector4{ -a.X, -a.Y, -a.Z, -a.W };
}

inline int64_t Dot(const Vector4& a, const Vector4& b)
{
	return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z) + (a.W * b.W);
}

template <>
struct std::hash<Vector4>
{
	std::size_t operator()(const Vector4& v) const noexcept
	{
		size_t hash = std::hash<decltype(v.X)>{}(v.X);
		hash = std::hash<decltype(v.Y)>{}(v.Y) ^ (hash >> 16 | hash << 48);
		hash = std::hash<decltype(v.Z)>{}(v.Z) ^ (hash >> 16 | hash << 48);
		hash = std::hash<decltype(v.W)>{}(v.W) ^ (hash >> 16 | hash << 48);
		return hash;
	}
};
