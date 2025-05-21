#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2024_Types
{
}

using namespace Puzzle11_2024_Types;

static bool TrySplitStone(int64_t stone, int64_t* hi, int64_t* lo)
{
	char buffer[256];
	string stoneDigits = _i64toa(stone, buffer, 10);
	if (stoneDigits.size() % 2 == 0)
	{
		*hi = stoll(stoneDigits.substr(0, stoneDigits.size() / 2).c_str());
		*lo = stoll(stoneDigits.substr(stoneDigits.size() / 2).c_str());
		return true;
	}
	return false;
}

static int64_t CountStone(int64_t stone, int64_t blink, map<pair<int64_t, int64_t>, int64_t>* cache)
{
	auto cachedSolution = cache->find({ stone, blink });
	if (cachedSolution != cache->end())
	{
		return cachedSolution->second;
	}

	int64_t ret;
	int64_t hi, lo;

	if (blink == 0)
	{
		ret = 1;
	}
	else if (stone == 0)
	{
		ret = CountStone(1, blink - 1, cache);
	}
	else if (TrySplitStone(stone, &hi, &lo))
	{
		ret = CountStone(hi, blink - 1, cache) + CountStone(lo, blink - 1, cache);
	}
	else
	{
		ret = CountStone(stone * 2024, blink - 1, cache);
	}

	cache->insert({ { stone, blink }, ret });
	return ret;
}

static int64_t CountStones(const vector<int64_t> stones, int64_t blinkCount)
{
	map<pair<int64_t, int64_t>, int64_t> cache;
	auto blinkStones = stones | views::transform([&](int64_t stone) { return CountStone(stone, blinkCount, &cache); });
	int64_t stoneCount = accumulate(blinkStones.begin(), blinkStones.end(), 0ll);
	return stoneCount;
}

static void Puzzle11_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> stones = ReadAsVectorOfNumbers(ReadAllLines(input).front());

	int64_t answer = CountStones(stones, 25);

	printf("[2024] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> stones = ReadAsVectorOfNumbers(ReadAllLines(input).front());

	int64_t answer = CountStones(stones, 75);

	printf("[2024] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2024(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
