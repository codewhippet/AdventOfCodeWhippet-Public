#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle01_2017_Types
{
}

using namespace Puzzle01_2017_Types;

static void Puzzle01_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string captchaLine = ReadSingleLine(input);

	int64_t answer = 0;
	for (size_t i = 0; i < captchaLine.size(); i++)
	{
		size_t j = (i + 1) % captchaLine.size();
		if (captchaLine[i] == captchaLine[j])
		{
			answer += captchaLine[i] - '0';
		}
	}

	printf("[2017] Puzzle01_A: %" PRId64 "\n", answer);
}


static void Puzzle01_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string captchaLine = ReadSingleLine(input);

	int64_t answer = 0;
	for (size_t i = 0; i < captchaLine.size(); i++)
	{
		size_t j = (i + (captchaLine.size() / 2)) % captchaLine.size();
		if (captchaLine[i] == captchaLine[j])
		{
			answer += captchaLine[i] - '0';
		}
	}

	printf("[2017] Puzzle01_B: %" PRId64 "\n", answer);
}

void Puzzle01_2017(const string& filename)
{
	Puzzle01_A(filename);
	Puzzle01_B(filename);
}
