#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle14_2016_Types
{
}

using namespace Puzzle14_2016_Types;

static bool IsNInARow(unsigned char hash[16], const int target, char *rep)
{
	int c = 0xff;
	int count = 0;
	for (int i = 0; i < 16; i++)
	{
		int h = (hash[i] >> 4) & 0xf;
		count = (h == c ? count + 1 : 1);
		if (count == target)
		{
			*rep = (char)h;
			return true;
		}
		c = h;

		h = hash[i] & 0xf;
		count = (h == c ? count + 1 : 1);
		if (count == target)
		{
			*rep = (char)h;
			return true;
		}
		c = h;
	}
	return false;
}

static int64_t FindIndexOfNthKey(const string& salt, const size_t keysNeeded, const function<void(const char*, int, unsigned char[16])>& hasher)
{
	map<int64_t, char> threeInARows;

	set<int64_t> keysFoundAt;
	int64_t stopIndex = -1;

	int64_t index = 0;
	while ((stopIndex == -1) || (index < stopIndex))
	{
		char buffer[64];
		int secretLength = sprintf(buffer, "%s%" PRId64 "", salt.c_str(), index);

		unsigned char hash[16];
		hasher(buffer, secretLength, hash);

		{
			char rep = 0;
			if (IsNInARow(hash, 5, &rep))
			{
				auto threeInARowsBegin = threeInARows.lower_bound(index - 1000);
				auto threeInARowsEnd = threeInARows.upper_bound(index);
				for (auto it = threeInARowsBegin; it != threeInARowsEnd; ++it)
				{
					if (it->second == rep)
					{
						keysFoundAt.insert(it->first);
						if ((keysFoundAt.size() >= keysNeeded) && (stopIndex == -1))
						{
							// Keep searching to make sure we've evaluated all potential triples prior to this point
							stopIndex = index + 1000;
						}
					}
				}
			}
		}

		{
			char rep = 0;
			if (IsNInARow(hash, 3, &rep))
			{
				threeInARows[index] = rep;
			}
		}

		index++;
	}

	return (keysFoundAt | views::drop(keysNeeded - 1)).front();
}

static void Puzzle14_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t keysNeeded = 64;

	string salt;
	getline(input, salt);

	MD5 md5;

	int64_t answer = FindIndexOfNthKey(salt, keysNeeded,
		[&](const char* buffer, int secretLength, unsigned char hash[16])
		{
			md5.Hash((uint8_t*)buffer, secretLength, hash);
		});

	printf("[2016] Puzzle14_A: %" PRId64 "\n", answer);
}


static void Puzzle14_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t keysNeeded = 64;

	string salt;
	getline(input, salt);

	MD5 md5;

	function<void(const char*, int, unsigned char[16])> stretchHash =
		[&](const char* buffer, int secretLength, unsigned char hash[16])
		{
			md5.Hash((uint8_t*)buffer, secretLength, hash);

			char scratchBuffer[64];
			for (int i = 0; i < 2016; i++)
			{
				int scratchLength = sprintf(scratchBuffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
					hash[0], hash[1], hash[2], hash[3],
					hash[4], hash[5], hash[6], hash[7],
					hash[8], hash[9], hash[10], hash[11],
					hash[12], hash[13], hash[14], hash[15]);
				md5.Hash((uint8_t*)scratchBuffer, scratchLength, hash);
			}
		};

	int64_t answer = FindIndexOfNthKey(salt, keysNeeded, stretchHash);

	printf("[2016] Puzzle14_B: %" PRId64 "\n", answer);
}

void Puzzle14_2016(const string& filename)
{
	Puzzle14_A(filename);
	Puzzle14_B(filename);
}
