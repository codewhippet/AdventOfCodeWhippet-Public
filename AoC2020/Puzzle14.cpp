#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2020_Types
{
	struct Mask
	{
		uint64_t AND = ~0ull;
		uint64_t OR = 0ull;
	};
}

using namespace Puzzle14_2020_Types;

static Mask MakeMask(const string& pattern)
{
	Mask m;

	for (char c : pattern)
	{
		m.AND = m.AND << 1;
		m.OR = m.OR << 1;

		switch (c)
		{
		case '0':
			// Keep AND as 0
			break;

		case '1':
			m.AND |= 1ll;
			m.OR |= 1ll;
			break;

		case 'X':
			m.AND |= 1ll;
			break;
		}
	}

	return m;
}

static string DecimalToBinary(uint64_t decimal)
{
	ostringstream out;
	out << bitset<36>{decimal};
	return out.str();
}

static string MaskAddress(const string& address, const string &mask)
{
	assert(address.size() == mask.size());

	string maskedAddress = address;
	for (size_t i = 0; i < mask.size(); i++)
	{
		maskedAddress[i] = mask[i] == '0' ? maskedAddress[i] : mask[i];
	}
	return maskedAddress;
}

static void WriteMemory(string address, uint64_t value, map<string, uint64_t>* memory)
{
	size_t xPos = address.find('X', 0);

	if (xPos == string::npos)
	{
		(*memory)[address] = value;
	}
	else
	{
		address[xPos] = '0';
		WriteMemory(address, value, memory);
		address[xPos] = '1';
		WriteMemory(address, value, memory);
	}
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	regex maskFormat(R"(mask = (.+))");
	regex memFormat(R"(mem\[(\d+)\] = (\d+))");

	map<uint64_t, uint64_t> memory;
	Mask mask;
	for (const string& line : ReadAllLines(input))
	{
		smatch maskMatch;
		if (regex_match(line, maskMatch, maskFormat))
		{
			mask = MakeMask(maskMatch[1].str());
		}

		smatch memMatch;
		if (regex_match(line, memMatch, memFormat))
		{
			uint64_t address = stoull(memMatch[1].str());
			uint64_t unmaskedValue = stoull(memMatch[2].str());
			uint64_t maskedValue = (unmaskedValue & mask.AND) | mask.OR;
			memory[address] = maskedValue;
		}
	}

	uint64_t answer = 0;
	for (map<uint64_t, uint64_t>::const_reference data : memory)
	{
		answer += data.second;
	}

	printf("[2020] Puzzle14_A: %" PRIu64 "\n", answer);
}

static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);

	regex maskFormat(R"(mask = (.+))");
	regex memFormat(R"(mem\[(\d+)\] = (\d+))");

	map<string, uint64_t> memory;
	string mask;
	for (const string& line : ReadAllLines(input))
	{
		smatch maskMatch;
		if (regex_match(line, maskMatch, maskFormat))
		{
			mask = maskMatch[1].str();
		}

		smatch memMatch;
		if (regex_match(line, memMatch, memFormat))
		{
			uint64_t address = stoull(memMatch[1].str());
			uint64_t value = stoull(memMatch[2].str());
			WriteMemory(MaskAddress(DecimalToBinary(address), mask), value, &memory);
		}
	}

	uint64_t answer = 0;
	for (map<string, uint64_t>::const_reference data : memory)
	{
		answer += data.second;
	}

	printf("[2020] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2020(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
