#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2023_Types
{
	struct Instruction
	{
		char Direction;
		int Distance;

		char Direction2;
		int Distance2;
	};

	struct Transform
	{
		map<int64_t, int64_t> ToVirtualX;
		map<int64_t, int64_t> ToRealX;

		map<int64_t, int64_t> ToVirtualY;
		map<int64_t, int64_t> ToRealY;
	};

	static map<char, Point2> Directions =
	{
		{ 'U', Point2{  0, -1 } },
		{ 'D', Point2{  0,  1 } },
		{ 'L', Point2{ -1,  0 } },
		{ 'R', Point2{  1,  0 } },

		{ '0', Point2{  1,  0 } },
		{ '1', Point2{  0,  1 } },
		{ '2', Point2{ -1,  0 } },
		{ '3', Point2{  0, -1 } },
	};

	static map<string, Point2> FillOffset =
	{
		{ "UL", Point2{ -1, 1 } },
		{ "LD", Point2{ 1, 1 } },
		{ "DL", Point2{ 1, -1 } },
		{ "LU", Point2{ -1, -1 } },

		{ "UR", Point2{ 1, 1 } },
		{ "RD", Point2{ -1, 1 } },
		{ "DR", Point2{ -1, -1 } },
		{ "RU", Point2{ 1, -1 } },
	};
}

using namespace Puzzle18_2023_Types;

static void FloodFill(const Point2& start, set<Point2>* canvas, int64_t debugMaxPixels)
{
	vector<Point2> fillPositions{ start };
	for (size_t i = 0; i < fillPositions.size(); i++)
	{
		assert((int64_t)canvas->size() <= debugMaxPixels);
		(void)debugMaxPixels;

		Point2 fillPosition = fillPositions[i];
		if (canvas->contains(fillPosition))
			continue;

		int64_t leftExtent = fillPosition.X;
		while (canvas->contains({ leftExtent, fillPosition.Y }) == false)
			leftExtent--;

		int64_t rightExtent = fillPosition.X;
		while (canvas->contains({ rightExtent, fillPosition.Y }) == false)
		{
			rightExtent++;
		}

		for (int64_t x = leftExtent; x <= rightExtent; x++)
		{
			canvas->insert({ x, fillPosition.Y });
			if (canvas->contains({ x, fillPosition.Y - 1 }) == false)
			{
				fillPositions.push_back({ x, fillPosition.Y - 1 });
			}
			if (canvas->contains({ x, fillPosition.Y + 1 }) == false)
			{
				fillPositions.push_back({ x, fillPosition.Y + 1 });
			}
		}
	}
}

static Transform MakeTranslations(const set<int64_t>& xCoords, const set<int64_t>& yCoords)
{
	Transform t;

	{
		int64_t index = 0;
		for (int64_t coord : xCoords)
		{
			t.ToVirtualX.insert({ coord, index });
			t.ToRealX.insert({ index, coord });
			index++;
		}
	}

	{
		int64_t index = 0;
		for (int64_t coord : yCoords)
		{
			t.ToVirtualY.insert({ coord, index });
			t.ToRealY.insert({ index, coord });
			index++;
		}
	}

	return t;
};

static Point2 TransformToVirtual(const Point2& p, const Transform& t)
{
	assert(t.ToVirtualX.contains(p.X) && t.ToVirtualY.contains(p.Y));
	return Point2{ t.ToVirtualX.at(p.X), t.ToVirtualY.at(p.Y) };
}

static Point2 TransformToReal(const Point2& p, const Transform& t)
{
	assert(t.ToRealX.contains(p.X) && t.ToRealY.contains(p.Y));
	return Point2{ t.ToRealX.at(p.X), t.ToRealY.at(p.Y) };
}

