#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2024_Types
{
	struct Puzzle
	{
		vector<string> Towels;
		vector<string> Targets;
	};
}

using namespace Puzzle19_2024_Types;

static Puzzle Parse(istream& input)
{
	Puzzle p;

	auto lines = ReadAllLines(input);

	ranges::copy(StringSplit(lines[0], ',')
		| views::transform([](const auto& t) { return Trim(t); }),
		back_inserter(p.Towels));

	ranges::copy(lines | views::drop(2), back_inserter(p.Targets));

	return p;
}

static bool CanMakePattern(const string& pattern, const multimap<char, string>& towels, map<string, bool>* cache)
{
	if (pattern.empty())
	{
		return true;
	}

	auto existingAnswer = cache->find(pattern);
	if (existingAnswer != cache->end())
	{
		return existingAnswer->second;
	}

	auto possibleTowels = towels.equal_range(pattern[0]);
	for (auto towelIt = possibleTowels.first; towelIt != possibleTowels.second; ++towelIt)
	{
		const auto& towel = towelIt->second;
		if (pattern.starts_with(towel) && CanMakePattern(pattern.substr(towel.size()), towels, cache))
		{
			cache->insert({ pattern, true });
			return true;
		}
	}

	cache->insert({ pattern, false });
	return false;
}

static int64_t WaysToMakePattern(const string& pattern, const multimap<char, string>& towels, map<string, int64_t>* cache)
{
	if (pattern.empty())
	{
		return 1;
	}

	auto existingAnswer = cache->find(pattern);
	if (existingAnswer != cache->end())
	{
		return existingAnswer->second;
	}

	int64_t waysToMake = 0;

	auto possibleTowels = towels.equal_range(pattern[0]);
	for (auto towelIt = possibleTowels.first; towelIt != possibleTowels.second; ++towelIt)
	{
		const auto& towel = towelIt->second;
		if (pattern.starts_with(towel))
		{
			waysToMake += WaysToMakePattern(pattern.substr(towel.size()), towels, cache);
		}
	}

	cache->insert({ pattern, waysToMake });
	return waysToMake;
}

static void Puzzle19_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = Parse(input);

	multimap<char, string> Towels;
	for (const auto& towel : p.Towels)
	{
		Towels.insert({ towel[0], towel });
	}

	map<string, bool> Cache;

	int64_t answer = 0;
	for (const auto& pattern : p.Targets)
	{
		if (CanMakePattern(pattern, Towels, &Cache))
		{
			answer++;
		}
	}

	printf("[2024] Puzzle19_A: %" PRId64 "\n", answer);
}

static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = Parse(input);

	multimap<char, string> Towels;
	for (const auto& towel : p.Towels)
	{
		Towels.insert({ towel[0], towel });
	}

	map<string, int64_t> Cache;

	int64_t answer = 0;
	for (const auto& pattern : p.Targets)
	{
		answer += WaysToMakePattern(pattern, Towels, &Cache);
	}

	printf("[2024] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2024(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
