#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2019_Types
{
}

using namespace Puzzle11_2019_Types;

static void Puzzle11_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	assert(puter.Execute() == Intputer::ExecutionResult::PendingIo);

	Point2 robotPos;
	Point2 robotDir = Point2::Up();
	set<Point2> whitePanels;
	set<Point2> paintedPanels;
	while (true)
	{
		in.push_back(whitePanels.contains(robotPos));
		auto exec = puter.Execute();
		if (exec == Intputer::ExecutionResult::Finished)
			break;

		assert(exec == Intputer::ExecutionResult::PendingIo);
		assert(in.size() == 0);
		assert(out.size() == 2);

		if (out[0] == 0)
		{
			whitePanels.erase(robotPos);
		}
		else
		{
			whitePanels.insert(robotPos);
		}

		paintedPanels.insert(robotPos);

		if (out[1] == 0)
		{
			robotDir = Point2::RotateAnticlockwise(robotDir);
		}
		else
		{
			robotDir = Point2::RotateClockwise(robotDir);
		}

		out.clear();

		robotPos = robotPos + robotDir;
	}

	int64_t answer = (int64_t)paintedPanels.size();

	printf("[2019] Puzzle11_A: %" PRId64 "\n", answer);
}


static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Intputer puter(input);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	assert(puter.Execute() == Intputer::ExecutionResult::PendingIo);

	Point2 robotPos;
	Point2 robotDir = Point2::Up();
	PointMap panels;
	panels.Data.insert({ {}, '#' });
	while (true)
	{
		in.push_back(panels.Data.contains(robotPos) && panels.Data.at(robotPos) == '#');
		auto exec = puter.Execute();
		if (exec == Intputer::ExecutionResult::Finished)
			break;

		assert(exec == Intputer::ExecutionResult::PendingIo);
		assert(in.size() == 0);
		assert(out.size() == 2);

		panels.Data[robotPos] = (out[0] == 0) ? '.' : '#';

		if (out[1] == 0)
		{
			robotDir = Point2::RotateAnticlockwise(robotDir);
		}
		else
		{
			robotDir = Point2::RotateClockwise(robotDir);
		}

		out.clear();

		robotPos = robotPos + robotDir;
	}

	panels.Size.X = ranges::max(panels.Data | views::keys | views::transform(Point2::GetX)) + 1;
	panels.Size.Y = ranges::max(panels.Data | views::keys | views::transform(Point2::GetY)) + 1;

	printf("[2019] Puzzle11_B:\n");
	PrintPointMap(panels);
}

void Puzzle11_2019(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
