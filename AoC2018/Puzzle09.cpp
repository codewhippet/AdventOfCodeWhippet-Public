#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2018_Types
{
	struct Marble
	{
		int64_t Value = -1;
		Marble* Next = nullptr;
		Marble* Prev = nullptr;

		void Remove()
		{
			Marble* currentNext = Next;
			Marble* currentPrev = Prev;

			Next->Prev = currentPrev;
			Prev->Next = currentNext;

			Next = nullptr;
			Prev = nullptr;
		}

		void InsertMarbleAfterThis(Marble* m)
		{
			m->Next = Next;
			m->Prev = this;

			Next->Prev = m;
			Next = m;
		}
	};
}

using namespace Puzzle09_2018_Types;

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	size_t numPlayers = 0;
	size_t lastMarble = 0;
	int scanned = sscanf(ReadSingleLine(input).c_str(), "%llu players; last marble is worth %llu points", &numPlayers, &lastMarble);
	assert(scanned == 2);
	(void)scanned;

	vector<Marble> marbles(lastMarble + 1);
	for (size_t i : views::iota(0ull, marbles.size()))
	{
		marbles[i].Value = i;
	}

	Marble* current = &marbles[0];
	current->Next = current;
	current->Prev = current;

	vector<int64_t> playerScores(numPlayers);
	for (size_t i = 1; i < marbles.size(); i++)
	{
		Marble* nextMarble = &marbles[i];
		int64_t& currentPlayerScore = playerScores[(i - 1) % playerScores.size()];

		if ((i % 23) == 0)
		{
			currentPlayerScore += nextMarble->Value;

			Marble* marbleToRemove = current;
			for (int j = 0; j < 7; j++)
			{
				marbleToRemove = marbleToRemove->Prev;
			}

			current = marbleToRemove->Next;
			marbleToRemove->Remove();

			currentPlayerScore += marbleToRemove->Value;
		}
		else
		{
			current->Next->InsertMarbleAfterThis(nextMarble);
			current = nextMarble;
		}
	}

	int64_t answer = ranges::max(playerScores);

	printf("[2018] Puzzle09_A: %" PRId64 "\n", answer);
}


static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	size_t numPlayers = 0;
	size_t lastMarble = 0;
	int scanned = sscanf(ReadSingleLine(input).c_str(), "%llu players; last marble is worth %llu points", &numPlayers, &lastMarble);
	assert(scanned == 2);
	(void)scanned;

	lastMarble *= 100;

	vector<Marble> marbles(lastMarble + 1);
	for (size_t i : views::iota(0ull, marbles.size()))
	{
		marbles[i].Value = i;
	}

	Marble* current = &marbles[0];
	current->Next = current;
	current->Prev = current;

	vector<int64_t> playerScores(numPlayers);
	for (size_t i = 1; i < marbles.size(); i++)
	{
		Marble* nextMarble = &marbles[i];
		int64_t& currentPlayerScore = playerScores[(i - 1) % playerScores.size()];

		if ((i % 23) == 0)
		{
			currentPlayerScore += nextMarble->Value;

			Marble* marbleToRemove = current;
			for (int j = 0; j < 7; j++)
			{
				marbleToRemove = marbleToRemove->Prev;
			}

			current = marbleToRemove->Next;
			marbleToRemove->Remove();

			currentPlayerScore += marbleToRemove->Value;
		}
		else
		{
			current->Next->InsertMarbleAfterThis(nextMarble);
			current = nextMarble;
		}
	}

	int64_t answer = ranges::max(playerScores);

	printf("[2018] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2018(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
