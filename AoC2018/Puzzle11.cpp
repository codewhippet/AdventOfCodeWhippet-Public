#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2018_Types
{
}

using namespace Puzzle11_2018_Types;

static vector<vector<int64_t>> GeneratePowerLevels(const size_t gridSize, const int64_t serialNumber)
{
	vector<vector<int64_t>> powerLevels(gridSize + 1);
	ranges::fill(powerLevels, (vector<int64_t>(gridSize + 1)));

	for (const Point2& p : GridRange{ Point2{ 1, 1 }, Point2{ 301, 301 } })
	{
		int64_t rackId = p.X + 10;
		int64_t powerLevel = rackId * p.Y;
		powerLevel += serialNumber;
		powerLevel *= rackId;
		powerLevel = (powerLevel / 100) % 10;
		powerLevel -= 5;

		powerLevels[p.Y][p.X] = powerLevel;
	}

	return powerLevels;
}

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	static int64_t gridSize = 300;

	const int64_t serialNumber = atoll(ReadSingleLine(input).c_str());

	vector<vector<int64_t>> powerLevels = GeneratePowerLevels(gridSize, serialNumber);

	int64_t totalPower = numeric_limits<int64_t>::min();
	Point2 coordinate;
	for (const Point2& p : GridRange{ Point2{ 1, 1 }, Point2{ gridSize - 3, gridSize - 3 } + Point2{ 1, 1 } })
	{
		auto threeByThree = GridRange{ p, p + Point2{ 3, 3 } } | views::transform([&](const Point2& q) { return powerLevels[q.Y][q.X]; });
		int64_t power = accumulate(threeByThree.begin(), threeByThree.end(), 0ll);
		if (power > totalPower)
		{
			totalPower = power;
			coordinate = p;
		}
	}

	Point2 answer = coordinate;

	printf("[2018] Puzzle11_A: %" PRId64 ",%" PRId64 "\n", answer.X, answer.Y);
}


static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t gridSize = 300;

	const int64_t serialNumber = atoll(ReadSingleLine(input).c_str());

	vector<vector<int64_t>> powerLevels = GeneratePowerLevels(gridSize, serialNumber);

	vector<vector<int64_t>> gridSum(gridSize + 1);
	ranges::fill(gridSum, (vector<int64_t>(gridSize + 1)));

	for (const Point2& p : GridRange{ Point2{ 1, 1 }, Point2{ gridSize, gridSize } + Point2{ 1, 1 } })
	{
		Point2 up = p + Point2::Up();
		Point2 left = p + Point2::Left();

		int64_t sum = gridSum[up.Y][up.X] + gridSum[left.Y][left.X] + powerLevels[p.Y][p.X] - gridSum[up.Y][left.X];
		assert(sum <= (gridSize * gridSize * 4));
		gridSum[p.Y][p.X] = sum;
	}

	int64_t totalPower = numeric_limits<int64_t>::min();
	pair<Point2, int64_t> answer;
	for (const Point2& p : GridRange{ Point2{ 1, 1 }, Point2{ gridSize, gridSize } + Point2{ 1, 1 } })
	{
		int64_t largestSquarePossible = min(p.X, p.Y);
		for (int64_t size = 1; size <= largestSquarePossible; size++)
		{
			assert(size >= 1 && size <= 300);

			Point2 up = p + Point2::Up() * size;
			Point2 left = p + Point2::Left() * size;

			int64_t power = gridSum[p.Y][p.X] - gridSum[up.Y][up.X] - gridSum[left.Y][left.X] + gridSum[up.Y][left.X];
			if (power > totalPower)
			{
				totalPower = power;
				answer = { Point2{ left.X, up.Y } + Point2{ 1, 1 }, size };
			}
		}
	}

	printf("[2018] Puzzle11_B: %" PRId64 ",%" PRId64 ",%" PRId64 "\n", answer.first.X, answer.first.Y, answer.second);
}

void Puzzle11_2018(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
