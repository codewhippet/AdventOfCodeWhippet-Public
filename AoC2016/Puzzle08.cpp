#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2016_Types
{
}

using namespace Puzzle08_2016_Types;

static int64_t UpdateBuffers(istream& input, vector<ArrayMap2D>* displayBuffers)
{
	assert(displayBuffers->size() == 2);

	size_t currentBufferIndex = 0;
	for (const string instruction : ReadEachLine(input))
	{
		currentBufferIndex = 1 - currentBufferIndex;

		ArrayMap2D& currentBuffer = (*displayBuffers)[currentBufferIndex];
		const ArrayMap2D& prevBuffer = (*displayBuffers)[1 - currentBufferIndex];

		currentBuffer = prevBuffer;

		if (instruction.starts_with("rect"sv))
		{
			int64_t x = 0;
			int64_t y = 0;
			int scanned = sscanf(instruction.c_str(), "rect %lldx%lld", &x, &y);
			assert(scanned == 2);
			(void)scanned;

			for (const Point2& p : GridRange(Point2{}, Point2{ x, y }))
			{
				currentBuffer(p) = '#';
			}
		}
		else if (instruction.starts_with("rotate"sv))
		{
			char type[16] = { 0 };
			int64_t index = 0;
			int64_t amount = 0;
			int scanned = sscanf(instruction.c_str(), "rotate %s %*c=%lld by %lld",
				type,
				&index,
				&amount);
			assert(scanned == 3);
			(void)scanned;

			assert(amount > -1);

			if (type == "row"sv)
			{
				for (int64_t x = 0; x < currentBuffer.GetWidth(); x++)
				{
					int64_t rotatedX = (x + amount) % currentBuffer.GetWidth();
					currentBuffer(rotatedX, index) = prevBuffer(x, index);
				}
			}
			else if (type == "column"sv)
			{
				for (int64_t y = 0; y < currentBuffer.GetHeight(); y++)
				{
					int64_t rotatedY = (y + amount) % currentBuffer.GetHeight();
					currentBuffer(index, rotatedY) = prevBuffer(index, y);
				}
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
	}

	return currentBufferIndex;
}

static void Puzzle08_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<ArrayMap2D> displayBuffers =
	{
		ArrayMap2D{ Point2{}, 50, 6, '.'},
		ArrayMap2D{ Point2{}, 50, 6, '.'}
	};

	size_t currentBufferIndex = UpdateBuffers(input, &displayBuffers);

	int64_t answer = displayBuffers[currentBufferIndex].Count('#');

	printf("[2016] Puzzle08_A: %" PRId64 "\n", answer);
}


static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<ArrayMap2D> displayBuffers =
	{
		ArrayMap2D{ Point2{}, 50, 6, '.'},
		ArrayMap2D{ Point2{}, 50, 6, '.'}
	};

	size_t currentBufferIndex = UpdateBuffers(input, &displayBuffers);

	printf("[2016] Puzzle08_B:\n");

	displayBuffers[currentBufferIndex].Print();
}

void Puzzle08_2016(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
