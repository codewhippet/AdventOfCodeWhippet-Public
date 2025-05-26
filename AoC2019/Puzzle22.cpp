#include "stdafx.h"
#include <intrin.h>

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2019_Types
{
	enum class ShuffleMove
	{
		Reverse,
		Cut,
		Increment,
	};

	struct ModularEquation
	{
		int64_t Coefficient = 0;
		int64_t Constant = 0;
		int64_t Modulo = 0;

		void Multiply(int64_t scale);
		void Add(int64_t value);
		void NegateAndAdd(int64_t value);
	};
}

using namespace Puzzle22_2019_Types;

static vector<int64_t> MakeDeck(size_t cards)
{
	vector<int64_t> deck(cards);
	iota(deck.begin(), deck.end(), 0);
	return deck;
}

static vector<pair<ShuffleMove, int64_t>> ReadShuffleSteps(istream& input)
{
	vector<pair<ShuffleMove, int64_t>> shuffleSteps;
	for (const string& line : ReadEachLine(input))
	{
		int64_t param = 0;
		if (line == "deal into new stack"sv)
		{
			shuffleSteps.push_back({ ShuffleMove::Reverse, 0 });
		}
		else if (sscanf(line.c_str(), "cut %lld", &param) == 1)
		{
			shuffleSteps.push_back({ ShuffleMove::Cut, param });
		}
		else if (sscanf(line.c_str(), "deal with increment %lld", &param) == 1)
		{
			shuffleSteps.push_back({ ShuffleMove::Increment, param });
		}
		else
		{
			assert(false);
		}
	}

	return shuffleSteps;
}

static int64_t MultiplicativeInverse(int64_t deckSize, int64_t increment)
{
	// https://extendedeuclideanalgorithm.com/multiplicative_inverse.php
	int64_t a, b, q, r, t1, t2, t3;

	b = deckSize;
	r = increment;
	t2 = 0;
	t3 = 1;
	while (true)
	{
		a = b;
		b = r;
		if (b == 0)
		{
			assert(false); // deckSize and increment aren't relatively prime
			return -1;
		}

		q = a / b;
		r = a % b;
		t1 = t2;
		t2 = t3;
		t3 = t1 - q * t2;

		if (b == 1)
		{
			int64_t inverse = abs(t2) % deckSize;
			assert(inverse != 0);
			return (t2 > 0 ? inverse : deckSize - inverse);
		}
	}
}

static int64_t MulMod(int64_t a, int64_t b, int64_t mod)
{
	__int64 hi;
	__int64 lo = _mul128(a, b, &hi);
	__int64 remainder;
	__int64 quotient = _div128(hi, lo, mod, &remainder);
	(void)quotient;
	return remainder;
}

void ModularEquation::Multiply(int64_t scale)
{
	Coefficient = MulMod(Coefficient, scale, Modulo);
	Constant = MulMod(Constant, scale, Modulo);
}

void ModularEquation::Add(int64_t value)
{
	Constant = (Constant + value) % Modulo;
}

void ModularEquation::NegateAndAdd(int64_t value)
{
	Coefficient = Modulo - Coefficient;

	Constant = value - Constant + Modulo;
	Constant = Constant % Modulo;
}

static ModularEquation RepeatModularEquation(const ModularEquation& meq, int repeatCount)
{
	ModularEquation repeatedMeq{ 1, 0, meq.Modulo };
	for (int i = 0; i < repeatCount; i++)
	{
		repeatedMeq.Multiply(meq.Coefficient);
		repeatedMeq.Add(meq.Constant);
	}
	return repeatedMeq;
}

static vector<ModularEquation> BuildEquationLadder(const ModularEquation& meq, int repeatCount, int ladderHeight)
{
	vector<ModularEquation> ladder{ meq };
	for (int i = 0; i < ladderHeight - 1; i++)
	{
		ladder.push_back(RepeatModularEquation(ladder.back(), repeatCount));
	}
	return ladder;
}

static int64_t RepeatShuffle(int64_t slot, const ModularEquation& meq, int repeatCount)
{
	for (int64_t i = 0; i < repeatCount; i++)
	{
		slot = MulMod(slot, meq.Coefficient, meq.Modulo);
		slot = (slot + meq.Constant) % meq.Modulo;
	}
	return slot;
}

static void Puzzle22_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const vector<pair<ShuffleMove, int64_t>> shuffleSteps = ReadShuffleSteps(input);

	vector<int64_t> deck = MakeDeck(10007);

	for (const auto& shuffleMove : shuffleSteps)
	{
		int64_t param = shuffleMove.second;
		if (shuffleMove.first == ShuffleMove::Reverse)
		{
			ranges::reverse(deck);
		}
		else if (shuffleMove.first == ShuffleMove::Cut)
		{
			vector<int64_t> newDeck(deck.size());

			int64_t offset = param >= 0 ? param : deck.size() + param;
			for (size_t i = 0; i < deck.size(); i++)
			{
				newDeck[i] = deck[(i + offset) % deck.size()];
			}

			deck.swap(newDeck);
		}
		else if (shuffleMove.first == ShuffleMove::Increment)
		{
			vector<int64_t> newDeck(deck.size());

			for (size_t i = 0; i < deck.size(); i++)
			{
				newDeck[(i * param) % newDeck.size()] = deck[i];
			}

			deck.swap(newDeck);
		}
		else
		{
			assert(false);
		}
	}

	int64_t answer = distance(deck.begin(), ranges::find(deck, 2019));

	printf("[2019] Puzzle22_A: %" PRId64 "\n", answer);
}


static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<ShuffleMove, int64_t>> shuffleSteps = ReadShuffleSteps(input);
	ranges::reverse(shuffleSteps);

	int64_t deckSize = 119315717514047ll;

	ModularEquation meq{ 1, 0, deckSize };
	for (const auto& shuffleMove : shuffleSteps)
	{
		int64_t param = shuffleMove.second;
		if (shuffleMove.first == ShuffleMove::Reverse)
		{
			meq.NegateAndAdd(deckSize - 1);
		}
		else if (shuffleMove.first == ShuffleMove::Cut)
		{
			int64_t offset = param >= 0 ? param : deckSize + param;
			meq.Add(offset);
		}
		else if (shuffleMove.first == ShuffleMove::Increment)
		{
			int64_t inverse = MultiplicativeInverse(deckSize, param);
			meq.Multiply(inverse);
		}
		else
		{
			assert(false);
		}
	}

	vector<ModularEquation> ladder = BuildEquationLadder(meq, 10, 16);

	int64_t numShuffles = 101741582076661ll;

	int64_t answer = 2020;
	size_t digit = 0;
	while (numShuffles > 0)
	{
		answer = RepeatShuffle(answer, ladder[digit], numShuffles % 10);
		numShuffles /= 10;
		digit++;
	}

	printf("[2019] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2019(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
