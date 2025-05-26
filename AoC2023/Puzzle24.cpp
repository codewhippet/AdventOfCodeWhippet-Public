#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2023_Types
{
	struct Intersection
	{
		double X, Y, Z;
		double P, Q;
	};

	bool operator==(const Intersection& a, const Intersection& b)
	{
		return (a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z);
	}

	bool operator!=(const Intersection& a, const Intersection& b)
	{
		return !(a == b);
	}
}

using namespace Puzzle24_2023_Types;

static pair<Vector3, Vector3> ParseHailstone(const string& line)
{
	Vector3 pos;
	Vector3 vel;
	int scanned = sscanf(line.c_str(), "%lld, %lld, %lld @ %lld, %lld, %lld",
		&pos.X, &pos.Y, &pos.Z,
		&vel.X, &vel.Y, &vel.Z);
	assert(scanned == 6);
	(void)scanned;
	return make_pair(pos, vel);
}

static bool SolveXY(const pair<Vector3, Vector3>& a, const pair<Vector3, Vector3>& b, Intersection* solution)
{
	// Form parametric equations for the lines, with independent parameters p and q
	// For the intersection point use:
	// x0 + p.sx = x1 + q.tx
	// y0 + p.sy = y1 + q.ty
	// Solve simultaneous equations for q to get the intersection

	int64_t x0 = a.first.X;
	int64_t y0 = a.first.Y;
	int64_t sx = a.second.X;
	int64_t sy = a.second.Y;

	int64_t x1 = b.first.X;
	int64_t y1 = b.first.Y;
	int64_t z1 = b.first.Z;
	int64_t tx = b.second.X;
	int64_t ty = b.second.Y;
	int64_t tz = b.second.Z;

	int64_t A = sx * (y1 - y0);
	int64_t B = sy * (x1 - x0);
	int64_t C = sy * tx;
	int64_t D = sx * ty;

	if ((C - D) == 0)
	{
		return false;
	}

	double q = static_cast<double>(A - B) / static_cast<double>(C - D);
	double p = (x1 - x0 + q * tx) / sx;

	// No solutions back in time
	if ((p < 0) || (q < 0))
	{
		return false;
	}

	solution->X = round(x1 + q * tx);
	solution->Y = round(y1 + q * ty);
	solution->Z = round(z1 + q * tz);

	solution->P = p;
	solution->Q = p;

	return true;
}

static bool SolveXZ(const pair<Vector3, Vector3>& a, const pair<Vector3, Vector3>& b, Intersection* solution)
{
	int64_t x0 = a.first.X;
	int64_t z0 = a.first.Z;
	int64_t sx = a.second.X;
	int64_t sz = a.second.Z;

	int64_t x1 = b.first.X;
	int64_t y1 = b.first.Y;
	int64_t z1 = b.first.Z;
	int64_t tx = b.second.X;
	int64_t ty = b.second.Y;
	int64_t tz = b.second.Z;

	int64_t A = sx * (z1 - z0);
	int64_t B = sz * (x1 - x0);
	int64_t C = sz * tx;
	int64_t D = sx * tz;

	if ((C - D) == 0)
	{
		return false;
	}

	double q = static_cast<double>(A - B) / static_cast<double>(C - D);
	double p = (x1 - x0 + q * tx) / sx;

	// No solutions back in time
	if ((p < 0) || (q < 0))
	{
		return false;
	}

	solution->X = round(x1 + q * tx);
	solution->Y = round(y1 + q * ty);
	solution->Z = round(z1 + q * tz);

	solution->P = p;
	solution->Q = p;

	return true;
}

static bool SolveYZ(const pair<Vector3, Vector3>& a, const pair<Vector3, Vector3>& b, Intersection* solution)
{
	int64_t y0 = a.first.Y;
	int64_t z0 = a.first.Z;
	int64_t sy = a.second.Y;
	int64_t sz = a.second.Z;

	int64_t x1 = b.first.X;
	int64_t y1 = b.first.Y;
	int64_t z1 = b.first.Z;
	int64_t tx = b.second.X;
	int64_t ty = b.second.Y;
	int64_t tz = b.second.Z;

	int64_t A = sy * (z1 - z0);
	int64_t B = sz * (y1 - y0);
	int64_t C = sz * ty;
	int64_t D = sy * tz;

	if ((C - D) == 0)
	{
		return false;
	}

	double q = static_cast<double>(A - B) / static_cast<double>(C - D);
	double p = (y1 - y0 + q * ty) / sy;

	// No solutions back in time
	if ((p < 0) || (q < 0))
	{
		return false;
	}

	solution->X = round(x1 + q * tx);
	solution->Y = round(y1 + q * ty);
	solution->Z = round(z1 + q * tz);

	solution->P = p;
	solution->Q = p;

	return true;
}

