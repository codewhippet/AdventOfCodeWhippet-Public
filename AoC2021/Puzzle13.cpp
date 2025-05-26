#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2021_Types
{
	struct Fold
	{
		char Axis;
		int Line;
	};
}

using namespace Puzzle13_2021_Types;

static void ReadPuzzle(istream& input, set<Point2>* points, vector<Fold>* folds)
{
	regex dotReg(R"((\d+),(\d+))");
	regex foldReg(R"(fold along (\w)=(\d+))");

	vector<string> lines = ReadAllLines(input);
	for (const string& line : lines)
	{
		smatch dotMatch;
		if (regex_match(line, dotMatch, dotReg))
		{
			points->emplace(Point2{ stoll(dotMatch[1].str()), stoll(dotMatch[2].str()) });
			continue;
		}

		smatch foldMatch;
		if (regex_match(line, foldMatch, foldReg))
		{
			folds->emplace_back(Fold{ foldMatch[1].str()[0], stoi(foldMatch[2].str()) });
			continue;
		}
	}
}

static void PrintPuzzle(const set<Point2>& points)
{
	int maxX = MakeEnumerator(points)->Select<int>([](const Point2& p) { return (int)p.X; })->Max() + 1;
	int maxY = MakeEnumerator(points)->Select<int>([](const Point2& p) { return (int)p.Y; })->Max() + 1;

	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			printf("%c", points.contains(Point2{ x, y }) ? '#' : '.');
		}
		printf("\n");
	}
}

static Point2 FoldPoint(Point2 original, Fold instruction)
{
	if (instruction.Axis == 'x')
	{
		if (original.X < instruction.Line)
		{
			return original;
		}

		int64_t distanceFromFold = original.X - instruction.Line;
		return Point2{ instruction.Line - distanceFromFold, original.Y };
	}

	if (instruction.Axis == 'y')
	{
		if (original.Y < instruction.Line)
		{
			return original;
		}

		int64_t distanceFromFold = original.Y - instruction.Line;
		return Point2{ original.X, instruction.Line - distanceFromFold };
	}

	return Point2{ -1, -1 };
}

static void Puzzle13_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<Point2> dots;
	vector<Fold> instructions;
	ReadPuzzle(input, &dots, &instructions);

	const Fold& f = instructions.front();

	set<Point2> newDots = MakeEnumerator(dots)
		->Select<Point2>([&f](const Point2& p) { return FoldPoint(p, f); })
		->ToSet();

	dots = move(newDots);

	int64_t answer = (int64_t)dots.size();

	printf("[2021] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	set<Point2> dots;
	vector<Fold> instructions;
	ReadPuzzle(input, &dots, &instructions);

	for (const Fold& f : instructions)
	{
		set<Point2> newDots = MakeEnumerator(dots)
			->Select<Point2>([&f](const Point2& p) { return FoldPoint(p, f); })
			->ToSet();

		dots = move(newDots);
	}

	printf("[2021] Puzzle13_B:\n");
	PrintPuzzle(dots);
}

void Puzzle13_2021(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
