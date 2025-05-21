#include "stdafx.h"
#include <charconv>

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2016_Types
{
}

using namespace Puzzle05_2016_Types;

static void Puzzle05_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secretKey;
	getline(input, secretKey);

	MD5 md5;

	string answer;
	int64_t counter = 0;
	while (answer.size() < 8)
	{
		char buffer[64];
		int secretLength = sprintf(buffer, "%s%" PRId64 "", secretKey.c_str(), counter);

		unsigned char hash[16];
		md5.Hash((uint8_t*)buffer, secretLength, hash);

		if (((hash[0] & 0xff) == 0) &&
			((hash[1] & 0xff) == 0) &&
			((hash[2] & 0xf0) == 0))
		{
			char hex[2];
			to_chars(&hex[0], &hex[1], hash[2] & 0x0f, 16);
			answer += hex[0];
		}

		counter++;
	}

	printf("[2016] Puzzle05_A: %s\n", answer.c_str());
}


static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secretKey;
	getline(input, secretKey);

	MD5 md5;

	string answer = "........";
	int64_t counter = 0;
	while (answer.find('.') != string::npos)
	{
		char buffer[64];
		int secretLength = sprintf(buffer, "%s%" PRId64 "", secretKey.c_str(), counter);

		unsigned char hash[16];
		md5.Hash((uint8_t*)buffer, secretLength, hash);

		if (((hash[0] & 0xff) == 0) &&
			((hash[1] & 0xff) == 0) &&
			((hash[2] & 0xf0) == 0))
		{
			size_t index = hash[2] & 0x0f;
			if ((index < answer.length()) && (answer[index] == '.'))
			{
				char hex[2];
				to_chars(&hex[0], &hex[1], (hash[3] & 0xf0) >> 4, 16);
				answer[index] = hex[0];
			}
		}

		counter++;
	}

	printf("[2016] Puzzle05_B: %s\n", answer.c_str());
}

void Puzzle05_2016(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
