#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2021_Types
{
}

using namespace Puzzle03_2021_Types;

static pair<int, int> CountZerosAndOnes(const vector<string>& lines, int bitIndex)
{
	int oneCount = 0;
	for (const string& line : lines)
	{
		oneCount += line[bitIndex] == '1' ? 1 : 0;
	}
	int zeroCount = static_cast<int>(lines.size()) - oneCount;

	return make_pair(zeroCount, oneCount);
}

static int ParseBinary(const string& number)
{
	int value = 0;
	for (char c : number)
	{
		value <<= 1;
		value |= c == '1' ? 1 : 0;
	}
	return value;
}

static void Puzzle03_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	map<int, int> oneCount;
	for (int i = 0; i < lines[0].length(); i++)
	{
		for (const string& line : lines)
		{
			oneCount[i] += line[i] == '1' ? 1 : 0;
		}
	}

	int gamma = 0;
	int epsilon = 0;
	for (int i = 0; i < lines[0].length(); i++)
	{
		gamma <<= 1;
		epsilon <<= 1;

		if (oneCount[i] > lines.size() / 2)
		{
			gamma |= 1;
		}
		else
		{
			epsilon |= 1;
		}
	}

	int64_t answer = gamma * epsilon;

	printf("[2021] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	vector<string> oxygen = lines;
	int bitIndex = 0;
	while (oxygen.size() > 1)
	{
		pair<int, int> counts = CountZerosAndOnes(oxygen, bitIndex);

		char keep = counts.second >= counts.first ? '1' : '0';

		oxygen.erase(remove_if(oxygen.begin(), oxygen.end(),
			[keep, bitIndex](const string& line)
			{
				return line[bitIndex] != keep;
			}),
			oxygen.end());

		++bitIndex;
	}

	vector<string> co2 = lines;
	bitIndex = 0;
	while (co2.size() > 1)
	{
		pair<int, int> counts = CountZerosAndOnes(co2, bitIndex);

		char keep = counts.first <= counts.second ? '0' : '1';

		co2.erase(remove_if(co2.begin(), co2.end(),
			[keep, bitIndex](const string& line)
			{
				return line[bitIndex] != keep;
			}),
			co2.end());

		++bitIndex;
	}

	int oxygenGenerator = ParseBinary(oxygen[0]);
	int co2Scrubber = ParseBinary(co2[0]);

	int64_t answer = oxygenGenerator * co2Scrubber;

	printf("[2021] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2021(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
