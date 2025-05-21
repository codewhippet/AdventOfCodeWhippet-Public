#include "stdafx.h"

using namespace std;
using namespace HexFlat;

static string dummy =
R"()";

namespace Puzzle11_2017_Types
{
}

using namespace Puzzle11_2017_Types;

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string line = ReadSingleLine(input);

	static const map<string, Hex> hexIndices =
	{
		{ "n", Hex::North() },
		{ "ne", Hex::NorthEast() },
		{ "se", Hex::SouthEast() },
		{ "s", Hex::South() },
		{ "sw", Hex::SouthWest() },
		{ "nw", Hex::NorthWest() }
	};

	Hex currentPosition;

	char* token = strtok(line.data(), ", ");
	while (token)
	{
		currentPosition = currentPosition + hexIndices.at(token);
		token = strtok(nullptr, ", ");
	}

	int64_t answer = MahattanDistance(currentPosition, Hex{});

	printf("[2017] Puzzle11_A: %" PRId64 "\n", answer);
}


static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);
	
	string line = ReadSingleLine(input);

	static const map<string, Hex> hexIndices =
	{
		{ "n", Hex::North() },
		{ "ne", Hex::NorthEast() },
		{ "se", Hex::SouthEast() },
		{ "s", Hex::South() },
		{ "sw", Hex::SouthWest() },
		{ "nw", Hex::NorthWest() }
	};

	Hex currentPosition;

	int64_t answer = 0;
	char* token = strtok(line.data(), ", ");
	while (token)
	{
		currentPosition = currentPosition + hexIndices.at(token);
		token = strtok(nullptr, ", ");
		answer = max(answer, MahattanDistance(currentPosition, Hex{}));
	}

	printf("[2017] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2017(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
