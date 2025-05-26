#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2023_Types
{
	struct Card
	{
		int Id;
		set<int64_t> WinningNumbers;
		vector<int64_t> CardNumbers;
	};
}

using namespace Puzzle04_2023_Types;

static vector<Card> ParseCards(istream& input)
{
	vector<Card> cards;
	for (const string& line : ReadAllLines(input))
	{
		Card c;

		vector<char> winningNumbersText(line.size(), 0);
		vector<char> cardNumbersText(line.size(), 0);
		if (sscanf(line.c_str(), "Card %d:%[^|]|%[0-9 ]", &c.Id, winningNumbersText.data(), cardNumbersText.data()) == 3)
		{
			c.WinningNumbers = Enumerable::Regex(string{ winningNumbersText.data() }, regex{ R"(\d+)" })
				->Select<int64_t>([](const smatch& m) { return stoll(m.str()); })
				->ToSet();

			c.CardNumbers = Enumerable::Regex(string{ cardNumbersText.data() }, regex{ R"(\d+)" })
				->Select<int64_t>([](const smatch& m) { return stoll(m.str()); })
				->ToVector();

			cards.push_back(move(c));
		}
	}

	return cards;
}

static int64_t CardsFromCard(int cardId, const vector<Card>& cards)
{
	static map<int, int64_t> cache;
	map<int, int64_t>::const_iterator existingSolution = cache.find(cardId);
	if (existingSolution != cache.end())
	{
		return existingSolution->second;
	}

	int64_t numCardsFromCard = 1; // All cards earn themselves

	const Card& card = cards[cardId];
	int matches = (int)count_if(card.CardNumbers.begin(), card.CardNumbers.end(),
		[&card](int64_t n) { return card.WinningNumbers.contains(n); });
	for (int i = 0; i < matches; i++)
	{
		numCardsFromCard += CardsFromCard(cardId + 1 + i, cards);
	}

	cache[cardId] = numCardsFromCard;
	return numCardsFromCard;
}

static void Puzzle04_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Card> cards = ParseCards(input);

	int64_t answer = 0;
	for (const Card& c : cards)
	{
		int matches = (int)count_if(c.CardNumbers.begin(), c.CardNumbers.end(),
			[&c](int64_t n) { return c.WinningNumbers.contains(n); });
		if (matches > 0)
		{
			int64_t points = 1ll << (matches - 1);
			answer += points;
		}
	}

	printf("[2023] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Card> cards = ParseCards(input);
	cards.insert(cards.begin(), Card{});

	int64_t answer = 0;
	for (int i = 1; i < cards.size(); i++)
	{
		answer += CardsFromCard(i, cards);
	}

	printf("[2023] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2023(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
