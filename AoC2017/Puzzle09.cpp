#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2017_Types
{
}

using namespace Puzzle09_2017_Types;

static const char* IgnoreGarbage(const char* garbage, int64_t* ignored)
{
	assert(garbage[0] == '<');
	garbage++;

	while (garbage[0] != '>')
	{
		switch (garbage[0])
		{
		case '!':
			garbage += 2;
			break;
		default:
			garbage += 1;
			*ignored += 1;
		}
	}

	return garbage + 1;
}

static const char* ScoreGroup(const char* group, int64_t indendationLevel, int64_t* score, int64_t *ignored)
{
	if (group[0] == '\0')
		return nullptr;

	assert(group[0] == '{');
	group++;

	*score += indendationLevel;

	while (group[0] != '}')
	{
		switch (group[0])
		{
		case '{':
			group = ScoreGroup(group, indendationLevel + 1, score, ignored);
			break;
		case '<':
			group = IgnoreGarbage(group, ignored);
			break;
		case ',':
			group++;
			break;
		default:
			assert(false);
		}
	}

	return group + 1;
}

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	//ifstream input(filename);
	istringstream input(dummy);

	string group = ReadSingleLine(input);

	int64_t groupScore = 0;
	int64_t ignoredGarbage = 0;
	ScoreGroup(group.c_str(), 1, &groupScore, &ignoredGarbage);

	int64_t answer = groupScore;

	printf("[2017] Puzzle09_A: %" PRId64 "\n", answer);
}


static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string group = ReadSingleLine(input);

	int64_t groupScore = 0;
	int64_t ignoredGarbage = 0;
	ScoreGroup(group.c_str(), 1, &groupScore, &ignoredGarbage);

	int64_t answer = ignoredGarbage;

	printf("[2017] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2017(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
