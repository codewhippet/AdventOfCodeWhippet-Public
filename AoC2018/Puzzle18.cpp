#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2018_Types
{
}

using namespace Puzzle18_2018_Types;

static int64_t CountSurrounding(const ArrayMap2D& lumberArea, const Point2& origin, char lookingFor)
{
	return ranges::count(Point2::CardinalAndDiagonalDirections() | views::transform([&](const Point2& dir) { return lumberArea(origin + dir); }), lookingFor);
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<ArrayMap2D> buffers = { ReadArrayMap(input, ' ') };
	buffers.push_back(buffers[0]);

	size_t currentBufferIndex = 0;
	for (int i = 0; i < 10; i++)
	{
		ArrayMap2D& currentBuffer = buffers[currentBufferIndex];
		ArrayMap2D &nextBuffer = buffers[1 - currentBufferIndex];
		for (const auto& kvp : buffers[currentBufferIndex].Grid())
		{
			switch (kvp.second)
			{
			case '.':
				nextBuffer(kvp.first) = CountSurrounding(currentBuffer, kvp.first, '|') >= 3 ? '|' : '.';
				break;
			case '|':
				nextBuffer(kvp.first) = CountSurrounding(currentBuffer, kvp.first, '#') >= 3 ? '#' : '|';
				break;
			case '#':
				{
					int64_t lumberyards = CountSurrounding(currentBuffer, kvp.first, '#');
					int64_t trees = CountSurrounding(currentBuffer, kvp.first, '|');
					nextBuffer(kvp.first) = lumberyards && trees ? '#' : '.';
				}
				break;
			}
		}

		currentBufferIndex = 1 - currentBufferIndex;
	}

	int64_t answer = buffers[currentBufferIndex].Count('|') * buffers[currentBufferIndex].Count('#');

	printf("[2018] Puzzle18_A: %" PRId64 "\n", answer);
}


static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<ArrayMap2D> buffers = { ReadArrayMap(input, ' ') };
	buffers.push_back(buffers[0]);

	map<vector<char>, int64_t> frames;

	int64_t repeatBegin = -1;
	int64_t repeatEnd = -1;

	size_t currentBufferIndex = 0;
	for (int64_t i = 0; i < 10000; i++)
	{
		ArrayMap2D& currentBuffer = buffers[currentBufferIndex];
		vector<char> data = currentBuffer.GetData();
		auto existingFrameIt = frames.find(data);
		if (existingFrameIt != frames.end())
		{
			repeatBegin = existingFrameIt->second;
			repeatEnd = i;
			break;
		}

		frames.insert({ move(data), i });

		ArrayMap2D& nextBuffer = buffers[1 - currentBufferIndex];
		for (const auto& kvp : buffers[currentBufferIndex].Grid())
		{
			switch (kvp.second)
			{
			case '.':
				nextBuffer(kvp.first) = CountSurrounding(currentBuffer, kvp.first, '|') >= 3 ? '|' : '.';
				break;
			case '|':
				nextBuffer(kvp.first) = CountSurrounding(currentBuffer, kvp.first, '#') >= 3 ? '#' : '|';
				break;
			case '#':
				{
					int64_t lumberyards = CountSurrounding(currentBuffer, kvp.first, '#');
					int64_t trees = CountSurrounding(currentBuffer, kvp.first, '|');
					nextBuffer(kvp.first) = lumberyards && trees ? '#' : '.';
				}
				break;
			}
		}

		currentBufferIndex = 1 - currentBufferIndex;
	}

	int64_t cycleLength = repeatEnd - repeatBegin;
	int64_t cycleOffset = (1000000000 - repeatBegin) % cycleLength;
	int64_t actualFrame = repeatBegin + cycleOffset;

	auto frameIt = ranges::find_if(frames, [&](const auto& kvp) { return kvp.second == actualFrame; });
	assert(frameIt != frames.end());

	const vector<char> lumberArea = frameIt->first;

	int64_t answer = ranges::count(lumberArea, '|') * ranges::count(lumberArea, '#');

	printf("[2018] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2018(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
