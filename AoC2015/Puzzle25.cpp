#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle25_2015_Types
{
}

using namespace Puzzle25_2015_Types;

static int64_t GridIndexFromRowColumn(int64_t row, int64_t column)
{
	// Get the 'starting' position [r0,1] of the diagonal containing [row,column]
	int64_t r0 = row + (column - 1);

	// The starting grid index for each row [r0,1] is TriangleNumber(r0-1) + 1
	int64_t triangleNumber = ((r0 - 1) * (r0)) / 2;
	int64_t startingGridIndex = triangleNumber + 1;

	// Then add back on the row count we took away
	int64_t gridIndex = startingGridIndex + (r0 - row);
	return gridIndex;
}

static void Puzzle25_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string instructions;
	getline(input, instructions);

	int64_t row = 0;
	int64_t column = 0;
	int scanned = sscanf(instructions.c_str(), "To continue, please consult the code grid in the manual.  Enter the code at row %lld, column %lld.",
		&row,
		&column);
	assert(scanned == 2);
	(void)scanned;

	int64_t rngIterations = GridIndexFromRowColumn(row, column);

	int64_t answer = 20151125;
	for (int64_t i = 1; i < rngIterations; i++)
	{
		answer = (answer * 252533) % 33554393;
	}

	printf("[2015] Puzzle25_A: %" PRId64 "\n", answer);
}

void Puzzle25_2015(const string& filename)
{
	Puzzle25_A(filename);
}