static bool Solve3D(const pair<Vector3, Vector3>& a, const pair<Vector3, Vector3>& b, Intersection* solution)
{
	bool solved = SolveXY(a, b, solution) || SolveXZ(a, b, solution) || SolveYZ(a, b, solution);
	return solved;
}

static void Puzzle24_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Vector3, Vector3>> hailstones = MakeEnumerator(ReadAllLines(input))
		->Select<pair<Vector3, Vector3>>([](const string& s) { return ParseHailstone(s); })
		->ToVector();

	int64_t answer = 0;
	for (size_t i = 0; i < hailstones.size(); i++)
	{
		for (size_t j = i + 1; j < hailstones.size(); j++)
		{
			Intersection intersection;
			bool solved = SolveXY(hailstones[i], hailstones[j], &intersection);
			if (solved)
			{
				if ((intersection.X >= 200000000000000) && (intersection.X <= 400000000000000) &&
					(intersection.Y >= 200000000000000) && (intersection.Y <= 400000000000000))
				{
					answer++;
				}
			}
		}
	}

	printf("[2023] Puzzle24_A: %" PRId64 "\n", answer);
}

static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Vector3, Vector3>> hailstones = MakeEnumerator(ReadAllLines(input))
		->Select<pair<Vector3, Vector3>>([](const string& s) { return ParseHailstone(s); })
		->ToVector();

	// Make assumptions about the range of velocities
	int64_t minAxis = -250;
	int64_t maxAxis = 250;

	// Try to find a velocity the works for all hailstones
	Vector3 throwVelocity{ 0, 0, 0 };
	for (int64_t x = minAxis; x < maxAxis; x++)
	{
		for (int64_t y = minAxis; y < maxAxis; y++)
		{
			for (int64_t z = minAxis; z < maxAxis; z++)
			{
				pair<Vector3, Vector3> adjustedHailstone0 = hailstones[0];
				pair<Vector3, Vector3> adjustedHailstone1 = hailstones[1];

				adjustedHailstone0.second = adjustedHailstone0.second - Vector3{ x, y, z };
				adjustedHailstone1.second = adjustedHailstone1.second - Vector3{ x, y, z };

				Intersection referenceIntersection;
				if (Solve3D(adjustedHailstone0, adjustedHailstone1, &referenceIntersection))
				{
					bool allMatch = true;

					for (size_t n = 2; n < hailstones.size(); n++)
					{
						pair<Vector3, Vector3> adjustedHailstoneN = hailstones[n];
						adjustedHailstoneN.second = adjustedHailstoneN.second - Vector3{ x, y, z };

						Intersection testIntersection;
						if (Solve3D(adjustedHailstone0, adjustedHailstoneN, &testIntersection) == false)
						{
							allMatch = false;
							break;
						}

						if (testIntersection != referenceIntersection)
						{
							allMatch = false;
							break;
						}
					}

					if (allMatch)
					{
						throwVelocity = Vector3{ x, y, z };
					}
				}
			}
		}
	}

	int64_t answer = 0;

	// Given a velocity, work backwards to find where the rock must have started from
	{
		pair<Vector3, Vector3> adjustedHailstone0 = hailstones[0];
		pair<Vector3, Vector3> adjustedHailstone1 = hailstones[1];

		adjustedHailstone0.second = adjustedHailstone0.second - throwVelocity;
		adjustedHailstone1.second = adjustedHailstone1.second - throwVelocity;

		Intersection referenceIntersection;
		bool solved = Solve3D(adjustedHailstone0, adjustedHailstone1, &referenceIntersection);
		assert(solved);
		(void)solved;

		Vector3 collisionPosition = hailstones[0].first + (int64_t)referenceIntersection.P * hailstones[0].second;
		Vector3 throwPosition = collisionPosition - (int64_t)referenceIntersection.P * throwVelocity;

		answer = throwPosition.X + throwPosition.Y + throwPosition.Z;
	}

	printf("[2023] Puzzle24_B: %" PRId64 "\n", answer);
}

void Puzzle24_2023(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
