#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2020_Types
{
}

using namespace Puzzle25_2020_Types;

static size_t FindLoopSize(size_t subjectNumber, size_t targetNumber)
{
	size_t workingValue = 1;
	for (size_t loopSize = 0; loopSize < 20201227; loopSize++)
	{
		workingValue = (workingValue * subjectNumber) % 20201227;
		if (workingValue == targetNumber)
		{
			return loopSize + 1;
		}
	}

	return 0;
}

static size_t Transform(size_t subjectNumber, size_t loopSize)
{
	size_t workingValue = 1;
	for (size_t i = 0; i < loopSize; i++)
	{
		workingValue = (workingValue * subjectNumber) % 20201227;
	}

	return workingValue;
}

static void Puzzle25_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t cardPublicKey = atoll(ReadSingleLine(input).c_str());
	const size_t doorPublicKey = atoll(ReadSingleLine(input).c_str());

	const size_t cardLoopSize = FindLoopSize(7, cardPublicKey);
	const size_t doorLoopSize = FindLoopSize(7, doorPublicKey);

	assert(Transform(7, cardLoopSize) == cardPublicKey);
	assert(Transform(7, doorLoopSize) == doorPublicKey);

	const size_t cardTransform = Transform(doorPublicKey, cardLoopSize);
	const size_t doorTransform = Transform(cardPublicKey, doorLoopSize);

	assert(cardTransform == doorTransform);
	(void)doorTransform;

	const int64_t answer = cardTransform;

	printf("[2020] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2020(const string& filename)
{
	Puzzle25_A(filename);
}
