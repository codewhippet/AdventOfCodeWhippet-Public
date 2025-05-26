#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2021_Types
{
	static const int64_t kOPEN = -1;
	static const int64_t kCOMMA = -2;
	static const int64_t kCLOSE = -3;
}

using namespace Puzzle18_2021_Types;

static vector<int64_t> ParseSnailfishNumber(const string& number)
{
	vector<int64_t> ret;
	ret.reserve(number.size());

	for (char c : number)
	{
		switch (c)
		{
		case '[':
			ret.push_back(kOPEN);
			break;

		case ',':
			ret.push_back(kCOMMA);
			break;

		case ']':
			ret.push_back(kCLOSE);
			break;

		default:
			ret.push_back(c - '0');
			break;
		}
	}

	return ret;
}

static string SnailfishToString(const vector<int64_t>& number)
{
	string ret;
	for (int64_t i : number)
	{
		switch (i)
		{
		case kOPEN:
			ret += "[";
			break;

		case kCOMMA:
			ret += ",";
			break;

		case kCLOSE:
			ret += "]";
			break;

		default:
			assert(i < 10);
			ret += (char)i + '0';
			break;
		}
	}
	return ret;
}

static vector<int64_t> Slice(const vector<int64_t>& v, int64_t offset, int64_t count = numeric_limits<int64_t>::max())
{
	return count == numeric_limits<int64_t>::max() ?
		vector<int64_t>(v.begin() + offset, v.end()) :
		vector<int64_t>(v.begin() + offset, v.begin() + offset + count);
}

static bool FindFirstAtom(const vector<int64_t>& number, size_t* index)
{
	for (size_t i = 0; i < number.size(); i++)
	{
		if (number[i] > -1)
		{
			*index = i;
			return true;
		}
	}
	return false;
}

static bool FindLastAtom(const vector<int64_t>& number, size_t* index)
{
	for (size_t i = number.size(); i > 0; i--)
	{
		if (number[i - 1] > -1)
		{
			*index = i - 1;
			return true;
		}
	}
	return false;
}

static bool FindFirstUnstableAtom(const vector<int64_t>& number, size_t* index)
{
	for (size_t i = 0; i < number.size(); i++)
	{
		if (number[i] > 9)
		{
			*index = i;
			return true;
		}
	}
	return false;
}

static void Append(vector<int64_t>* dest, const vector<int64_t>& source)
{
	dest->reserve(dest->size() + source.size());
	for (int64_t i : source)
	{
		dest->push_back(i);
	}
}

static bool Explode(vector<int64_t>* number)
{
	int depth = 0;
	for (size_t i = 0; i < number->size(); i++)
	{
		int64_t current = (*number)[i];
		if (current > -1)
		{
			continue;
		}
		if (current == kOPEN)
		{
			depth += 1;
			if (depth == 5)
			{
				// Explode
				vector<int64_t> before = Slice(*number, 0, i);
				size_t firstLeft;
				if (FindLastAtom(before, &firstLeft))
				{
					before[firstLeft] += (*number)[i + 1];
				}

				vector<int64_t> after = Slice(*number, i + 5);
				size_t firstRight;
				if (FindFirstAtom(after, &firstRight))
				{
					after[firstRight] += (*number)[i + 3];
				}

				// Recombine
				number->clear();
				for (int64_t b : before)
				{
					number->push_back(b);
				}
				number->push_back(0);
				for (int64_t a : after)
				{
					number->push_back(a);
				}

				return true;
			}
		}
		if (current == kCLOSE)
		{
			depth -= 1;
			assert(depth >= 0);
		}
	}
	assert(depth == 0);
	return false;
}

static bool Split(vector<int64_t>* number)
{
	size_t firstBigNumber;
	if (FindFirstUnstableAtom(*number, &firstBigNumber))
	{
		int64_t currentValue = (*number)[firstBigNumber];
		int64_t leftValue = currentValue / 2;
		int64_t rightValue = (currentValue + 1) / 2;

		vector<int64_t> newNumber;
		Append(&newNumber, Slice(*number, 0, firstBigNumber));
		newNumber.push_back(kOPEN);
		newNumber.push_back(leftValue);
		newNumber.push_back(kCOMMA);
		newNumber.push_back(rightValue);
		newNumber.push_back(kCLOSE);
		Append(&newNumber, Slice(*number, firstBigNumber + 1));

		*number = newNumber;
		return true;
	}

	return false;
}

static vector<int64_t> Reduce(const vector<int64_t>& number)
{
	vector<int64_t> ret = number;
	while (true)
	{
		if (Explode(&ret) == false)
		{
			if (Split(&ret) == false)
			{
				return ret;
			}
		}
	}
}

static vector<int64_t> Add(const vector<int64_t>& lhs, const vector<int64_t>& rhs)
{
	vector<int64_t> sum;
	sum.push_back(kOPEN);
	Append(&sum, lhs);
	sum.push_back(kCOMMA);
	Append(&sum, rhs);
	sum.push_back(kCLOSE);

	return Reduce(sum);
}

static int64_t Magnitude(const vector<int64_t>& number, size_t& index)
{
	int64_t current = number[index];

	if (current > -1)
	{
		return current;
	}
	else
	{
		assert(current == kOPEN);
		int64_t leftValue = Magnitude(number, ++index);
		index++;
		assert(number[index] == kCOMMA);
		int64_t rightValue = Magnitude(number, ++index);
		index++;
		assert(number[index] == kCLOSE);

		assert(leftValue > -1);
		assert(rightValue > -1);
		return (leftValue * 3) + (rightValue * 2);
	}
}

static void Puzzle18_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> numbers = ReadAllLines(input);
	vector<int64_t> sum = ParseSnailfishNumber(numbers[0]);
	for (size_t i = 1; i < numbers.size(); i++)
	{
		sum = Add(sum, ParseSnailfishNumber(numbers[i]));
	}

	size_t index = 0;
	string finalSum = SnailfishToString(sum);
	int64_t answer = Magnitude(sum, index);

	printf("[2021] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<vector<int64_t>> snailfishNumbers = MakeEnumerator(ReadAllLines(input))
		->Select<vector<int64_t>>([](const string& line) { return ParseSnailfishNumber(line); })
		->ToVector();

	int64_t largest = -1;
	for (size_t i = 0; i < snailfishNumbers.size(); i++)
	{
		for (size_t j = 0; j < snailfishNumbers.size(); j++)
		{
			if (i == j)
				continue;

			size_t index = 0;
			largest = max<int64_t>(largest, Magnitude(Add(snailfishNumbers[i], snailfishNumbers[j]), index));
		}
	}

	int64_t answer = largest;

	printf("[2021] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2021(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
