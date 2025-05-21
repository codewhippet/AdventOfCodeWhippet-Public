#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2016_Types
{
}

using namespace Puzzle04_2016_Types;

static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const auto& match : ReadEachLine(input, regex{ R"(([[:alpha:]\-]+)(\d+)\[(\w+)\])" }))
	{
		map<char, int64_t> characterCounts;
		for (char c : match[1].str())
		{
			characterCounts[c]++;
		}

		set<pair<int64_t, char>> commonLetters;
		ranges::copy(characterCounts
			| views::filter([](const auto& kvp)
				{
					return kvp.first != '-';
				})
			| views::transform([](const auto& kvp)
				{
					return make_pair(-kvp.second, kvp.first);
				}),
			inserter(commonLetters, commonLetters.end()));

		auto letterCountsToChars = views::common(commonLetters | views::take(5) | views::transform([](const pair<int64_t, char>& kvp) { return kvp.second; }));
		string mostCommonLetters = accumulate(letterCountsToChars.begin(), letterCountsToChars.end(), string{});
		if (match[3].str() == mostCommonLetters)
		{
			answer += stoll(match[2].str());
		}
	}

	printf("[2016] Puzzle04_A: %" PRId64 "\n", answer);
}


static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const auto& match : ReadEachLine(input, regex{ R"(([[:alpha:]\-]+)(\d+).*)" }))
	{
		string roomName = match[1].str();
		int64_t sectorId = stoll(match[2].str());
		for (char& c : roomName)
		{
			if (c != '-')
			{
				int64_t code = c - 'a';
				code = (code + sectorId) % 26;
				c = 'a' + (char)code;
			}
		}

		if (roomName.find("northpole") != string::npos)
		{
			answer = sectorId;
			break;
		}
	}

	printf("[2016] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2016(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
