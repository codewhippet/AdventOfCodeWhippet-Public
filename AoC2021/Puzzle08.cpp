#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2021_Types
{
}

using namespace Puzzle08_2021_Types;

// 0: ABC_EFG
// 1: __C__F_
// 2: A_CDE_G
// 3: A_CD_FG
// 4: _BCD_F_
// 5: AB_D_FG
// 6: AB_DEFG
// 7: A_C__F_
// 8: ABCDEFG
// 9: ABCD_FG

// Know: 1, 4, 7, 8
// 
// A appears 8
// B appears 6
// C appears 8
// D appears 7
// E appears 4
// F appears 9
// G appears 7
// Therefore know B, E, F

// A = 7 - 1
// B = Known
// C = 1 - F
// D = 4 - B - C - F
// E = Known
// F = Known
// G = 8 - A - B - C - D - E - F

static int ToDigits(char c)
{
	return 1 << (c - 'a');
}

static int ToDigits(const string& str)
{
	int digits = 0;
	for (char c : str)
	{
		digits |= ToDigits(c);
	}
	return digits;
}

static map<int, int> Solve(const vector<string>& digits)
{
	// 1 has 2 segments
	int D_1 = MakeEnumerator(digits)
		->Where([](const string& s) { return s.length() == 2; })
		->Select<int>([](const string& s) { return ToDigits(s); })
		->First();

	// 4 has 4 segments
	int D_4 = MakeEnumerator(digits)
		->Where([](const string& s) { return s.length() == 4; })
		->Select<int>([](const string& s) { return ToDigits(s); })
		->First();

	// 7 has 3 segments
	int D_7 = MakeEnumerator(digits)
		->Where([](const string& s) { return s.length() == 3; })
		->Select<int>([](const string& s) { return ToDigits(s); })
		->First();

	// 8 has 7 segments
	int D_8 = MakeEnumerator(digits)
		->Where([](const string& s) { return s.length() == 7; })
		->Select<int>([](const string& s) { return ToDigits(s); })
		->First();

	map<char, int> charCounts;
	for (const string& str : digits)
	{
		for (char c : str)
		{
			charCounts[c]++;
		}
	}

	// B segment appears 5 times
	int B = ToDigits(find_if(charCounts.begin(), charCounts.end(),
		[](map<char, int>::const_reference kvp)
		{
			return kvp.second == 6;
		})
		->first);

	// E segment appears 4 times
	int E = ToDigits(find_if(charCounts.begin(), charCounts.end(),
		[](map<char, int>::const_reference kvp)
		{
			return kvp.second == 4;
		})
		->first);

	// F segment appears 9 times
	int F = ToDigits(find_if(charCounts.begin(), charCounts.end(),
		[](map<char, int>::const_reference kvp)
		{
			return kvp.second == 9;
		})
		->first);

	// A = 7 - 1
	// B = Known
	// C = 1 - F
	// D = 4 - B - C - F
	// E = Known
	// F = Known
	// G = 8 - A - B - C - D - E - F

	int A = D_7 & ~D_1;
	int C = D_1 & ~F;
	int D = D_4 & ~B & ~C & ~F;
	int G = D_8 & ~A & ~B & ~C & ~D & ~E & ~F;

	map<int, int> mapping;

	mapping[(A | B | C | E | F | G)] = 0;
	mapping[(C | F)] = 1;
	mapping[(A | C | D | E | G)] = 2;
	mapping[(A | C | D | F | G)] = 3;
	mapping[(B | C | D | F)] = 4;
	mapping[(A | B | D | F | G)] = 5;
	mapping[(A | B | D | E | F | G)] = 6;
	mapping[(A | C | F)] = 7;
	mapping[(A | B | C | D | E | F | G)] = 8;
	mapping[(A | B | C | D | F | G)] = 9;

	return mapping;
}

static void Puzzle08_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	regex reg(R"([^\|]+\|([^\|]+))", regex::icase);
	vector<string> outputValues = MakeEnumerator(lines)
		->Select<string>([&reg](const string& line)
			{
				smatch m;
				regex_match(line, m, reg);
				return m[1].str();
			})
		->ToVector();

	map<int, int> lengths;

	for (const string& outputValue : outputValues)
	{
		istringstream values(outputValue);
		string value;
		while (values >> value)
		{
			lengths[(int)value.length()]++;
		}
	}

	int64_t answer = lengths[2] + lengths[4] + lengths[3] + lengths[7];

	printf("[2021] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	regex reg(R"(([^\|]+)\|([^\|]+))", regex::icase);
	vector<pair<string, string>> outputValues = MakeEnumerator(lines)
		->Select<pair<string, string>>([&reg](const string& line)
			{
				smatch m;
				regex_match(line, m, reg);
				return make_pair(m[1].str(), m[2].str());
			})
		->ToVector();

	map<int, int> lengths;

	int64_t answer = 0;

	for (const pair<string, string>& outputValue : outputValues)
	{
		vector<string> digits;
		istringstream values(outputValue.first);
		string value;
		while (values >> value)
		{
			digits.push_back(value);
		}

		int outputNumber = 0;

		map<int, int> digitMap = Solve(digits);
		istringstream output(outputValue.second);
		while (output >> value)
		{
			outputNumber *= 10;
			int digitValue = digitMap[ToDigits(value)];
			outputNumber += digitValue;
		}

		answer += outputNumber;
	}

	printf("[2021] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2021(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
