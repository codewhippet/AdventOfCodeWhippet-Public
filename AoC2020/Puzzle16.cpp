#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2020_Types
{
	struct TicketInfo 
	{
		vector<pair<string, function<bool(int64_t)>>> FieldRules;
		vector<int64_t> MyTicket;
		vector<vector<int64_t>> NearbyTickets;
	};
}

using namespace Puzzle16_2020_Types;

static function<bool(int64_t)> MakeRule(int64_t a, int64_t b, int64_t c, int64_t d)
{
	return [a, b, c, d](int64_t v) { return ((v >= a) && (v <= b)) || ((v >= c) && (v <= d)); };
}

static TicketInfo ReadPuzzle(istream& input)
{
	TicketInfo info;

	vector<string> lines = ReadAllLines(input);

	size_t lineNo = 0;

	regex ruleFormat(R"(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))");
	while (lines[lineNo].empty() == false)
	{
		smatch ruleMatch;
		bool shouldMatch = regex_match(lines[lineNo], ruleMatch, ruleFormat);
		assert(shouldMatch);
		(void)shouldMatch;
		function<bool(int64_t)> validityRule = MakeRule(
			stoll(ruleMatch[2]),
			stoll(ruleMatch[3]),
			stoll(ruleMatch[4]),
			stoll(ruleMatch[5]));
		info.FieldRules.push_back(make_pair(ruleMatch[1].str(), move(validityRule)));
		lineNo++;
	}

	lineNo += 2;
	info.MyTicket = MakeEnumerator(StringSplit(lines[lineNo], ','))
		->Select<int64_t>([](const string& s) { return stoll(s); })
		->ToVector();

	lineNo += 3;
	while (lineNo < lines.size())
	{
		info.NearbyTickets.push_back(
			MakeEnumerator(StringSplit(lines[lineNo], ','))
				->Select<int64_t>([](const string& s) { return stoll(s); })
				->ToVector());
		lineNo++;
	}

	return info;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	TicketInfo info = ReadPuzzle(input);

	vector<int64_t> invalidValues;
	for (const vector<int64_t>& ticket : info.NearbyTickets)
	{
		for (int64_t value : ticket)
		{
			bool isValid = false;
			for (vector<pair<string, function<bool(int64_t)>>>::const_reference validityCheck : info.FieldRules)
			{
				if (validityCheck.second(value))
				{
					isValid = true;
					break;
				}
			}
			if (isValid == false)
			{
				invalidValues.push_back(value);
			}
		}
	}

	int64_t answer = accumulate(invalidValues.begin(), invalidValues.end(), 0ll);

	printf("[2020] Puzzle16_A: %" PRId64 "\n", answer);
}

static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	TicketInfo info = ReadPuzzle(input);

	map<size_t, vector<int64_t>> ticketValidityMasks;
	for (size_t ticketIndex = 0; ticketIndex < info.NearbyTickets.size(); ticketIndex++)
	{
		const vector<int64_t>& ticket = info.NearbyTickets[ticketIndex];
		vector<int64_t> ticketValueValidity;
		for (int64_t value : ticket)
		{
			int64_t validityMask = 0;
			for (size_t fieldIndex = 0; fieldIndex < info.FieldRules.size(); fieldIndex++)
			{
				int64_t valid = info.FieldRules[fieldIndex].second(value) ? 1 : 0;
				validityMask |= (valid << fieldIndex);
			}
			ticketValueValidity.push_back(validityMask);
		}
		if (find(ticketValueValidity.begin(), ticketValueValidity.end(), 0) == ticketValueValidity.end())
		{
			ticketValidityMasks[ticketIndex] = move(ticketValueValidity);
		}
	}

	vector<int64_t> globalValidity;
	globalValidity.resize(ticketValidityMasks.begin()->second.size());
	fill(globalValidity.begin(), globalValidity.end(), ~0ll);

	for (map<size_t, vector<int64_t>>::const_reference ticketValidity : ticketValidityMasks)
	{
		for (size_t valueIndex = 0; valueIndex < ticketValidity.second.size(); valueIndex++)
		{
			globalValidity[valueIndex] &= ticketValidity.second[valueIndex];
		}
	}

	map<size_t, size_t> fieldIndexToValueIndex;
	while (true)
	{
		vector<int64_t>::iterator canBeMapped = find_if(globalValidity.begin(), globalValidity.end(),
			[](int64_t v) { return __popcnt64(v) == 1; });
		if (canBeMapped == globalValidity.end())
		{
			break;
		}

		int64_t validityMask = *canBeMapped;

		int64_t fieldIndex = _tzcnt_u64(validityMask);
		fieldIndexToValueIndex[fieldIndex] = distance(globalValidity.begin(), canBeMapped);

		for_each(globalValidity.begin(), globalValidity.end(), [validityMask](int64_t& v) { v &= ~validityMask; });
	}

	int64_t answer = 1;
	for (size_t fieldIndex = 0; fieldIndex < info.FieldRules.size(); fieldIndex++)
	{
		if (info.FieldRules[fieldIndex].first.substr(0, sizeof("departure") - 1) == "departure")
		{
			answer *= info.MyTicket[fieldIndexToValueIndex[fieldIndex]];
		}
	}

	printf("[2020] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2020(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
