#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2019_Types
{
}

using namespace Puzzle04_2019_Types;

static bool IsValidPasswordPart1(const string& s)
{
	bool hasDouble = false;
	bool decreases = false;
	for (size_t i = 0; (i + 1) < s.size(); i++)
	{
		if (s[i] == s[i + 1])
			hasDouble = true;
		if (s[i] > s[i + 1])
			decreases = true;
	}
	return hasDouble && !decreases;
}

static bool IsValidPasswordPart2(const string& s)
{
	vector<char> diffs(s.size() - 1);
	diffs.push_back(100);
	for (size_t i = 0; (i + 1) < s.size(); i++)
	{
		diffs.push_back(s[i + 1] - s[i]);
	}
	diffs.push_back(100);

	bool hasSingletonZero = false;
	for (size_t i = 1; (i + 1) < diffs.size(); i++)
	{
		if ((diffs[i] == 0) &&
			(diffs[i - 1] != 0) &&
			(diffs[i + 1] != 0))
		{
			hasSingletonZero = true;
		}
	}

	return hasSingletonZero && ranges::none_of(diffs, [](char c) { return c < 0; });
}

static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t start, stop;
	int scanned = sscanf(ReadSingleLine(input).c_str(), "%lld-%lld", &start, &stop);
	assert(scanned == 2);
	(void)scanned;

	int64_t answer = (int64_t)ranges::distance(views::iota(start, stop + 1)
		| views::transform([](int64_t i) { return to_string(i); })
		| views::filter(IsValidPasswordPart1));

	printf("[2019] Puzzle04_A: %" PRId64 "\n", answer);
}


static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t start, stop;
	int scanned = sscanf(ReadSingleLine(input).c_str(), "%lld-%lld", &start, &stop);
	assert(scanned == 2);
	(void)scanned;

	int64_t answer = (int64_t)ranges::distance(views::iota(start, stop + 1)
		| views::transform([](int64_t i) { return to_string(i); })
		| views::filter(IsValidPasswordPart2));

	printf("[2019] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2019(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