static void Puzzle18_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> instructions = MakeEnumerator(ReadAllLines(input))
		->Select<Instruction>([](const string& s)
			{
				Instruction inst;
				int scanned = sscanf(s.c_str(), "%c %d (#%5x%c)",
					&inst.Direction,
					&inst.Distance,
					&inst.Distance2,
					&inst.Direction2);
				assert(scanned == 4);
				(void)scanned;
				return inst;
			})
		->ToVector();

	const int mapScale = 1;

	int64_t minX = 0;
	int64_t maxX = 0;
	int64_t minY = 0;
	int64_t maxY = 0;

	Point2 current{ 0, 0 };
	set<Point2> hole = { current };
	for (const Instruction& inst : instructions)
	{
		Point2 direction = Directions[inst.Direction];
		for (int i = 0; i < inst.Distance * mapScale; i++)
		{
			current = current + direction;
			hole.insert(current);

			minX = min(minX, current.X);
			maxX = max(maxX, current.X);

			minY = min(minY, current.Y);
			maxY = max(maxY, current.Y);
		}
	}

	Point2 fillFrom{ 1, 1 }; // Cheat!
	FloodFill(fillFrom, &hole, (maxX - minX + 1) * (maxY - minY + 1));

	int64_t answer = 0;
	for (int64_t y = minY; y <= maxY; y += mapScale)
	{
		for (int64_t x = minX; x <= maxX; x += mapScale)
		{
			if (hole.contains({ x, y }))
			{
				answer++;
			}
		}
	}

	printf("[2023] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Instruction> instructions = MakeEnumerator(ReadAllLines(input))
		->Select<Instruction>([](const string& s)
			{
				Instruction inst;
				int scanned = sscanf(s.c_str(), "%c %d (#%5x%c)",
					&inst.Direction,
					&inst.Distance,
					&inst.Distance2,
					&inst.Direction2);
				assert(scanned == 4);
				(void)scanned;
				return inst;
			})
		->ToVector();

	const int mapScale = 1;

	set<int64_t> xCoords = { 0, 1 };
	set<int64_t> yCoords = { 0, 1 };

	{
		Point2 current{ 0, 0 };
		for (const Instruction& inst : instructions)
		{
			Point2 direction = Directions[inst.Direction2];
			int distance = inst.Distance2;

			current = current + (direction * distance * mapScale);

			xCoords.insert(current.X);
			xCoords.insert(current.X + 1);

			yCoords.insert(current.Y);
			yCoords.insert(current.Y + 1);
		}
	}

	Transform transform = MakeTranslations(xCoords, yCoords);

	int64_t minX = 0;
	int64_t maxX = 0;
	int64_t minY = 0;
	int64_t maxY = 0;

	set<Point2> hole = { TransformToVirtual({ 0, 0 }, transform) };

	{
		Point2 currentReal{ 0, 0 };
		for (const Instruction& inst : instructions)
		{
			Point2 direction = Directions[inst.Direction2];
			int distance = inst.Distance2;

			Point2 nextReal = currentReal + (direction * distance * mapScale);

			Point2 start = TransformToVirtual(currentReal, transform);
			Point2 stop = TransformToVirtual(nextReal, transform);

			vector<Point2> edge = Enumerable::Line(start, stop)->ToVector();
			for (const Point2& p : edge)
			{
				hole.insert(p);

				minX = min(minX, p.X);
				maxX = max(maxX, p.X);

				minY = min(minY, p.Y);
				maxY = max(maxY, p.Y);
			}

			currentReal = nextReal;
		}
	}

	Point2 fillFrom = TransformToVirtual({ 1, 1 }, transform); // Cheat!
	FloodFill(fillFrom, &hole, (maxX - minX + 1) * (maxY - minY + 1));

	int64_t answer = 0;
	for (int64_t y = minY; y <= maxY; y += mapScale)
	{
		for (int64_t x = minX; x <= maxX; x += mapScale)
		{
			if (hole.contains({ x, y }))
			{
				Point2 realTopLeft = TransformToReal({ x, y }, transform);
				Point2 realBottomRight = TransformToReal({ x + 1, y + 1 }, transform);
				int64_t area = (realBottomRight.X - realTopLeft.X) * (realBottomRight.Y - realTopLeft.Y);
				answer += area;
			}
		}
	}

	printf("[2023] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2023(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
