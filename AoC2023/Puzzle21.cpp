#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2023_Types
{
}

using namespace Puzzle21_2023_Types;

static int64_t CountOddsFromFinite(const Point2& start, int64_t countStart, int64_t countLimit, const PointMap& garden)
{
	const vector<Point2> neighbourOffsets =
	{
		Point2{  0, -1 },
		Point2{ -1,  0 },
		Point2{  1,  0 },
		Point2{  0,  1 },
	};

	map<Point2, int64_t> stepDistance;

	vector<pair<Point2, int64_t>> searchQueue = { { start, countStart } };
	set<Point2> addedToQueue = { start };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		pair<Point2, int64_t> current = searchQueue[i];
		if (current.second > countLimit)
			break;

		if (stepDistance.contains(current.first))
			continue;

		stepDistance[current.first] = current.second;

		for (const Point2& offset : neighbourOffsets)
		{
			Point2 neighbour = current.first + offset;
			if (garden.Data.contains(neighbour) && (addedToQueue.contains(neighbour) == false) && (stepDistance.contains(neighbour) == false))
			{
				searchQueue.push_back({ neighbour, current.second + 1 });
				addedToQueue.insert(neighbour);
			}
		}
	}

	int64_t answer = count_if(stepDistance.begin(), stepDistance.end(),
		[](map<Point2, int64_t>::const_reference tile)
		{
			return (tile.second % 2) == 1;
		});

	return answer;
}

