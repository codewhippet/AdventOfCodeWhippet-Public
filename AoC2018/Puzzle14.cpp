#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2018_Types
{
}

using namespace Puzzle14_2018_Types;

static bool EndsWith(const vector<int8_t> &scores, const vector<int8_t> &lookingFor)
{
	if (scores.size() < lookingFor.size())
		return false;

	vector<int8_t>::const_reverse_iterator scoreIt = scores.crbegin();
	vector<int8_t>::const_reverse_iterator lookingForIt = lookingFor.crbegin();
	while (lookingForIt != lookingFor.crend())
	{
		if (*scoreIt++ != *lookingForIt++)
		{
			return false;
		}
	}

	return true;
}

static int64_t FindFirstAppearanceOf(const vector<int8_t>& lookingFor)
{
	vector<int8_t> scores;
	scores.reserve(25000000);

	scores.push_back(3);
	scores.push_back(7);

	size_t elf1 = 0;
	size_t elf2 = 1;

	while (true)
	{
		int8_t recipeSum = scores[elf1] + scores[elf2];

		if (recipeSum > 9)
		{
			scores.push_back(1);
			if (EndsWith(scores, lookingFor))
			{
				return scores.size() - lookingFor.size();
			}
		}

		scores.push_back(recipeSum % 10);
		if (EndsWith(scores, lookingFor))
		{
			return scores.size() - lookingFor.size();
		}

		elf1 = (elf1 + 1 + scores[elf1]) % scores.size();
		elf2 = (elf2 + 1 + scores[elf2]) % scores.size();
	}

	return -1;
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const uint64_t improveAfter = strtoull(ReadSingleLine(input).c_str(), nullptr, 10);

	vector<int8_t> scores;
	scores.reserve(1000000);

	scores.push_back(3);
	scores.push_back(7);

	size_t elf1 = 0;
	size_t elf2 = 1;

	while (scores.size() < improveAfter + 10)
	{
		int8_t recipeSum = scores[elf1] + scores[elf2];
		if (recipeSum > 9)
		{
			scores.push_back(1);
		}
		scores.push_back(recipeSum % 10);

		elf1 = (elf1 + 1 + scores[elf1]) % scores.size();
		elf2 = (elf2 + 1 + scores[elf2]) % scores.size();
	}

	int64_t answer = 0;
	for (size_t i = 0; i < 10; i++)
	{
		answer *= 10;
		answer += scores[scores.size() - 10 + i];
	}

	printf("[2018] Puzzle14_A: %" PRId64 "\n", answer);
}


static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string lookingForAsString = ReadSingleLine(input).c_str();

	vector<int8_t> lookingFor;
	ranges::copy(lookingForAsString | views::transform([](char c) { return (int8_t)(c - '0'); }), back_inserter(lookingFor));

	int64_t answer = FindFirstAppearanceOf(lookingFor);

	printf("[2018] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2018(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
