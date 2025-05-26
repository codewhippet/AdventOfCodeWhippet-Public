#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2023_Types
{
	enum class HandType
	{
		HighCard,
		OnePair,
		TwoPair,
		ThreeOfAKind,
		FullHouse,
		FourOfAKind,
		FiveOfAKind,
	};

	struct Hand
	{
		string Source;
		HandType Type;
		char Cards[5];
		int NumJokers;
	};

	bool operator<(const Hand& a, const Hand& b)
	{
		if (a.Type != b.Type)
		{
			return a.Type < b.Type;
		}
		for (int i = 0; i < 5; i++)
		{
			if (a.Cards[i] != b.Cards[i])
			{
				return a.Cards[i] < b.Cards[i];
			}
		}
		return false;
	}
}

using namespace Puzzle07_2023_Types;

static Hand ParseHand(const string& s)
{
	Hand h;
	h.Source = s;

	assert(s.length() == 5);
	for (int i = 0; i < 5; i++)
	{
		switch (s[i])
		{
		case '2': h.Cards[i] = 2; break;
		case '3': h.Cards[i] = 3; break;
		case '4': h.Cards[i] = 4; break;
		case '5': h.Cards[i] = 5; break;
		case '6': h.Cards[i] = 6; break;
		case '7': h.Cards[i] = 7; break;
		case '8': h.Cards[i] = 8; break;
		case '9': h.Cards[i] = 9; break;
		case 'T': h.Cards[i] = 10; break;
		case 'J': h.Cards[i] = 11; break;
		case 'Q': h.Cards[i] = 12; break;
		case 'K': h.Cards[i] = 13; break;
		case 'A': h.Cards[i] = 14; break;
		}
	}

	map<int, int> cardCounts;
	for (int i = 0; i < 5; i++)
	{
		cardCounts[h.Cards[i]]++;
	}

	map<int, int> countCounts;
	for (map<int, int>::const_reference cardCount : cardCounts)
	{
		countCounts[cardCount.second]++;
	}

	if (countCounts[5] == 1)
	{
		h.Type = HandType::FiveOfAKind;
	}
	else if (countCounts[4] == 1)
	{
		h.Type = HandType::FourOfAKind;
	}
	else if ((countCounts[3] == 1) && (countCounts[2] == 1))
	{
		h.Type = HandType::FullHouse;
	}
	else if (countCounts[3] == 1)
	{
		h.Type = HandType::ThreeOfAKind;
	}
	else if (countCounts[2] == 2)
	{
		h.Type = HandType::TwoPair;
	}
	else if (countCounts[2] == 1)
	{
		h.Type = HandType::OnePair;
	}
	else
	{
		h.Type = HandType::HighCard;
	}

	return h;
}

static vector<pair<Hand, int64_t>> ReadCards(istream& input)
{
	vector<pair<Hand, int64_t>> cards;
	while (input)
	{
		string card;
		int64_t rank;

		if (input >> card >> rank)
		{
			cards.emplace_back(make_pair(ParseHand(card), rank));
		}
	}
	return cards;
}

static Hand ParseHand2(const string& s)
{
	Hand h;
	h.Source = s;

	assert(s.length() == 5);
	for (int i = 0; i < 5; i++)
	{
		switch (s[i])
		{
		case 'J': h.Cards[i] = 1; break;
		case '2': h.Cards[i] = 2; break;
		case '3': h.Cards[i] = 3; break;
		case '4': h.Cards[i] = 4; break;
		case '5': h.Cards[i] = 5; break;
		case '6': h.Cards[i] = 6; break;
		case '7': h.Cards[i] = 7; break;
		case '8': h.Cards[i] = 8; break;
		case '9': h.Cards[i] = 9; break;
		case 'T': h.Cards[i] = 10; break;
		case 'Q': h.Cards[i] = 11; break;
		case 'K': h.Cards[i] = 12; break;
		case 'A': h.Cards[i] = 13; break;
		}
	}

	map<int, int> cardCounts;
	h.NumJokers = 0;
	for (int i = 0; i < 5; i++)
	{
		if (h.Cards[i] == 1)
		{
			h.NumJokers++;
		}
		else
		{
			cardCounts[h.Cards[i]]++;
		}
	}

	map<int, int> countCounts;
	for (map<int, int>::const_reference cardCount : cardCounts)
	{
		countCounts[cardCount.second]++;
	}

	if (countCounts[5] == 1)
	{
		assert(h.NumJokers == 0);
		h.Type = HandType::FiveOfAKind;
	}
	else if (countCounts[4] == 1)
	{
		HandType promotions[] =
		{
			HandType::FourOfAKind,
			HandType::FiveOfAKind,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}
	else if ((countCounts[3] == 1) && (countCounts[2] == 1))
	{
		HandType promotions[] =
		{
			HandType::FullHouse,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}
	else if (countCounts[3] == 1)
	{
		HandType promotions[] =
		{
			HandType::ThreeOfAKind,
			HandType::FourOfAKind,
			HandType::FiveOfAKind,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}
	else if (countCounts[2] == 2)
	{
		HandType promotions[] =
		{
			HandType::TwoPair,
			HandType::FullHouse,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}
	else if (countCounts[2] == 1)
	{
		HandType promotions[] =
		{
			HandType::OnePair,
			HandType::ThreeOfAKind,
			HandType::FourOfAKind,
			HandType::FiveOfAKind,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}
	else
	{
		HandType promotions[] =
		{
			HandType::HighCard,
			HandType::OnePair,
			HandType::ThreeOfAKind,
			HandType::FourOfAKind,
			HandType::FiveOfAKind,
			HandType::FiveOfAKind,
		};

		assert(h.NumJokers < sizeof(promotions) / sizeof(promotions[0]));
		h.Type = promotions[h.NumJokers];
	}

	return h;
}

static vector<pair<Hand, int64_t>> ReadCards2(istream& input)
{
	vector<pair<Hand, int64_t>> cards;
	while (input)
	{
		string card;
		int64_t rank;

		if (input >> card >> rank)
		{
			cards.emplace_back(make_pair(ParseHand2(card), rank));
		}
	}
	return cards;
}

static void Puzzle07_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Hand, int64_t>> cards = ReadCards(input);

	int64_t answer = 0;

	sort(cards.begin(), cards.end());
	for (size_t i = 0; i < cards.size(); i++)
	{
		int64_t rank = i + 1;
		answer += rank * cards[i].second;
	}

	printf("[2023] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<pair<Hand, int64_t>> cards = ReadCards2(input);

	int64_t answer = 0;

	sort(cards.begin(), cards.end());
	for (size_t i = 0; i < cards.size(); i++)
	{
		int64_t rank = i + 1;
		answer += rank * cards[i].second;
	}

	printf("[2023] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2023(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
