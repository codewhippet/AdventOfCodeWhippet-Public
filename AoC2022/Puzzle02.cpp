#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle02_2022_Types
{
}

using namespace Puzzle02_2022_Types;

static void Puzzle02_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

    // A = rock
    // B = paper
    // C = scissors

    // X = rock
    // Y = paper
    // Z = scissors

    map<string, int64_t> scoring =
    {
        // I win
        { "A Y", 6 + 2 },
        { "B Z", 6 + 3 },
        { "C X", 6 + 1 },

        // I lose
        { "A Z", 0 + 3 },
        { "B X", 0 + 1 },
        { "C Y", 0 + 2 },

        // I draw
        { "A X", 3 + 1 },
        { "B Y", 3 + 2 },
        { "C Z", 3 + 3 },
    };

    auto score = ReadEachLine(input) | views::transform([&](const string& line) { return scoring.at(line); });
    int64_t answer = accumulate(score.begin(), score.end(), 0ll);

	printf("[2022] Puzzle02_A: %" PRId64 "\n", answer);
}

static void Puzzle02_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

    map<string, int64_t> scoring =
    {
        // I need to lose
        { "A X", 3 + 0 },
        { "B X", 1 + 0 },
        { "C X", 2 + 0 },

        // I need to draw
        { "A Y", 1 + 3 },
        { "B Y", 2 + 3 },
        { "C Y", 3 + 3 },

        // I need to win
        { "A Z", 2 + 6 },
        { "B Z", 3 + 6 },
        { "C Z", 1 + 6 },
    };

    auto score = ReadEachLine(input) | views::transform([&](const string& line) { return scoring.at(line); });
    int64_t answer = accumulate(score.begin(), score.end(), 0ll);

	printf("[2022] Puzzle02_B: %" PRId64 "\n", answer);
}

void Puzzle02_2022(const string& filename)
{
	Puzzle02_A(filename);
	Puzzle02_B(filename);
}
