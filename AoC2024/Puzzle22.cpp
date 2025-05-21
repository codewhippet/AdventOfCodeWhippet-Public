#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2024_Types
{
}

using namespace Puzzle22_2024_Types;

static int64_t Mix(int64_t secretNumber, int64_t value)
{
	return secretNumber ^ value;
}

static int64_t Prune(int64_t secretNumber)
{
	return secretNumber & 0xffffff;
}

static int64_t Step(int64_t secretNumber)
{
	secretNumber = Mix(secretNumber, secretNumber * 64);
	secretNumber = Prune(secretNumber);
	secretNumber = Mix(secretNumber, secretNumber / 32);
	secretNumber = Prune(secretNumber);
	secretNumber = Mix(secretNumber, secretNumber * 2048);
	secretNumber = Prune(secretNumber);

	return secretNumber;
}

static void Puzzle22_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& line : ReadAllLines(input))
	{
		int64_t secretNumber = atoll(line.c_str());
		for (int i = 0; i < 2000; i++)
		{
			secretNumber = Step(secretNumber);
		}
		answer += secretNumber;
	}

	printf("[2024] Puzzle22_A: %" PRId64 "\n", answer);
}

static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, int64_t> allSequenceValues;

	for (const string& line : ReadAllLines(input))
	{
		int64_t secretNumber = atoll(line.c_str());

		int64_t lastPrice = secretNumber % 10;

		vector<pair<int64_t, int64_t>> priceDelta;
		priceDelta.push_back({ lastPrice, -100 });

		for (int i = 0; i < 2000; i++)
		{
			secretNumber = Step(secretNumber);
			int64_t newPrice = secretNumber % 10;
			priceDelta.push_back({ newPrice, newPrice - lastPrice });
			lastPrice = newPrice;
		}

		map<string, int64_t> sequenceValues;
		for (size_t i = 1; i + 3 < priceDelta.size(); i++)
		{
			char buffer[64];
			sprintf(buffer, "%lld,%lld,%lld,%lld",
				priceDelta[i + 0].second,
				priceDelta[i + 1].second,
				priceDelta[i + 2].second,
				priceDelta[i + 3].second);
			string sequenceKey{ buffer };

			if (sequenceValues.contains(sequenceKey) == false)
			{
				sequenceValues[sequenceKey] = priceDelta[i + 3].first;
			}
		}

		for (const auto& sequenceValue : sequenceValues)
		{
			allSequenceValues[sequenceValue.first] += sequenceValue.second;
		}
	}

	int64_t answer = ranges::max(allSequenceValues
		| views::transform([](const auto& sequenceValue)
			{
				return sequenceValue.second;
			})
	);

	printf("[2024] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2024(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
