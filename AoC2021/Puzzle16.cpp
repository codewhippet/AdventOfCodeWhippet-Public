#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2021_Types
{
	static map<char, string> NibbleToBits =
	{
		{'0', "0000"},
		{'1', "0001"},
		{'2', "0010"},
		{'3', "0011"},
		{'4', "0100"},
		{'5', "0101"},
		{'6', "0110"},
		{'7', "0111"},
		{'8', "1000"},
		{'9', "1001"},
		{'A', "1010"},
		{'B', "1011"},
		{'C', "1100"},
		{'D', "1101"},
		{'E', "1110"},
		{'F', "1111"},
	};

	static map<string, int> Version =
	{
		{"000", 0},
		{"001", 1},
		{"010", 2},
		{"011", 3},
		{"100", 4},
		{"101", 5},
		{"110", 6},
		{"111", 7},
	};
}

using namespace Puzzle16_2021_Types;

static string HexToBits(const string& hex)
{
	string ret;
	for (char c : hex)
	{
		ret += NibbleToBits[c];
	}
	return ret;
}

int BitsToInt(const string& bits)
{
	int ret = 0;
	for (char b : bits)
	{
		ret <<= 1;
		ret += (b == '1' ? 1 : 0);
	}
	return ret;
}

static int VersionSum(const string& packet, int* packetStart)
{
	int version = Version[packet.substr(*packetStart, 3)];
	*packetStart += 3;

	string type = packet.substr(*packetStart, 3);
	*packetStart += 3;

	if (type == "100")
	{
		while (true)
		{
			bool isLastNibble = packet[*packetStart] == '0';
			*packetStart += 5;
			if (isLastNibble)
				break;
		}
	}
	else
	{
		char lengthTypeId = packet[*packetStart];
		*packetStart += 1;
		if (lengthTypeId == '0')
		{
			int subPacketBits = BitsToInt(packet.substr(*packetStart, 15));
			*packetStart += 15;

			int subPacketStart = *packetStart;
			int subPacketEnd = subPacketStart + subPacketBits;
			while (*packetStart < subPacketEnd)
			{
				version += VersionSum(packet, packetStart);
			}
		}
		else
		{
			int numSubPackets = BitsToInt(packet.substr(*packetStart, 11));
			*packetStart += 11;
			for (int i = 0; i < numSubPackets; i++)
			{
				version += VersionSum(packet, packetStart);
			}
		}
	}

	return version;
}

static uint64_t Value(const string& packet, int* packetStart, int indent)
{
	int version = Version[packet.substr(*packetStart, 3)];
	(void)version;
	*packetStart += 3;

	string type = packet.substr(*packetStart, 3);
	*packetStart += 3;

	if (type == "100")
	{
		uint64_t value = 0;

		while (true)
		{
			value <<= 4;

			bool isLastNibble = packet[*packetStart] == '0';
			*packetStart += 1;

			value += BitsToInt(packet.substr(*packetStart, 4));
			*packetStart += 4;

			if (isLastNibble)
				break;
		}

		return value;
	}
	else
	{
		vector<uint64_t> subValues;

		char lengthTypeId = packet[*packetStart];
		*packetStart += 1;
		if (lengthTypeId == '0')
		{
			int subPacketBits = BitsToInt(packet.substr(*packetStart, 15));
			*packetStart += 15;

			int subPacketStart = *packetStart;
			int subPacketEnd = subPacketStart + subPacketBits;
			while (*packetStart < subPacketEnd)
			{
				subValues.push_back(Value(packet, packetStart, indent + 1));
			}
		}
		else
		{
			int numSubPackets = BitsToInt(packet.substr(*packetStart, 11));
			*packetStart += 11;
			for (int i = 0; i < numSubPackets; i++)
			{
				subValues.push_back(Value(packet, packetStart, indent + 1));
			}
		}

		if (type == "000")
		{
			return accumulate(subValues.begin(), subValues.end(), (uint64_t)0, plus<uint64_t>());
		}
		else if (type == "001")
		{
			return accumulate(subValues.begin(), subValues.end(), (uint64_t)1, multiplies<uint64_t>());
		}
		else if (type == "010")
		{
			return MakeEnumerator(subValues)->Min();
		}
		else if (type == "011")
		{
			return MakeEnumerator(subValues)->Max();
		}
		else if (type == "101")
		{
			assert(subValues.size() == 2);
			return (subValues[0] > subValues[1] ? 1 : 0);
		}
		else if (type == "110")
		{
			assert(subValues.size() == 2);
			return (subValues[0] < subValues[1] ? 1 : 0);
		}
		else if (type == "111")
		{
			assert(subValues.size() == 2);
			return (subValues[0] == subValues[1] ? 1 : 0);
		}
	}

	assert(false);

	return 0;
}

static void Puzzle16_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> packets = ReadAllLines(input);
	for (const string& packet : packets)
	{
		int packetStart = 0;
		answer += VersionSum(HexToBits(packet), &packetStart);
	}

	printf("[2021] Puzzle16_A: %" PRId64 "\n", answer);
}

static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	vector<string> packets = ReadAllLines(input);
	for (const string& packet : packets)
	{
		int packetStart = 0;
		answer += Value(HexToBits(packet), &packetStart, 0);
	}

	printf("[2021] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2021(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
