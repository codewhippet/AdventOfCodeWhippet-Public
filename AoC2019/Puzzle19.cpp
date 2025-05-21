#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2019_Types
{
}

using namespace Puzzle19_2019_Types;

static bool IsInside(const Point2& pos, const vector<int64_t> &program, Intputer* puter)
{
	puter->Reset(program);

	puter->GetReadQueue()->push_back(pos.X);
	puter->GetReadQueue()->push_back(pos.Y);

	auto exec = puter->Execute();
	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;

	return puter->GetWriteQueue()->front() == 1;
}

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> program = ReadAsVectorOfNumbers(ReadSingleLine(input));

	Intputer puter;

	IntputerIO io;
	puter.SetReadWriteQueues(&io);

	int64_t answer = ranges::count_if(GridRange{ Point2{}, Point2{ 50, 50 } },
		[&](const auto& p)
		{
			return IsInside(p, program, &puter);
		});

	printf("[2019] Puzzle19_A: %" PRId64 "\n", answer);
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> program = ReadAsVectorOfNumbers(ReadSingleLine(input));

	Intputer puter;

	IntputerIO io;
	puter.SetReadWriteQueues(&io);

	Point2 topRight{ 10, 0 };
	while (IsInside(topRight, program, &puter) == false)
		topRight += Point2::Down();

	Point2 closest;
	while (true)
	{
		assert(IsInside(topRight, program, &puter));

		Point2 bottomLeft = topRight + 99 * (Point2::Left() + Point2::Down());
		if (IsInside(bottomLeft, program, &puter))
		{
			closest = Point2{ bottomLeft.X, topRight.Y };
			break;
		}

		topRight += IsInside(topRight + Point2::Right(), program, &puter) ? Point2::Right() : Point2::Down();
	}

	int64_t answer = closest.X * 10000 + closest.Y;

	printf("[2019] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2019(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
