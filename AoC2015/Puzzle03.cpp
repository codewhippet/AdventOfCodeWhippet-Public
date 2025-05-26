#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2015_Types
{
}

using namespace Puzzle03_2015_Types;

static void Puzzle03_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Point2 santa;
	set<Point2> visited;
	for (const string& line : ReadEachLine(input))
	{
		visited.insert(santa);
		for (char c : line)
		{
			santa = santa + Point2::DirectionFromChar().at(c);
			visited.insert(santa);
		}
	}

	int64_t answer = visited.size();

	printf("[2015] Puzzle03_A: %" PRId64 "\n", answer);
}


static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Point2> santas = { {}, {} };
	set<Point2> visited;
	for (const string& line : ReadEachLine(input))
	{
		visited.insert(santas[0]);
		for (size_t i = 0; i < line.size(); i++)
		{
			size_t santa = i % 2;
			santas[santa] = santas[santa] + Point2::DirectionFromChar().at(line[i]);
			visited.insert(santas[santa]);
		}
	}

	int64_t answer = visited.size();

	printf("[2015] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2015(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
