#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2022_Types
{
}

using namespace Puzzle25_2022_Types;

static int64_t ToInt64(string snafu)
{
    static const map<char, int64_t> digitValue =
    {
        { '2', 2 },
        { '1', 1 },
        { '0', 0 },
        { '-', -1 },
        { '=', -2 }
    };

    int64_t ret = 0;

    ranges::reverse(snafu);
    int64_t power = 1;
    for (size_t i = 0; i < snafu.size(); i++)
    {
        ret += digitValue.at(snafu[i]) * power;
        power *= 5;
    }

    return ret;
}

static string ToSnafuAbs(int64_t number)
{
	if (number == 0)
		return "";

	int64_t digit = number % 5;
	number = (number - digit) / 5;

	string digitString = "";
	switch (digit)
	{
	case 0:
		digitString = "0";
		break;
	case 1:
		digitString = "1";
		break;
	case 2:
		digitString = "2";
		break;
	case 3:
		digitString = "=";
		number++;
		break;
	case 4:
		digitString = "-";
		number++;
		break;
	}

	return ToSnafuAbs(number) + digitString;
}

static string NegateSnafu(const string& snafu)
{
	const map<char, char> negate =
	{
		{ '2', '=' },
		{ '1', '-' },
		{ '0', '0' },
		{ '-', '1' },
		{ '=', '2' }
	};

	return snafu | views::transform([&](const char c) { return negate.at(c); }) | ranges::to<string>();
}

static string ToSnafu(int64_t number)
{
	if (number == 0)
		return "0";

	if (number < 0)
	{
		return NegateSnafu(ToSnafuAbs(-number));
	}

	return ToSnafuAbs(number);
}

static void Puzzle25_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> decimals = ReadEachLine(input)
		| views::transform(&ToInt64)
		| ranges::to<vector>();

	int64_t sum = ranges::fold_left(decimals, 0, plus{});

	string answer = ToSnafu(sum);

	printf("[2022] Puzzle25_A: %s\n", answer.c_str());
}

void Puzzle25_2022(const string& filename)
{
	Puzzle25_A(filename);
}
