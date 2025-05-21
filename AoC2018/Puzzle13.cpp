#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2018_Types
{
	struct Cart
	{
		size_t Id;
		Point2 Dir;
		size_t TurnIndex;
	};

	struct CartSorter
	{
		bool operator()(const Point2& a, const Point2& b) const
		{
			return (a.Y == b.Y) ? a.X < b.X : a.Y < b.Y;
		}
	};

	using CartSet = map<Point2, Cart, CartSorter>;
}

using namespace Puzzle13_2018_Types;

static CartSet ExtractCarts(ArrayMap2D* tracks)
{
	const map<char, Point2> cartDirections =
	{
		{ '^', Point2::Up() },
		{ 'v', Point2::Down() },
		{ '<', Point2::Left() },
		{ '>', Point2::Right() },
	};

	const map<char, char> cartRepairs =
	{
		{ '^', '|' },
		{ 'v', '|' },
		{ '<', '-' },
		{ '>', '-' },
	};

	CartSet carts;
	for (const pair<Point2, char> p : tracks->Grid())
	{
		if (cartDirections.contains(p.second))
		{
			carts.insert({ p.first, { carts.size(), cartDirections.at(p.second), 0 } });
			(*tracks)(p.first) = cartRepairs.at(p.second);
		}
	}
	return carts;
}

static Point2 FindFirstCrashLocation(const ArrayMap2D& tracks, CartSet *carts)
{
	const map<pair<char, Point2>, Point2> cornerTurns =
	{
		{ { '/', Point2::Up() }, Point2::Right() },
		{ { '/', Point2::Down() }, Point2::Left() },
		{ { '/', Point2::Left() }, Point2::Down() },
		{ { '/', Point2::Right() }, Point2::Up() },

		{ { '\\', Point2::Up() }, Point2::Left() },
		{ { '\\', Point2::Down() }, Point2::Right() },
		{ { '\\', Point2::Left() }, Point2::Up() },
		{ { '\\', Point2::Right() }, Point2::Down() },
	};

	const vector<function<Point2(const Point2& d)>> intersectionTurns =
	{
		[](const Point2& d) { return Point2::RotateAnticlockwise(d); },
		[](const Point2& d) { return d; },
		[](const Point2& d) { return Point2::RotateClockwise(d); },
	};

	while (true)
	{
		CartSet newCarts;
		while (carts->empty() == false)
		{
			CartSet::value_type cart = *carts->begin();
			carts->erase(carts->begin());

			Point2 newCartPos = cart.first + cart.second.Dir;
			if (newCarts.contains(newCartPos) || carts->contains(newCartPos))
			{
				return newCartPos;
			}

			char trackPiece = tracks(newCartPos);
			switch (trackPiece)
			{
			case '/':
			case '\\':
				cart.second.Dir = cornerTurns.at({ trackPiece, cart.second.Dir });
				break;

			case '+':
				cart.second.Dir = intersectionTurns.at(cart.second.TurnIndex)(cart.second.Dir);
				cart.second.TurnIndex = (cart.second.TurnIndex + 1) % intersectionTurns.size();
				break;
			};

			newCarts.insert({ newCartPos, cart.second });
		}

		carts->swap(newCarts);
	}

	return Point2{ -1, -1 };
}

static Point2 FindLastCartLocation(const ArrayMap2D& tracks, CartSet* carts)
{
	const map<pair<char, Point2>, Point2> cornerTurns =
	{
		{ { '/', Point2::Up() }, Point2::Right() },
		{ { '/', Point2::Down() }, Point2::Left() },
		{ { '/', Point2::Left() }, Point2::Down() },
		{ { '/', Point2::Right() }, Point2::Up() },

		{ { '\\', Point2::Up() }, Point2::Left() },
		{ { '\\', Point2::Down() }, Point2::Right() },
		{ { '\\', Point2::Left() }, Point2::Up() },
		{ { '\\', Point2::Right() }, Point2::Down() },
	};

	const vector<function<Point2(const Point2& d)>> intersectionTurns =
	{
		[](const Point2& d) { return Point2::RotateAnticlockwise(d); },
		[](const Point2& d) { return d; },
		[](const Point2& d) { return Point2::RotateClockwise(d); },
	};

	while (carts->size() != 1)
	{
		CartSet newCarts;
		while (carts->empty() == false)
		{
			CartSet::value_type cart = *carts->begin();
			carts->erase(carts->begin());

			Point2 newCartPos = cart.first + cart.second.Dir;
			if (newCarts.contains(newCartPos))
			{
				newCarts.erase(newCartPos);
				continue;
			}
			if (carts->contains(newCartPos))
			{
				carts->erase(newCartPos);
				continue;
			}

			char trackPiece = tracks(newCartPos);
			switch (trackPiece)
			{
			case '/':
			case '\\':
				cart.second.Dir = cornerTurns.at({ trackPiece, cart.second.Dir });
				break;

			case '+':
				cart.second.Dir = intersectionTurns.at(cart.second.TurnIndex)(cart.second.Dir);
				cart.second.TurnIndex = (cart.second.TurnIndex + 1) % intersectionTurns.size();
				break;
			};

			newCarts.insert({ newCartPos, cart.second });
		}

		if (newCarts.size() == 1)
		{
			return newCarts.begin()->first;
		}

		carts->swap(newCarts);
	}

	return Point2{ -1, -1 };
}

static void Puzzle13_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D tracks = ReadArrayMap(input, ' ');
	CartSet carts = ExtractCarts(&tracks);

	Point2 answer = FindFirstCrashLocation(tracks, &carts);

	printf("[2018] Puzzle13_A: %" PRId64 ",%" PRId64 "\n", answer.X, answer.Y);
}


static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D tracks = ReadArrayMap(input, ' ');
	CartSet carts = ExtractCarts(&tracks);

	Point2 answer = FindLastCartLocation(tracks, &carts);

	printf("[2018] Puzzle13_B: %" PRId64 ",%" PRId64 "\n", answer.X, answer.Y);
}

void Puzzle13_2018(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
