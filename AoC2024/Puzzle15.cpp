#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2024_Types
{
	struct Puzzle
	{
		ArrayMap2D* Room = nullptr;
		vector<string> Instructions;
	};

	static map<char, Point2> Directions =
	{
		{'^', Point2::Up()},
		{'>', Point2::Right()},
		{'v', Point2::Down()},
		{'<', Point2::Left()},
	};
}

using namespace Puzzle15_2024_Types;

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle ret;

	string board;
	for (const string& line : ReadAllLines(input))
	{
		if (line[0] == '#')
		{
			board += line;
			board += '\n';
		}
		else if (line.size() > 0)
		{
			ret.Instructions.push_back(line);
		}
	}

	istringstream boardInput(board);
	ret.Room = new ArrayMap2D(ReadArrayMap(boardInput));

	return ret;
}

static void WidenRoom(Puzzle* p)
{
	ArrayMap2D* newRoom = new ArrayMap2D(p->Room->GetOrigin(),
		p->Room->GetWidth() * 2,
		p->Room->GetHeight(),
		p->Room->GetInvalidCharacter());

	for (const auto& kvp : p->Room->Grid())
	{
		Point2 dest = kvp.first;
		dest.X *= 2;
		switch (kvp.second)
		{
		case '#':
		case '.':
			(*newRoom)(dest) = kvp.second;
			(*newRoom)(dest + Point2::Right()) = kvp.second;
			break;

		case '@':
			(*newRoom)(dest) = '@';
			(*newRoom)(dest + Point2::Right()) = '.';
			break;

		case 'O':
			(*newRoom)(dest) = '[';
			(*newRoom)(dest + Point2::Right()) = ']';
			break;
		}
	}

	delete p->Room;
	p->Room = newRoom;
}

static bool TryMove(const Point2& from, const Point2& direction, ArrayMap2D* board)
{
	if ((*board)(from + direction) == '#')
	{
		return false;
	}

	if ((*board)(from + direction) == '.')
	{
		(*board)(from + direction) = (*board)(from);
		return true;
	}

	if (TryMove(from + direction, direction, board))
	{
		(*board)(from + direction) = (*board)(from);
		return true;
	}

	return false;
}

static bool CanMoveVertically(const Point2& from, const Point2& direction, ArrayMap2D* board)
{
	switch ((*board)(from + direction))
	{
	case '#':
		return false;

	case '.':
		return true;

	case '[':
		return CanMoveVertically(from + direction, direction, board) && CanMoveVertically(from + direction + Point2::Right(), direction, board);

	case ']':
		return CanMoveVertically(from + direction, direction, board) && CanMoveVertically(from + direction + Point2::Left(), direction, board);
	}

	return false;
}

static void CommitMoveVertically(const Point2& from, const Point2& direction, ArrayMap2D* board)
{
	char thisChar = (*board)(from);
	assert(thisChar != '#');

	if (thisChar == '.')
		return;

	CommitMoveVertically(from + direction, direction, board);

	switch (thisChar)
	{
	case '[':
		CommitMoveVertically(from + direction + Point2::Right(), direction, board);

		(*board)(from + direction + Point2::Right()) = ']';
		(*board)(from + Point2::Right()) = '.';
		break;

	case ']':
		CommitMoveVertically(from + direction + Point2::Left(), direction, board);

		(*board)(from + direction + Point2::Left()) = '[';
		(*board)(from + Point2::Left()) = '.';
		break;
	}

	(*board)(from + direction) = thisChar;
	(*board)(from) = '.';
}

static int64_t GpsWarehouse(const ArrayMap2D* room, char box)
{
	auto locateBoxes = room->Grid()
		| views::filter([&](const auto& kvp)
			{
				return kvp.second == box;
			})
		| views::transform([](const auto& kvp)
			{
				return (kvp.first.Y * 100) + kvp.first.X;
			});

	return accumulate(locateBoxes.begin(), locateBoxes.end(), 0ll);
}

static void Puzzle15_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	auto findRobot = p.Room->Grid()
		| views::filter([](const auto& p)
			{
				return p.second == '@';
			})
		| views::transform([](const auto& p)
			{
				return p.first;
			});
	Point2 robot = findRobot.front();

	for (const string& line : p.Instructions)
	{
		for (char d : line)
		{
			//p.Room->Print();

			decltype(Directions)::const_iterator move = Directions.find(d);
			if (move != Directions.end())
			{
				Point2 newLocation = robot + move->second;
				if (TryMove(robot, move->second, p.Room))
				{
					(*p.Room)(robot) = '.';
					robot = newLocation;
				}
			}
		}
	}

	int64_t answer = GpsWarehouse(p.Room, 'O');

	printf("[2024] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ReadPuzzle(input);

	WidenRoom(&p);

	auto findRobot = p.Room->Grid()
		| views::filter([](const auto& p)
			{
				return p.second == '@';
			})
		| views::transform([](const auto& p)
			{
				return p.first;
			});
	Point2 robot = findRobot.front();

	for (const string& line : p.Instructions)
	{
		for (char d : line)
		{
			decltype(Directions)::const_iterator move = Directions.find(d);
			if (move != Directions.end())
			{
				//p.Room->Print();

				Point2 newLocation = robot + move->second;
				switch (d)
				{
				case '^':
				case 'v':
					{
						if (CanMoveVertically(robot, move->second, p.Room))
						{
							CommitMoveVertically(robot, move->second, p.Room);
							robot = newLocation;
						}
					}
					break;

				case '<':
				case '>':
					{
						if (TryMove(robot, move->second, p.Room))
						{
							(*p.Room)(robot) = '.';
							robot = newLocation;
						}
					}
					break;
				}
			}
		}
	}

	//p.Room->Print();

	int64_t answer = GpsWarehouse(p.Room, '[');

	printf("[2024] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2024(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
