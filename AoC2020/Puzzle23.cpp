#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2020_Types
{
	struct Cup
	{
		size_t Id = 0;
		Cup* Next = nullptr;
		Cup* Prev = nullptr;
	};
}

using namespace Puzzle23_2020_Types;

static vector<Cup *> MakeCupCircle(const vector<size_t>& order)
{
	vector<Cup*> cups;

	auto getOrder = [&order](size_t i)
	{
		return (i < order.size() ? order[i] : order.size() + i);
	};

	for (size_t i = 0; i < order.size(); i++)
	{	
		cups.push_back(new Cup{ i + 1 });
	}

	for (size_t i = 0; i < order.size(); i++)
	{
		size_t currentCup = order[i];
		size_t nextCup = order[(i + 1) % order.size()];
		size_t prevCup = order[(i + order.size() - 1) % order.size()];

		cups[currentCup - 1]->Next = cups[nextCup - 1];
		cups[currentCup - 1]->Prev = cups[prevCup - 1];
	}

	return cups;
}

static vector<Cup*> MakeLargeCupCircle(const vector<size_t>& order, size_t cupCount)
{
	vector<Cup*> cups;

	auto getOrder = [&order](size_t i)
	{
		return (i < order.size() ? order[i] : i + 1);
	};

	for (size_t i = 0; i < cupCount; i++)
	{
		cups.push_back(new Cup{ i + 1 });
	}

	for (size_t i = 0; i < cupCount; i++)
	{
		size_t currentCup = getOrder(i);
		size_t nextCup = getOrder((i + 1) % cupCount);
		size_t prevCup = getOrder((i + cupCount - 1) % cupCount);

		cups[currentCup - 1]->Next = cups[nextCup - 1];
		cups[currentCup - 1]->Prev = cups[prevCup - 1];
	}

	return cups;
}

#if 0
static void PrintCupCircle(const Cup* start)
{
	printf("%" PRIu64 "", start->Id);
	for (Cup* c = start->Next; c != start; c = c->Next)
	{
		printf(" %" PRIu64 "", c->Id);
	}
	printf("\n");
}
#endif

#if 0
static void DestroyCupCirle(vector<Cup*>* cups)
{
	for (Cup* c : *cups)
	{
		delete c;
	}
	cups->clear();
}
#endif

static Cup* PickUp(Cup* c)
{
	c->Next->Prev = c->Prev;
	c->Prev->Next = c->Next;

	c->Next = nullptr;
	c->Prev = nullptr;

	return c;
}

static bool IsPickedUp(const Cup* c)
{
	return c->Next == nullptr;
}

static void InsertAfter(Cup* cupToInsert, Cup* cupToInsertAfter)
{
	cupToInsert->Next = cupToInsertAfter->Next;
	cupToInsert->Prev = cupToInsertAfter;

	cupToInsert->Next->Prev = cupToInsert;
	cupToInsert->Prev->Next = cupToInsert;
}

static void Puzzle23_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<size_t> startingArrangement;
	ranges::copy(ReadSingleLine(input) | views::transform([](char c) { return (size_t)(c - '0'); }), back_inserter(startingArrangement));
	size_t numberOfCups = startingArrangement.size();
	vector<Cup*> cupCircle = MakeCupCircle(startingArrangement);
	Cup* currentCup = cupCircle[startingArrangement[0] - 1];

	for (int i = 0; i < 100; i++)
	{
		//PrintCupCircle(currentCup);

		// The crab picks up the three cups that are immediately clockwise of the current cup.
		// They are removed from the circle; cup spacing is adjusted as necessary to maintain the circle.
		Cup* pickup[3];
		for (int p = 0; p < 3; p++)
		{
			pickup[p] = PickUp(currentCup->Next);
		}

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one.
		// If this would select one of the cups that was just picked up, the crab will keep subtracting one
		// until it finds a cup that wasn't just picked up.
		// If at any point in this process the value goes below the lowest value on any cup's label,
		// it wraps around to the highest value on any cup's label instead.
		size_t currentCupIndex = currentCup->Id - 1;
		size_t destinationCupIndex = (currentCupIndex + numberOfCups - 1) % numberOfCups;
		while (IsPickedUp(cupCircle[destinationCupIndex]))
		{
			destinationCupIndex = (destinationCupIndex + numberOfCups - 1) % numberOfCups;
		}

		Cup* destinationCup = cupCircle[destinationCupIndex];

		// The crab places the cups it just picked up so that they are immediately clockwise
		// of the destination cup. They keep the same order as when they were picked up.
		for (int p = 3; p > 0; p--)
		{
			InsertAfter(pickup[p - 1], destinationCup);
		}

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup.
		currentCup = currentCup->Next;
	}

	//PrintCupCircle(currentCup);

	int64_t answer = 0;
	for (const Cup* c = cupCircle[0]->Next; c->Id != 1; c = c->Next)
	{
		answer = (answer * 10) + c->Id;
	}

	printf("[2020] Puzzle23_A: %" PRId64 "\n", answer);
}

static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t numberOfCups = 1000000;

	vector<size_t> startingArrangement;
	ranges::copy(ReadSingleLine(input) | views::transform([](char c) { return (size_t)(c - '0'); }), back_inserter(startingArrangement));

	startingArrangement.reserve(1000000);
	while (startingArrangement.size() < 1000000)
	{
		startingArrangement.push_back(startingArrangement.size() + 1);
	}
	
	vector<Cup*> cupCircle = MakeLargeCupCircle(startingArrangement, numberOfCups);

	Cup* currentCup = cupCircle[startingArrangement[0] - 1];

	for (int i = 0; i < 10000000; i++)
	{
		// The crab picks up the three cups that are immediately clockwise of the current cup.
		// They are removed from the circle; cup spacing is adjusted as necessary to maintain the circle.
		Cup* pickup[3];
		for (int p = 0; p < 3; p++)
		{
			pickup[p] = PickUp(currentCup->Next);
		}

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one.
		// If this would select one of the cups that was just picked up, the crab will keep subtracting one
		// until it finds a cup that wasn't just picked up.
		// If at any point in this process the value goes below the lowest value on any cup's label,
		// it wraps around to the highest value on any cup's label instead.
		size_t currentCupIndex = currentCup->Id - 1;
		size_t destinationCupIndex = (currentCupIndex + numberOfCups - 1) % numberOfCups;
		while (IsPickedUp(cupCircle[destinationCupIndex]))
		{
			destinationCupIndex = (destinationCupIndex + numberOfCups - 1) % numberOfCups;
		}

		Cup* destinationCup = cupCircle[destinationCupIndex];

		// The crab places the cups it just picked up so that they are immediately clockwise
		// of the destination cup. They keep the same order as when they were picked up.
		for (int p = 3; p > 0; p--)
		{
			InsertAfter(pickup[p - 1], destinationCup);
		}

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup.
		currentCup = currentCup->Next;
	}

	int64_t answer = cupCircle[0]->Next->Id * cupCircle[0]->Next->Next->Id;

	printf("[2020] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2020(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
