#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2015_Types
{
}

using namespace Puzzle11_2015_Types;

static string Digits = "abcdefghjkmnpqrstuvwxyz";

static vector<uint8_t> DecodeDigits(const string& encoded)
{
	vector<uint8_t> digits;

	ranges::copy(encoded
		| views::transform([](char c) -> uint8_t
			{
				return (uint8_t)distance(Digits.begin(), find(Digits.begin(), Digits.end(), c));
			}),
		back_inserter(digits));
	ranges::reverse(digits);

	return digits;
}

static string EncodeDigits(const vector<uint8_t>& decoded)
{
	string digits;

	ranges::copy(decoded
		| views::transform([](uint8_t i)
			{
				return Digits[i];
			}),
		back_inserter(digits));
	ranges::reverse(digits);

	return digits;
}

static void Increment(vector<uint8_t>* digits)
{
	size_t i = 0;
	uint8_t carry = 1;
	while (carry > 0)
	{
		assert(i < digits->size());

		uint8_t incremented = (*digits)[i] + carry;
		carry = (uint8_t)(incremented / Digits.size());
		(*digits)[i] = incremented % Digits.size();

		i++;
	}
}

static bool IsValid(const vector<uint8_t>& digits)
{
	int decreaseRun = 0;
	bool hasDecrease = false;

	int pairCount = 0;
	bool suppressPairCheck = false;

	for (size_t i = 1; i < digits.size(); i++)
	{
		bool decrease = ((digits[i] - digits[i - 1]) == -1);
		decreaseRun = (decrease ? decreaseRun + 1 : 0);
		hasDecrease = hasDecrease || decreaseRun >= 2;

		bool same = (digits[i] == digits[i - 1]);
		if (same && !suppressPairCheck)
		{
			pairCount++;
			suppressPairCheck = true;
		}
		else
		{
			suppressPairCheck = false;
		}
	}

	return hasDecrease && pairCount >= 2;
}

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string password;
	getline(input, password);

	vector<uint8_t> digits = DecodeDigits(password);
	do
	{
		Increment(&digits);

	} while (IsValid(digits) == false);

	string answer = EncodeDigits(digits);

	printf("[2015] Puzzle11_A: %s\n", answer.c_str());
}


static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string password;
	getline(input, password);

	vector<uint8_t> digits = DecodeDigits(password);
	for (int i = 0; i < 2; i++)
	{
		do
		{
			Increment(&digits);

		} while (IsValid(digits) == false);
	}

	string answer = EncodeDigits(digits);

	printf("[2015] Puzzle11_B: %s\n", answer.c_str());
}

void Puzzle11_2015(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
