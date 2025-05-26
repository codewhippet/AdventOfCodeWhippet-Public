#pragma once
#include <functional>
#include <vector>
#include <stdint.h>

// With reference to https://www.redblobgames.com/grids/hexagons/
// Cube co-ordinate system for both Flat and Pointy varieties

namespace HexFlat
{
	struct Hex
	{
		int64_t Q = 0;
		int64_t R = 0;
		int64_t S = 0;

		static constexpr Hex North() { return Hex{ 0, -1, 1 }; }
		static constexpr Hex NorthEast() { return Hex{ 1, -1, 0 }; }
		static constexpr Hex SouthEast() { return Hex{ 1, 0, -1 }; }
		static constexpr Hex South() { return Hex{ 0, 1, -1 }; }
		static constexpr Hex SouthWest() { return Hex{ -1, 1, 0 }; }
		static constexpr Hex NorthWest() { return Hex{ -1, 0, 1 }; }

		static const std::vector<Hex>& NeighbourDirections();

		auto operator<=>(const Hex&) const = default;
	};

	inline Hex operator+(const Hex& a, const Hex& b)
	{
		return Hex{ a.Q + b.Q, a.R + b.R, a.S + b.S };
	}

	inline Hex operator-(const Hex& a, const Hex& b)
	{
		return Hex{ a.Q - b.Q, a.R - b.R, a.S - b.S };
	}

	inline Hex operator-(const Hex& a)
	{
		return Hex{ -a.Q, -a.R, -a.S };
	}

	inline int64_t MahattanDistance(const Hex& a, const Hex& b)
	{
		Hex h = a - b;
		return (abs(h.Q) + abs(h.R) + abs(h.S)) / 2;
	}
}

namespace HexPointy
{
	struct Hex
	{
		int64_t Q = 0;
		int64_t R = 0;
		int64_t S = 0;

		static constexpr Hex NorthEast() { return Hex{ 1, -1, 0 }; }
		static constexpr Hex East() { return Hex{ 1, 0, -1 }; }
		static constexpr Hex SouthEast() { return Hex{ 0, 1, -1 }; }
		static constexpr Hex SouthWest() { return Hex{ -1, 1, 0 }; }
		static constexpr Hex West() { return Hex{ -1, 0, 1 }; }
		static constexpr Hex NorthWest() { return Hex{ 0, -1, 1 }; }

		static const std::vector<Hex>& NeighbourDirections();

		auto operator<=>(const Hex&) const = default;
	};

	inline Hex operator+(const Hex& a, const Hex& b)
	{
		return Hex{ a.Q + b.Q, a.R + b.R, a.S + b.S };
	}

	inline Hex operator-(const Hex& a, const Hex& b)
	{
		return Hex{ a.Q - b.Q, a.R - b.R, a.S - b.S };
	}

	inline Hex operator-(const Hex& a)
	{
		return Hex{ -a.Q, -a.R, -a.S };
	}

	inline int64_t MahattanDistance(const Hex& a, const Hex& b)
	{
		Hex h = a - b;
		return (abs(h.Q) + abs(h.R) + abs(h.S)) / 2;
	}
}

template <>
struct std::hash<HexFlat::Hex>
{
	std::size_t operator()(const HexFlat::Hex& h) const noexcept
	{
		size_t hash = std::hash<decltype(h.Q)>{}(h.Q);
		hash = std::hash<decltype(h.R)>{}(h.R) ^ (hash >> 16 | hash << 48);
		hash = std::hash<decltype(h.S)>{}(h.S) ^ (hash >> 16 | hash << 48);
		return hash;
	}
};
