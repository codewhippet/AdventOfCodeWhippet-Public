#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2017_Types
{
}

using namespace Puzzle21_2017_Types;

static string Rotate2x2(const string& input)
{
	string output(input.size(), '.');
	output[0] = input[1];
	output[1] = input[3];
	output[2] = input[0];
	output[3] = input[2];
	return output;
}

static string Flip2x2(const string& input)
{
	string output(input.size(), '.');
	output[0] = input[1];
	output[1] = input[0];
	output[2] = input[3];
	output[3] = input[2];
	return output;
}

static string Rotate3x3(const string& input)
{
	string output(input.size(), '.');
	output[0] = input[6];
	output[1] = input[3];
	output[2] = input[0];
	output[3] = input[7];
	output[4] = input[4];
	output[5] = input[1];
	output[6] = input[8];
	output[7] = input[5];
	output[8] = input[2];
	return output;
}

static string Flip3x3(const string& input)
{
	string output(input.size(), '.');
	output[0] = input[2];
	output[1] = input[1];
	output[2] = input[0];
	output[3] = input[5];
	output[4] = input[4];
	output[5] = input[3];
	output[6] = input[8];
	output[7] = input[7];
	output[8] = input[6];
	return output;
}

static set<string> MakePatternVariations(const string& start)
{
	assert((start.size() == 4) || (start.size() == 9));

	auto flip = (start.size() == 4) ? &Flip2x2 : &Flip3x3;
	auto rotate = (start.size() == 4) ? &Rotate2x2 : &Rotate3x3;

	set<string> variations;

	string variant = start;

	for (int i = 0; i < 4; i++)
	{
		variations.insert(variant);
		variations.insert(flip(variant));
		variant = rotate(variant);
	}

	return variations;
}

static string ExtractSquare(const PointMap& image, int64_t squareSize, int64_t squareX, int64_t squareY)
{
	string square;
	for (const Point2& p : GridRange{ Point2{ squareX, squareY } * squareSize, Point2{ squareX + 1, squareY + 1 } * squareSize })
	{
		square += image.Data.at(p);
	}
	assert((int64_t)square.size() == (squareSize * squareSize));
	return square;
}

static void InjectSquare(string pattern, int64_t squareSize, int64_t squareX, int64_t squareY, PointMap* image)
{
	assert((int64_t)pattern.size() == (squareSize * squareSize));
	for (const Point2& p : GridRange{ Point2{ squareX, squareY } * squareSize, Point2{ squareX + 1, squareY + 1 } * squareSize })
	{
		image->Data[p] = pattern[0];
		pattern = pattern.substr(1);
	}
	assert(pattern.size() == 0);
}

static PointMap Enhance(const PointMap& oldImage, const map<string, string>& mappings)
{
	assert(((oldImage.Size.X % 2) == 0) || ((oldImage.Size.X % 3) == 0));
	int64_t oldSquareSize = (oldImage.Size.X % 2 == 0) ? 2 : 3;
	int64_t expandedSquareSize = oldSquareSize + 1;

	int64_t newImageSize = (oldImage.Size.X / oldSquareSize) * expandedSquareSize;
	PointMap newImage{ {}, { newImageSize, newImageSize } };

	for (int64_t squareY = 0; squareY < oldImage.Size.X / oldSquareSize; squareY++)
	{
		for (int64_t squareX = 0; squareX < oldImage.Size.X / oldSquareSize; squareX++)
		{
			string square = ExtractSquare(oldImage, oldSquareSize, squareX, squareY);
			InjectSquare(mappings.at(square), expandedSquareSize, squareX, squareY, &newImage);
		}
	}
	return newImage;
}

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, string> mappings;
	for (const auto& mapping : ScanfEachLine<char[64], char[64]>(input, "%[.#/] => %[.#/]"))
	{
		string from = get<0>(mapping);
		string to = get<1>(mapping);

		from.erase(remove(from.begin(), from.end(), '/'), from.end());
		to.erase(remove(to.begin(), to.end(), '/'), to.end());

		for (const string& pattern : MakePatternVariations(from))
		{
			mappings[pattern] = to;
		}
	}

	const string startingPattern = ".#.\n..#\n###";
	istringstream startingPatternInput{ startingPattern };

	PointMap image = ReadPointMap(startingPatternInput, ' ');
	for (int i = 0; i < 5; i++)
	{
		image = Enhance(image, mappings);
	}

	int64_t answer = ranges::count_if(image.Data, [](const auto& kvp) { return kvp.second == '#'; });

	printf("[2017] Puzzle21_A: %" PRId64 "\n", answer);
}


static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, string> mappings;
	for (const auto& mapping : ScanfEachLine<char[64], char[64]>(input, "%[.#/] => %[.#/]"))
	{
		string from = get<0>(mapping);
		string to = get<1>(mapping);

		from.erase(remove(from.begin(), from.end(), '/'), from.end());
		to.erase(remove(to.begin(), to.end(), '/'), to.end());

		for (const string& pattern : MakePatternVariations(from))
		{
			mappings[pattern] = to;
		}
	}

	const string startingPattern = ".#.\n..#\n###";
	istringstream startingPatternInput{ startingPattern };

	PointMap image = ReadPointMap(startingPatternInput, ' ');
	for (int i = 0; i < 18; i++)
	{
		image = Enhance(image, mappings);
	}

	int64_t answer = ranges::count_if(image.Data, [](const auto& kvp) { return kvp.second == '#'; });

	printf("[2017] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2017(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
