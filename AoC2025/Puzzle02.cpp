#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2025_Types
{
}

using namespace Puzzle02_2025_Types;

static int64_t NumDigits(int64_t n)
{
	if (n == 0)
		return 1;

	int64_t ret = 0;
	while (n > 0)
	{
		n /= 10;
		ret++;
	}
	return ret;
}

static int64_t Magnitude(int64_t numDigits)
{
	int64_t ret = 1;
	for (int64_t i = 1; i < numDigits; i++)
	{
		ret *= 10;
	}
	return ret;
}

static bool IsInRanges(int64_t candidate, const map<int64_t, int64_t>& ranges)
{
	const auto rangeBeyondCandidate = ranges.upper_bound(candidate);
	if (rangeBeyondCandidate == ranges.begin())
		return false;

	const auto possibleRange = prev(rangeBeyondCandidate);
	return (candidate >= possibleRange->first && candidate <= possibleRange->second);
}

static int64_t RepeatPattern(int64_t pattern, int64_t patternMagnitude, int64_t repeatCount)
{
	int64_t ret = 0;
	for (int64_t i = 0; i < repeatCount; i++)
	{
		ret += pattern;
		pattern *= patternMagnitude;
	}
	return ret;
}

static void Puzzle02_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string rangesInput;
	getline(input, rangesInput);

	map<int64_t, int64_t> ranges = Enumerable::Regex(rangesInput, regex{ R"((\d+)-(\d+))" })
		->Select<pair<int64_t, int64_t>>([](const smatch& m) { return make_pair(stoll(m[1].str()), stoll(m[2].str())); })
		->ToMap<int64_t, int64_t>();

	const int64_t largestNumber = ranges::max(ranges | views::values);
	const int64_t mostDigits = NumDigits(largestNumber);
	const int64_t largestPattern = (mostDigits + 1) / 2;

	set<int64_t> invalidIds;
	for (int64_t patternDigits = 1; patternDigits <= largestPattern; patternDigits++)
	{
		const int64_t patternBegin = Magnitude(patternDigits);
		const int64_t patternEnd = Magnitude(patternDigits + 1);

		for (int64_t pattern = patternBegin; pattern < patternEnd; pattern++)
		{
			const int64_t numPatterns = 2;
			if ((patternDigits * numPatterns) > mostDigits)
				break;

			int64_t candidate = RepeatPattern(pattern, patternEnd, numPatterns);
			if (IsInRanges(candidate, ranges))
			{
				invalidIds.insert(candidate);
			}
		}
	}

	int64_t answer = ranges::fold_left(invalidIds, 0, plus{});

	printf("[2025] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string rangesInput;
	getline(input, rangesInput);

	map<int64_t, int64_t> ranges = Enumerable::Regex(rangesInput, regex{ R"((\d+)-(\d+))" })
		->Select<pair<int64_t, int64_t>>([](const smatch& m) { return make_pair(stoll(m[1].str()), stoll(m[2].str())); })
		->ToMap<int64_t, int64_t>();

	const int64_t largestNumber = ranges::max(ranges | views::values);
	const int64_t mostDigits = NumDigits(largestNumber);
	const int64_t largestPattern = (mostDigits + 1) / 2;

	set<int64_t> invalidIds;
	for (int64_t patternDigits = 1; patternDigits <= largestPattern; patternDigits++)
	{
		const int64_t patternBegin = Magnitude(patternDigits);
		const int64_t patternEnd = Magnitude(patternDigits + 1);

		for (int64_t pattern = patternBegin; pattern < patternEnd; pattern++)
		{
			for (int64_t numPatterns = 2; numPatterns < mostDigits; numPatterns++)
			{
				if ((patternDigits * numPatterns) > mostDigits)
					break;

				int64_t candidate = RepeatPattern(pattern, patternEnd, numPatterns);
				if (IsInRanges(candidate, ranges))
				{
					invalidIds.insert(candidate);
				}
			}
		}
	}

	int64_t answer = ranges::fold_left(invalidIds, 0, plus{});

	printf("[2025] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2025(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
