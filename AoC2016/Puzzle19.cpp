#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2016_Types
{
	struct Elf
	{
		Elf* Next = nullptr;
		Elf* Prev = nullptr;
	};
}

using namespace Puzzle19_2016_Types;

static void RemoveElf(Elf* elf)
{
	elf->Next->Prev = elf->Prev;
	elf->Prev->Next = elf->Next;
	elf->Next = nullptr;
	elf->Prev = nullptr;
}

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string puzzleInput;
	getline(input, puzzleInput);

	const size_t elfCount = atoll(puzzleInput.c_str());

	vector<size_t> elves(elfCount);
	for (size_t i = 0; i < elves.size(); i++)
	{
		elves[i] = (i + 1) % elves.size();
	}

	size_t currentElf = 0;
	while (elves[currentElf] != elves[elves[currentElf]])
	{
		elves[currentElf] = elves[elves[currentElf]];
		currentElf = elves[currentElf];
	}

	int64_t answer = currentElf + 1;

	printf("[2016] Puzzle19_A: %" PRId64 "\n", answer);
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string puzzleInput;
	getline(input, puzzleInput);

	const size_t startingElfCount = atoll(puzzleInput.c_str());

	vector<Elf> elves(startingElfCount);
	for (size_t i = 0; i < elves.size(); i++)
	{
		elves[i].Next = &elves[(i + 1) % elves.size()];
		elves[i].Prev = &elves[(i + elves.size() - 1) % elves.size()];
	}

	Elf* currentElf = &elves[0];
	Elf* stealTarget = &elves[elves.size() / 2];

	for (size_t elfCount = startingElfCount; elfCount > 1; elfCount--)
	{
		stealTarget = stealTarget->Next;
		RemoveElf(stealTarget->Prev);
		if (elfCount % 2)
		{
			stealTarget = stealTarget->Next;
		}
		currentElf = currentElf->Next;
	}

	int64_t answer = distance(&elves[0], currentElf) + 1;

	printf("[2016] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2016(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
