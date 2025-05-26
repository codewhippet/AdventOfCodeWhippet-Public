#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2015_Types
{
}

using namespace Puzzle08_2015_Types;

static int64_t SizeInBytes(const char* s)
{
	int64_t size = 0;

	assert(s[0] == '"');
	s++;

	while (*s != '"')
	{
		if (s[0] == '\\')
		{
			switch (s[1])
			{
			case '\\':
			case '"':
				s += 2;
				break;

			case 'x':
				s += 4;
				break;
			}
		}
		else
		{
			s++;
		}

		size++;
	}

	return size;
}

static int64_t EncodedSize(const string &s)
{
	int64_t size = 2;

	for (char c : s)
	{
		switch (c)
		{
		case '"':
		case '\\':
			size += 2;
			break;

		default:
			size += 1;
			break;
		}
	}

	return size;
}

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& s : ReadEachLine(input))
	{
		int64_t sizeInBytes = SizeInBytes(s.c_str());
		answer += s.size() - sizeInBytes;
	}

	printf("[2015] Puzzle08_A: %" PRId64 "\n", answer);
}


static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;
	for (const string& s : ReadEachLine(input))
	{
		int64_t encodedSize = EncodedSize(s);
		answer += encodedSize - s.size();
	}

	printf("[2015] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2015(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