static void Puzzle21_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	PointMap garden = ReadPointMap(input, '#');
	map<Point2, char>::const_iterator startIt = find_if(garden.Data.begin(), garden.Data.end(),
		[](map<Point2, char>::const_reference& mapSquare)
		{
			return mapSquare.second == 'S';
		});
	assert(startIt != garden.Data.end());
	Point2 start = startIt->first;

	const vector<Point2> neighbourOffsets =
	{
		Point2{  0, -1 },
		Point2{ -1,  0 },
		Point2{  1,  0 },
		Point2{  0,  1 },
	};

	map<Point2, int> tileColours;
	tileColours[start] = 0;

	vector<pair<Point2, int>> searchQueue = { { start, 0 } };
	set<Point2> addedToQueue = { start };
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		pair<Point2, int> current = searchQueue[i];
		if (current.second > 64)
			break;

		int currentColour = current.second % 2;
		tileColours[current.first] = currentColour;

		for (const Point2& offset : neighbourOffsets)
		{
			Point2 neighbour = current.first + offset;
			if (garden.Data.contains(neighbour) && (addedToQueue.contains(neighbour) == false) && (tileColours.contains(neighbour) == false))
			{
				searchQueue.push_back({ neighbour, current.second + 1 });
				addedToQueue.insert(neighbour);
			}
		}
	}

	int64_t answer = count_if(tileColours.begin(), tileColours.end(),
		[](map<Point2, int>::const_reference tile)
		{
			return tile.second == 0;
		});

	printf("[2023] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int maxSteps = 26501365;

	PointMap smallMap = ReadPointMap(input, '#');

	const int64_t offsetX = ((maxSteps / smallMap.Size.X) * smallMap.Size.X) + smallMap.Size.X;

	auto getStart = [](const PointMap& garden) -> Point2
		{
			map<Point2, char>::const_iterator startIt = find_if(garden.Data.begin(), garden.Data.end(),
				[](map<Point2, char>::const_reference& mapSquare)
				{
					return mapSquare.second == 'S';
				});
			assert(startIt != garden.Data.end());
			return startIt->first;
		};

	Point2 finiteMapStart = getStart(smallMap);
	Point2 infiniteMapStart = finiteMapStart + Point2{ offsetX, offsetX };

	// Centre = white tile

	int64_t whiteTileCount = CountOddsFromFinite(finiteMapStart, 0, 270, smallMap);
	int64_t blackTileCount = CountOddsFromFinite(finiteMapStart + Point2{ 1, 0 }, 0, 270, smallMap);

	int64_t spokeCompleteTiles = (maxSteps - 130) / 131;
	int64_t spokeWhiteTiles = spokeCompleteTiles / 2;
	int64_t spokeBlackTiles = spokeCompleteTiles - spokeWhiteTiles;

	auto sumOfTerms = [](int64_t terms) { return (terms * (terms + 1)) / 2; };
	int64_t totalCompleteDiagonals = sumOfTerms(spokeCompleteTiles - 1);

	int64_t diagonalWhiteTiles = 0;
	int64_t diagonalBlackTiles = 0;
	for (int diagonal = 1; diagonal < spokeCompleteTiles; diagonal++)
	{
		if ((diagonal % 2) == 1)
		{
			diagonalWhiteTiles += diagonal;
		}
		else
		{
			diagonalBlackTiles += diagonal;
		}
	}
	assert(totalCompleteDiagonals == (diagonalWhiteTiles + diagonalBlackTiles));
	(void)totalCompleteDiagonals;

	int64_t numCompleteWhiteTiles = 1 + (4 * spokeWhiteTiles) + (4 * diagonalWhiteTiles);
	int64_t numCompleteBlackTiles = (4 * spokeBlackTiles) + (4 * diagonalBlackTiles);

	int64_t wholeLowerBound = (whiteTileCount * numCompleteWhiteTiles) + (blackTileCount * numCompleteBlackTiles);

	int64_t spokeStartCount = (spokeCompleteTiles * 131) + 66;
	int64_t diagonalStartCount = ((spokeCompleteTiles - 1) * 131) + 132;

	Point2 northStartPoint{ finiteMapStart.X, smallMap.Size.Y - 1 };
	Point2 southStartPoint{ finiteMapStart.X, 0 };
	Point2 eastStartPoint{ 0, finiteMapStart.Y };
	Point2 westStartPoint{ smallMap.Size.X - 1, finiteMapStart.Y };

	int64_t northCount = CountOddsFromFinite(northStartPoint, spokeStartCount, maxSteps, smallMap);
	int64_t southCount = CountOddsFromFinite(southStartPoint, spokeStartCount, maxSteps, smallMap);
	int64_t eastCount = CountOddsFromFinite(eastStartPoint, spokeStartCount, maxSteps, smallMap);
	int64_t westCount = CountOddsFromFinite(westStartPoint, spokeStartCount, maxSteps, smallMap);

	int64_t tighterLowerBound = wholeLowerBound + northCount + southCount + eastCount + westCount;

	Point2 northEastStartPoint{ 0, smallMap.Size.Y - 1 };
	Point2 southEastStartPoint{ 0, 0 };
	Point2 northWestStartPoint{ smallMap.Size.X - 1, smallMap.Size.Y - 1 };
	Point2 southWestStartPoint{ smallMap.Size.X - 1, 0 };

	int64_t northEastCount = CountOddsFromFinite(northEastStartPoint, diagonalStartCount, maxSteps, smallMap);
	int64_t southEastCount = CountOddsFromFinite(southEastStartPoint, diagonalStartCount, maxSteps, smallMap);
	int64_t northWestCount = CountOddsFromFinite(northWestStartPoint, diagonalStartCount, maxSteps, smallMap);
	int64_t southWestCount = CountOddsFromFinite(southWestStartPoint, diagonalStartCount, maxSteps, smallMap);

	int64_t bigDiagonalContribution = (northEastCount + southEastCount + northWestCount + southWestCount) * spokeCompleteTiles;

	int64_t smallDiagonalStartCount = diagonalStartCount + 131;

	int64_t smallNorthEastCount = CountOddsFromFinite(northEastStartPoint, smallDiagonalStartCount, maxSteps, smallMap);
	int64_t smallSouthEastCount = CountOddsFromFinite(southEastStartPoint, smallDiagonalStartCount, maxSteps, smallMap);
	int64_t smallNorthWestCount = CountOddsFromFinite(northWestStartPoint, smallDiagonalStartCount, maxSteps, smallMap);
	int64_t smallSouthWestCount = CountOddsFromFinite(southWestStartPoint, smallDiagonalStartCount, maxSteps, smallMap);

	int64_t smallDiagonalContribution = (smallNorthEastCount + smallSouthEastCount + smallNorthWestCount + smallSouthWestCount) * (spokeCompleteTiles + 1);

	int64_t answer = tighterLowerBound + bigDiagonalContribution + smallDiagonalContribution;

	printf("[2023] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2023(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
