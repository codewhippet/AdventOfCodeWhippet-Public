#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2025_Types
{
}

using namespace Puzzle03_2025_Types;

static int64_t Magnitude(int64_t power)
{
	int64_t ret = 1;
	while (--power)
		ret *= 10;
	return ret;
}

static int64_t MaximumJolt(const string& batteries, int64_t numDigits)
{
	vector<int64_t> jolts = batteries
		| views::transform([](const char c) { return static_cast<int64_t>(c - '0'); })
		| ranges::to<vector>();
	jolts.push_back(0);

	vector<vector<int64_t>> dp(numDigits + 1, vector<int64_t>(batteries.size() + 1, 0));
	for (int64_t digits = 1; digits <= numDigits; digits++)
	{
		for (int64_t battery = batteries.size() - digits; battery >= 0; battery--)
		{
			dp[digits][battery] = max(dp[digits][battery + 1], (Magnitude(digits) * jolts[battery]) + dp[digits - 1][battery + 1]);
		}
	}

	return dp[numDigits][0];
}

static void Puzzle03_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	string line;
	while (getline(input, line))
	{
		answer += MaximumJolt(line, 2);
	}

	printf("[2025] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	string line;
	while (getline(input, line))
	{
		answer += MaximumJolt(line, 12);
	}

	printf("[2025] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2025(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
