#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2023_Types
{
	struct Puzzle
	{
		map<Point2, shared_ptr<int64_t>> PartNumbers;
		map<Point2, char> Symbols;
	};
}

using namespace Puzzle03_2023_Types;

static Puzzle ParseSchematic(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);
	for (int64_t y = 0; y < (int64_t)lines.size(); y++)
	{
		regex numberPattern{ R"(\d+)" };
		for (const smatch& m : ranges::subrange(sregex_iterator{ lines[y].begin(), lines[y].end(), numberPattern }, sregex_iterator{}))
		{
			string numberText = m.str();
			shared_ptr<int64_t> number = make_shared<int64_t>(stoll(numberText));
			for (int64_t x = m.position(); x < m.position() + (int64_t)numberText.length(); x++)
			{
				p.PartNumbers[Point2{ x, y }] = number;
			}
		}

		regex symbolPattern{ R"([^\d\.])" };
		for (const smatch& m : ranges::subrange(sregex_iterator{ lines[y].begin(), lines[y].end(), symbolPattern }, sregex_iterator{}))
		{
			int64_t x = m.position();
			p.Symbols[Point2{ x, y }] = m.str()[0];
		}
	}

	return p;
}

static void Puzzle03_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParseSchematic(input);

	set<shared_ptr<int64_t>> partNumbersIncluded;
	for (map<Point2, char>::const_reference symbol : p.Symbols)
	{
		for (const Point2& neighbourOffset : Point2::CardinalAndDiagonalDirections())
		{
			const Point2 neighbour = symbol.first + neighbourOffset;
			if (p.PartNumbers.contains(neighbour))
			{
				partNumbersIncluded.insert(p.PartNumbers[neighbour]);
			}
		}
	}

	int64_t answer = MakeEnumerator(partNumbersIncluded)
		->Select<int64_t>([](const shared_ptr<int64_t>& number) { return *number; })
		->Sum();

	printf("[2023] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParseSchematic(input);

	int64_t answer = 0;
	for (map<Point2, char>::const_reference symbol : p.Symbols)
	{
		if (symbol.second != '*')
			continue;

		set<shared_ptr<int64_t>> gears;
		for (const Point2& neighbourOffset : Point2::CardinalAndDiagonalDirections())
		{
			const Point2 neighbour = symbol.first + neighbourOffset;
			if (p.PartNumbers.contains(neighbour))
			{
				gears.insert(p.PartNumbers[neighbour]);
			}
		}

		if (gears.size() == 2)
		{
			int64_t gearRatio = *(*gears.begin()) * *(*++gears.begin());
			answer += gearRatio;
		}
	}

	printf("[2023] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2023(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
