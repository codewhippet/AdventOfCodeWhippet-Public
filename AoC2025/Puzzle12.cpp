#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2025_Types
{
}

using namespace Puzzle12_2025_Types;

static int TileCount(istream& input)
{
	string line;
	getline(input, line);
	return (int)ranges::count(line, '#');
}

static void Puzzle12_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const size_t numPieces = 6;

	string line;
	vector<int> pieceTileCounts(numPieces);
	for (size_t i = 0; i < numPieces; i++)
	{
		getline(input, line);
		pieceTileCounts[i] += TileCount(input);
		pieceTileCounts[i] += TileCount(input);
		pieceTileCounts[i] += TileCount(input);
		getline(input, line);
	}

	int64_t answer = 0;
	while (getline(input, line))
	{
		int width, height;
		vector<int> pieceCounts(numPieces);

		int parsed = sscanf(line.c_str(), "%dx%d: %d %d %d %d %d %d",
			&width,
			&height,
			&pieceCounts[0],
			&pieceCounts[1],
			&pieceCounts[2],
			&pieceCounts[3],
			&pieceCounts[4],
			&pieceCounts[5]);
		assert(parsed == 8);
		(void)parsed;

		int numPiecesToPlace = ranges::fold_left(pieceCounts, 0, plus{});
		int loosePackedPieceCount = (width / 3) * (height / 3);
		bool trivialAccept = loosePackedPieceCount >= numPiecesToPlace;

		int numTilesToPlace = ranges::fold_left(views::zip_transform(multiplies{}, pieceTileCounts, pieceCounts), 0, plus{});
		int numTilesAvailable = width * height;
		bool trivialReject = numTilesToPlace > numTilesAvailable;

		assert(trivialAccept || trivialReject);
		assert(trivialAccept != trivialReject);

		answer += trivialAccept;
	}

	printf("[2025] Puzzle12_A: %" PRId64 "\n", answer);
}

void Puzzle12_2025(const string& filename)
{
	Puzzle12_A(filename);
}
