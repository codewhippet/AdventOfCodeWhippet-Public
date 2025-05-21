#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2015_Types
{
}

using namespace Puzzle05_2015_Types;

static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const regex vowels(R"(a|e|i|o|u)");
	const regex doubleLetter(R"(([a-z])\1)");
	const regex containsForbidden(R"(ab|cd|pq|xy)");

	int64_t answer = ranges::count_if(ReadAllLines(input),
		[&](const string& s)
		{
			size_t numVowels = distance(sregex_iterator{ s.begin(), s.end(), vowels }, sregex_iterator{});
			bool hasDoubleLetter = regex_search(s.begin(), s.end(), doubleLetter);
			bool hasForbidden = regex_search(s.begin(), s.end(), containsForbidden);

			return (numVowels >= 3) && hasDoubleLetter && !hasForbidden;
		});

	printf("[2015] Puzzle05_A: %" PRId64 "\n", answer);
}


static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const regex doubleDouble(R"(([a-z]{2}).*\1)");
	const regex repeatedLetter(R"(([a-z]).\1)");

	int64_t answer = ranges::count_if(ReadAllLines(input),
		[&](const string& s)
		{
			bool hasDoubleLetter = regex_search(s.begin(), s.end(), doubleDouble);
			bool hasRepeated = regex_search(s.begin(), s.end(), repeatedLetter);

			return hasDoubleLetter && hasRepeated;
		});

	printf("[2015] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2015(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
