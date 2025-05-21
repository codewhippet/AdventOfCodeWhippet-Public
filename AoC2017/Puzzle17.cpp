#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2017_Types
{
}

using namespace Puzzle17_2017_Types;

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t stepAmount = atoll(ReadSingleLine(input).c_str());

	vector<int64_t> buffer;
	buffer.push_back(0);

	size_t currentPosition = 0;
	for (int64_t i = 1; i <= 2017; i++)
	{
		size_t nextPosition = (currentPosition + stepAmount) % buffer.size();
		vector<int64_t>::iterator insertPosition = buffer.begin() + nextPosition + 1;
		vector<int64_t>::iterator insertedAt = buffer.insert(insertPosition, i);
		currentPosition = insertedAt - buffer.begin();
	}

	int64_t answer = buffer[(currentPosition + 1) % buffer.size()];

	printf("[2017] Puzzle17_A: %" PRId64 "\n", answer);
}


static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t stepAmount = atoll(ReadSingleLine(input).c_str());

	size_t positionOfZero = 0;
	size_t bufferSize = 1;
	int64_t numberAfterZero = -1;

	size_t currentPosition = 0;
	for (int64_t i = 1; i <= 50000000; i++)
	{
		size_t nextPosition = (currentPosition + stepAmount) % bufferSize;
		if (nextPosition < positionOfZero)
		{
			positionOfZero++;
		}
		else if (nextPosition == positionOfZero)
		{
			numberAfterZero = i;
		}

		bufferSize++;
		currentPosition = nextPosition + 1;
	}

	int64_t answer = numberAfterZero;

	printf("[2017] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2017(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
