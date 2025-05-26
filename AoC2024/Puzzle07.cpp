#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2024_Types
{
}

using namespace Puzzle07_2024_Types;

static bool CanBeSolved(int64_t target, const vector<int64_t>& constituents, size_t endAt)
{
	assert(endAt > 0);
	int64_t lastNumber = constituents[endAt - 1];
	if (endAt == 1)
	{
		return lastNumber == target;
	}
	else
	{
		bool divisibleByLastNumber = (target % lastNumber) == 0;
		if (divisibleByLastNumber && CanBeSolved(target / lastNumber, constituents, endAt - 1))
		{
			return true;
		}
		return CanBeSolved(target - lastNumber, constituents, endAt - 1);
	}
}

static bool CanBeSolved(int64_t target, const vector<int64_t>& constituents)
{
	return CanBeSolved(target, constituents, constituents.size());
}

static int64_t Concat(int64_t hi, int64_t lo)
{
	int64_t loCopy = lo;
	while (loCopy > 0)
	{
		hi *= 10;
		loCopy /= 10;
	}
	return hi + lo;
}

static bool CanBeSolvedWithConcatenation(int64_t target, int64_t runningTotal, const vector<int64_t>& constituents, size_t startAt)
{
	if (runningTotal > target)
	{
		return false;
	}

	if (startAt == constituents.size())
	{
		return target == runningTotal;
	}

	return
		CanBeSolvedWithConcatenation(target, Concat(runningTotal, constituents[startAt]), constituents, startAt + 1) ||
		CanBeSolvedWithConcatenation(target, runningTotal * constituents[startAt], constituents, startAt + 1) ||
		CanBeSolvedWithConcatenation(target, runningTotal + constituents[startAt], constituents, startAt + 1);
}

static bool CanBeSolvedWithConcatenation(int64_t target, const vector<int64_t>& constituents)
{
	return CanBeSolvedWithConcatenation(target, constituents[0], constituents, 1);
}

static void Puzzle07_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	for (const string& line : ReadEachLine(input))
	{
		int64_t target;
		char buffer[256] = { 0 };
		int captured = sscanf(line.c_str(), "%lld: %255[0-9 ]", &target, buffer);
		assert(captured == 2);
		(void)captured;

		if (CanBeSolved(target, ReadAsVectorOfNumbers(buffer)))
		{
			answer += target;
		}
	}

	printf("[2024] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	assert(Concat(12, 34) == 1234);
	assert(Concat(123, 456) == 123456);
	assert(Concat(1234, 5678) == 12345678);
	assert(Concat(1, 2345) == 12345);
	assert(Concat(1234, 5) == 12345);

	vector<int64_t> c{ 6, 8, 6, 15 };
	assert(CanBeSolvedWithConcatenation(7290, c));

	for (const string& line : ReadEachLine(input))
	{
		int64_t target;
		char buffer[256] = { 0 };
		int captured = sscanf(line.c_str(), "%lld: %255[0-9 ]", &target, buffer);
		assert(captured == 2);
		(void)captured;

		vector<int64_t> constituents = ReadAsVectorOfNumbers(buffer);
		if (CanBeSolved(target, constituents))
		{
			//printf("Can be solved: %lld\n", target);
			answer += target;
		}
		else if (CanBeSolvedWithConcatenation(target, constituents))
		{
			//printf("Can be solved with concatenation: %lld\n", target);
			answer += target;
		}
	}

	printf("[2024] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2024(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
