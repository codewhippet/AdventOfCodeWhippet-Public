#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2020_Types
{
}

using namespace Puzzle15_2020_Types;

static pair<int64_t, int64_t> ShiftHistory(int64_t newTurn, const pair<int64_t, int64_t>& history)
{
	return { newTurn, history.first };
}

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> startingNumbers = ReadAsVectorOfNumbers(ReadSingleLine(input));

	map<int64_t, pair<int64_t, int64_t>> spokenHistory;
	for (size_t i = 0; i < startingNumbers.size(); i++)
	{
		spokenHistory[startingNumbers[i]] = { i + 1, 0 };
	}

	int64_t lastSpokenNumber = startingNumbers.back();
	for (int64_t i = startingNumbers.size() + 1; i <= 2020; i++)
	{
		map<int64_t, pair<int64_t, int64_t>>::iterator lastSpokenIt = spokenHistory.find(lastSpokenNumber);
		assert(lastSpokenIt != spokenHistory.end());

		int64_t shouldSpeak;

		bool wasNewNumber = lastSpokenIt->second.second == 0;
		if (wasNewNumber)
		{
			// Speak 0 for a new number
			shouldSpeak = 0;
		}
		else
		{
			// Speak the diff
			shouldSpeak = lastSpokenIt->second.first - lastSpokenIt->second.second;
			assert(shouldSpeak > 0);
		}

		spokenHistory[shouldSpeak] = ShiftHistory(i, spokenHistory[shouldSpeak]);
		lastSpokenNumber = shouldSpeak;
	}

	int64_t answer = lastSpokenNumber;

	printf("[2020] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> startingNumbers = ReadAsVectorOfNumbers(ReadSingleLine(input));

	unordered_map<int64_t, pair<int64_t, int64_t>> spokenHistory;
	for (size_t i = 0; i < startingNumbers.size(); i++)
	{
		spokenHistory[startingNumbers[i]] = { i + 1, 0 };
	}

	int64_t lastSpokenNumber = startingNumbers.back();
	for (int64_t i = startingNumbers.size() + 1; i <= 30000000; i++)
	{
		unordered_map<int64_t, pair<int64_t, int64_t>>::iterator lastSpokenIt = spokenHistory.find(lastSpokenNumber);
		assert(lastSpokenIt != spokenHistory.end());

		int64_t shouldSpeak;

		bool wasNewNumber = lastSpokenIt->second.second == 0;
		if (wasNewNumber)
		{
			// Speak 0 for a new number
			shouldSpeak = 0;
		}
		else
		{
			// Speak the diff
			shouldSpeak = lastSpokenIt->second.first - lastSpokenIt->second.second;
			assert(shouldSpeak > 0);
		}

		spokenHistory[shouldSpeak] = ShiftHistory(i, spokenHistory[shouldSpeak]);
		lastSpokenNumber = shouldSpeak;
	}

	int64_t answer = lastSpokenNumber;

	printf("[2020] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2020(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
