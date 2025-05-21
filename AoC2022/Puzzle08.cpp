#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle08_2022_Types
{
}

using namespace Puzzle08_2022_Types;

static char DoMaxCalc(char height, char* max)
{
    char ret = *max;
    if (height > *max)
    {
        *max = height;
    }
    return ret;
}

static ArrayMap2D GenerateMaxHeights(const ArrayMap2D& heights, const Point2 &startAt, const Point2 &majorAxis, const Point2 &maxDirection)
{
	ArrayMap2D maxHeights{ ArrayMap2DOptions::CloneAsInvalid, heights };

	Point2 lineStart = startAt;
	do
	{
		char maxHeight = -1;
		Point2 currentPos = lineStart;
		do
		{
			maxHeights(currentPos) = DoMaxCalc(heights(currentPos), &maxHeight);
			currentPos += maxDirection;
		}
		while (heights.IsInside(currentPos));

		lineStart += majorAxis;
	}
	while (heights.IsInside(lineStart));

	return maxHeights;
}

static generator<char> IterateFrom(const ArrayMap2D& heights, const Point2 &startAt, const Point2 &direction)
{
	char startHeight = heights(startAt);

	Point2 currentPos = startAt + direction;
	while (heights.IsInside(currentPos))
	{
		char thisHeight = heights(currentPos);
		co_yield thisHeight;
		if (thisHeight >= startHeight)
			break;

		currentPos += direction;
	}
}
auto foo = &IterateFrom;

static void Puzzle08_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D heights = ReadArrayMap(input);

	ArrayMap2D leftToRightMax = GenerateMaxHeights(heights, Point2{ 0, 0 }, Point2::Down(), Point2::Right());
	ArrayMap2D rightToLeftMax = GenerateMaxHeights(heights, Point2{ heights.GetWidth() - 1, 0 }, Point2::Down(), Point2::Left());
	ArrayMap2D topToBottomMax = GenerateMaxHeights(heights, Point2{ 0, 0 }, Point2::Right(), Point2::Down());
	ArrayMap2D bottomToTopMax = GenerateMaxHeights(heights, Point2{ 0, heights.GetHeight() - 1 }, Point2::Right(), Point2::Up());

	int64_t answer = 0;
	for (const auto &p : heights.Grid())
	{
		if ((p.second > leftToRightMax(p.first)) ||
			(p.second > rightToLeftMax(p.first)) ||
			(p.second > topToBottomMax(p.first)) ||
			(p.second > bottomToTopMax(p.first)))
		{
			answer++;
		}
	}

	printf("[2022] Puzzle08_A: %" PRId64 "\n", answer);
}

static void Puzzle08_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D heights = ReadArrayMap(input);

	const auto True = [](const auto &) { return true; };

	int64_t answer = 0;
	for (const auto &p : heights.Grid())
	{
		int64_t scenicScore =
			ranges::count_if(IterateFrom(heights, p.first, Point2::Up()), True) *
			ranges::count_if(IterateFrom(heights, p.first, Point2::Down()), True) *
			ranges::count_if(IterateFrom(heights, p.first, Point2::Left()), True) *
			ranges::count_if(IterateFrom(heights, p.first, Point2::Right()), True);
		answer = max(scenicScore, answer);
	}

	printf("[2022] Puzzle08_B: %" PRId64 "\n", answer);
}

void Puzzle08_2022(const string& filename)
{
	Puzzle08_A(filename);
	Puzzle08_B(filename);
}
