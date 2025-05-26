#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2024_Types
{
	struct Machine
	{
		Point2 A;
		Point2 B;
		Point2 P;
	};
}

using namespace Puzzle13_2024_Types;

vector<Machine> ReadMachines(istream& input)
{
	vector<Machine> ret;

	vector<string> lines = ReadAllLines(input);

	for (size_t i = 0; i + 2 < lines.size(); i += 4)
	{
		Machine m;
		int scanned;
		{
			scanned = sscanf(lines[i + 0].c_str(), "Button A: X+%lld, Y+%lld", &m.A.X, &m.A.Y);
			assert(scanned == 2);

			scanned = sscanf(lines[i + 1].c_str(), "Button B: X+%lld, Y+%lld", &m.B.X, &m.B.Y);
			assert(scanned == 2);

			scanned = sscanf(lines[i + 2].c_str(), "Prize: X=%lld, Y=%lld", &m.P.X, &m.P.Y);
			assert(scanned == 2);
		}
		(void)scanned;

		ret.push_back(m);
	}

	return ret;
}

// 1) Ax.m + Bx.n = Px
// 2) Ay.m + By.n = Py
// 
// 1 * By = 3) (By.Ax).m + (By.Bx).n = By.Px
// 2 * Bx = 4) (Bx.Ay).m + (Bx.By).n = Bx.Py
// 
// 3 - 4 = 5) (By.Ax).m - (Bx.Ay).m = By.Px - Bx.Py
// = > (By.Ax - Bx.Ay).m = (By.Px - Bx.Py)
// = > m = (By.Px - Bx.Py) / (By.Ax - Bx.Ay)
// 
// 1 = > Bx.n = Px - Ax.m
// = > n = (Px - Ax.m) / Bx

int64_t PrizeCost(const Point2& A, const Point2& B, const Point2& P)
{
	int64_t ByPx_BxPy = B.Y * P.X - B.X * P.Y;
	int64_t ByAx_BxAy = B.Y * A.X - B.X * A.Y;

	if (ByAx_BxAy == 0 || ByPx_BxPy % ByAx_BxAy != 0)
	{
		return 0;
	}

	int64_t m = ByPx_BxPy / ByAx_BxAy;
	int64_t Px_Axm = P.X - A.X * m;

	if (B.X == 0 || Px_Axm % B.X != 0)
	{
		return 0;
	}

	int64_t n = Px_Axm / B.X;

	int64_t cost = 3 * m + n;
	return cost;
}

static void Puzzle13_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Machine> machines = ReadMachines(input);
	auto machineCost = machines
		| views::transform([](const auto& m)
			{
				return PrizeCost(m.A, m.B, m.P);
			});

	int64_t answer = accumulate(machineCost.begin(), machineCost.end(), 0ll);

	printf("[2024] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Machine> machines = ReadMachines(input);
	auto machineCost = machines
		| views::transform([](const auto& m)
			{
				return PrizeCost(m.A, m.B, m.P + Point2{ 10000000000000, 10000000000000 });
			});

	int64_t answer = accumulate(machineCost.begin(), machineCost.end(), 0ll);

	printf("[2024] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2024(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
