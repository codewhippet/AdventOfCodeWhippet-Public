#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2016_Types
{
}

using namespace Puzzle16_2016_Types;

static vector<bool> ParseInput(istream &input)
{
	string startingStateDescription;
	getline(input, startingStateDescription);

	vector<bool> startingState;
	ranges::copy(startingStateDescription
		| views::transform([](char c)
			{
				return c == '1';
			}),
			back_inserter(startingState));

	return startingState;
}

static vector<bool> Transform(const vector<bool>& oldState)
{
	vector<bool> newState = oldState;
	newState.resize(newState.size() + 1 + newState.size());
	for (size_t i = 0; i < newState.size() / 2; i++)
	{
		newState[(newState.size() - 1) - i] = !newState[i];
	}
	return newState;
}

static vector<bool> ChecksumStep(const vector<bool>& oldState)
{
	assert((oldState.size() % 2) == 0);

	vector<bool> newState(oldState.size() / 2);
	for (size_t i = 0; i < oldState.size(); i += 2)
	{
		newState[i / 2] = (oldState[i + 0] == oldState[i + 1]);
	}
	return newState;
}

static string Checksum(const vector<bool>& state)
{
	vector<bool> checksum = state;
	do
	{
		checksum = ChecksumStep(checksum);

	} while ((checksum.size() % 2) == 0);

	string checksumString;
	ranges::copy(checksum
		| views::transform(
			[](bool b)
			{
				return (char)('0' + b);
			}),
		back_inserter(checksumString));
	return checksumString;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t minimumLength = 272;

	vector<bool> state = ParseInput(input);
	while (state.size() < minimumLength)
	{
		state = Transform(state);
	}
	state.resize(minimumLength);

	string answer = Checksum(state);

	printf("[2016] Puzzle16_A: %s\n", answer.c_str());
}


static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t minimumLength = 35651584;

	vector<bool> state = ParseInput(input);
	while (state.size() < minimumLength)
	{
		state = Transform(state);
	}
	state.resize(minimumLength);

	string answer = Checksum(state);

	printf("[2016] Puzzle16_B: %s\n", answer.c_str());
}

void Puzzle16_2016(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
