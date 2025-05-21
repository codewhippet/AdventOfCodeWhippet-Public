#include "stdafx.h"
#include <Windows.h>
#include <bcrypt.h>

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2015_Types
{
}

using namespace Puzzle04_2015_Types;

static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secretKey;
	getline(input, secretKey);

	MD5 md5;

	int64_t answer = 0;
	while (true)
	{
		char buffer[64];
		int secretLength = sprintf(buffer, "%s%" PRId64 "", secretKey.c_str(), answer);

		unsigned char hash[16];
		md5.Hash((uint8_t*)buffer, secretLength, hash);

		if (((hash[0] & 0xff) == 0) &&
			((hash[1] & 0xff) == 0) &&
			((hash[2] & 0xf0) == 0))
		{
			break;
		}

		answer++;
	}

	printf("[2015] Puzzle04_A: %" PRId64 "\n", answer);
}


static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string secretKey;
	getline(input, secretKey);

	MD5 md5;

	int64_t answer = 0;
	while (true)
	{
		char buffer[64];
		int secretLength = sprintf(buffer, "%s%" PRId64 "", secretKey.c_str(), answer);

		unsigned char hash[16];
		md5.Hash((uint8_t*)buffer, secretLength, hash);

		if (((hash[0] & 0xff) == 0) &&
			((hash[1] & 0xff) == 0) &&
			((hash[2] & 0xff) == 0))
		{
			break;
		}

		answer++;
	}

	printf("[2015] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2015(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
