#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2020_Types
{
	struct Ferry
	{
		Point2 Position;
		Point2 Heading;
	};

	struct Ferry2
	{
		Point2 Position;
		Point2 Waypoint;
	};
}

using namespace Puzzle12_2020_Types;

static map<string, function<void(Ferry*)>> MakeMovements()
{
	map<string, function<void(Ferry *)>> movements;

	movements["R90"] = [](Ferry* f) { f->Heading = Point2{ f->Heading.Y, -f->Heading.X }; };
	movements["R180"] = [](Ferry* f) { f->Heading = Point2{ -f->Heading.X, -f->Heading.Y }; };
	movements["R270"] = [](Ferry* f) { f->Heading = Point2{ -f->Heading.Y, f->Heading.X }; };

	movements["L90"] = movements["R270"];
	movements["L180"] = movements["R180"];
	movements["L270"] = movements["R90"];

	for (int i = 1; i <= 100; i++)
	{
		movements["N" + to_string(i)] = [i](Ferry* f) { f->Position.Y += i; };
		movements["S" + to_string(i)] = [i](Ferry* f) { f->Position.Y -= i; };
		movements["E" + to_string(i)] = [i](Ferry* f) { f->Position.X += i; };
		movements["W" + to_string(i)] = [i](Ferry* f) { f->Position.X -= i; };

		movements["F" + to_string(i)] = [i](Ferry* f)
		{
			f->Position = f->Position + Point2{ f->Heading.X * i, f->Heading.Y * i };
		};
	}

	return movements;
}

static map<string, function<void(Ferry2*)>> MakeWaypointMovements()
{
	map<string, function<void(Ferry2*)>> movements;

	movements["R90"] = [](Ferry2* f) { f->Waypoint = Point2{ f->Waypoint.Y, -f->Waypoint.X }; };
	movements["R180"] = [](Ferry2* f) { f->Waypoint = Point2{ -f->Waypoint.X, -f->Waypoint.Y }; };
	movements["R270"] = [](Ferry2* f) { f->Waypoint = Point2{ -f->Waypoint.Y, f->Waypoint.X }; };

	movements["L90"] = movements["R270"];
	movements["L180"] = movements["R180"];
	movements["L270"] = movements["R90"];

	for (int i = 1; i <= 100; i++)
	{
		movements["N" + to_string(i)] = [i](Ferry2* f) { f->Waypoint.Y += i; };
		movements["S" + to_string(i)] = [i](Ferry2* f) { f->Waypoint.Y -= i; };
		movements["E" + to_string(i)] = [i](Ferry2* f) { f->Waypoint.X += i; };
		movements["W" + to_string(i)] = [i](Ferry2* f) { f->Waypoint.X -= i; };

		movements["F" + to_string(i)] = [i](Ferry2* f)
		{
			f->Position = f->Position + Point2{ f->Waypoint.X * i, f->Waypoint.Y * i };
		};
	}

	return movements;
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, function<void(Ferry*)>> movements = MakeMovements();

	vector<string> instructions = ReadAllLines(input);

	Ferry f = { { 0, 0 }, { 1, 0 } };
	for (const string& instruction : instructions)
	{
		assert(movements.contains(instruction));
		movements[instruction](&f);
	}

	int64_t answer = abs(f.Position.X) + abs(f.Position.Y);

	printf("[2020] Puzzle12_A: %" PRId64 "\n", answer);
}

static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, function<void(Ferry2*)>> movements = MakeWaypointMovements();

	vector<string> instructions = ReadAllLines(input);

	Ferry2 f = { { 0, 0 }, { 10, 1 } };
	for (const string& instruction : instructions)
	{
		assert(movements.contains(instruction));
		movements[instruction](&f);
	}

	int64_t answer = abs(f.Position.X) + abs(f.Position.Y);

	printf("[2020] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2020(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
