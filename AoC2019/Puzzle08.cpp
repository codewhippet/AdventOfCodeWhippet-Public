#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2019_Types
{
}

using namespace Puzzle08_2019_Types;

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const string imageData = ReadSingleLine(input);
	const int64_t width = 25;
	const int64_t height = 6;
	const int64_t layerSize = width * height;

	string fewestZerosLayer;
	int64_t fewestZeros = numeric_limits<int64_t>::max();
	for (int64_t layerStart = 0; layerStart < (int64_t)imageData.size(); layerStart += layerSize)
	{
		string line = imageData.substr(layerStart, layerSize);
		int64_t numZeros = ranges::count(line, '0');
		if (numZeros < fewestZeros)
		{
			fewestZeros = numZeros;
			fewestZerosLayer = line;
		}
	}

	int64_t answer = ranges::count(fewestZerosLayer, '1') * ranges::count(fewestZerosLayer, '2');

	printf("[2019] Puzzle08_A: %" PRId64 "\n", answer);
}


static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const string imageData = ReadSingleLine(input);
	const int64_t width = 25;
	const int64_t height = 6;

	ArrayMap2D image{ {}, width, height, '-' };
	Point2 cursor;
	for (char c : imageData)
	{
		if (image(cursor) == '-')
		{
			switch (c)
			{
			case '0':
				image(cursor) = '.';
				break;

			case '1':
				image(cursor) = '*';
				break;
			}
		}

		if (++cursor.X == width)
		{
			cursor.X = 0;
			if (++cursor.Y == height)
			{
				cursor.Y = 0;
			}
		}
	}

	printf("[2019] Puzzle08_B:\n");
	image.Print();
}

void Puzzle08_2019(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
