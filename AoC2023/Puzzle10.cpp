#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2023_Types
{
	enum class InOutState
	{
		Outside,
		InsideTop,
		InsideBottom,
		Inside,
	};

	struct Puzzle
	{
		Point2 Animal;
		set<Point2> Vertices;
		map<Point2, set<Point2>> Edges;

		map<Point2, char> RawMap;

		int64_t SizeX = 0;
		int64_t SizeY = 0;
	};

	static map<Point2, set<char>> Start_ConnectsWith =
	{
		{ Point2::North(), { '|', '7', 'F' } },
		{ Point2::East(), { '-', 'J', '7' } },
		{ Point2::South(), { '|', 'L', 'J' } },
		{ Point2::West(), { '-', 'L', 'F' } }
	};

	static map<set<Point2>, char> Start_SubWith =
	{
		{ { Point2::North(), Point2::South() }, '|' },
		{ { Point2::East(), Point2::West() }, '-' },
		{ { Point2::North(), Point2::East() }, 'L' },
		{ { Point2::North(), Point2::West() }, 'J' },
		{ { Point2::South(), Point2::West() }, '7' },
		{ { Point2::East(), Point2::South() }, 'F' },
	};

	static map<char, set<Point2>> Pipe_ConnectsTo =
	{
		{ '|', { Point2::North(), Point2::South() } },
		{ '-', { Point2::East(), Point2::West() }  },
		{ 'L', { Point2::North(), Point2::East() } },
		{ 'J', { Point2::North(), Point2::West() } },
		{ '7', { Point2::South(), Point2::West() } },
		{ 'F', { Point2::East(), Point2::South() } },
	};
}

using namespace Puzzle10_2023_Types;

static Puzzle ParsePipes(istream& input)
{
	Puzzle p;

	vector<string> lines = ReadAllLines(input);
	p.SizeX = lines[0].size();
	p.SizeY = lines.size();

	// Find the Animal starting point and substitute for correct pipe
	for (int64_t y = 0; y < (int64_t)lines.size(); y++)
	{
		for (int64_t x = 0; x < (int64_t)lines[y].size(); x++)
		{
			char c = lines[y][x];

			if (c == 'S')
			{
				p.Animal = Point2(x, y);
			}
			else
			{
				p.RawMap[Point2{ x, y }] = c;
			}
		}
	}

	set<Point2> animalConnectedTo;
	for (const Point2& cp : Point2::CardinalDirections())
	{
		Point2 animalNeighbour = p.Animal + cp;
		char animalNeighbourChar = p.RawMap[animalNeighbour];
		if (Start_ConnectsWith[cp].contains(animalNeighbourChar))
		{
			animalConnectedTo.insert(cp);
		}
	}
	assert(animalConnectedTo.size() == 2);

	char animalSub = Start_SubWith[animalConnectedTo];
	p.RawMap[p.Animal] = animalSub;

	for (map<Point2, char>::const_reference rawMapIt : p.RawMap)
	{
		Point2 thisVert = rawMapIt.first;
		p.Vertices.insert(thisVert);
		for (const Point2& cp : Pipe_ConnectsTo[rawMapIt.second])
		{
			Point2 connectedTo = cp + thisVert;
			p.Edges[thisVert].insert(connectedTo);
		}
	}

	return p;
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePipes(input);

	vector<pair<Point2, int64_t>> searchQueue;
	searchQueue.push_back(make_pair(p.Animal, 0));
	map<Point2, int64_t> visited;

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentLocation = searchQueue[i].first;
		int64_t currentSteps = searchQueue[i].second;

		if (visited.contains(currentLocation))
		{
			assert(visited[currentLocation] <= currentSteps);
			continue;
		}

		visited[currentLocation] = currentSteps;

		for (const Point2& neighbour : p.Edges[currentLocation])
		{
			if (visited.contains(neighbour) == false)
			{
				searchQueue.push_back(make_pair(neighbour, currentSteps + 1));
			}
		}
	}

	int64_t answer = 0;
	for (map<Point2, int64_t>::const_reference& visitCount : visited)
	{
		answer = max(answer, visitCount.second);
	}

	printf("[2023] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePipes(input);

	vector<pair<Point2, int64_t>> searchQueue;
	searchQueue.push_back(make_pair(p.Animal, 0));
	map<Point2, int64_t> visited;

	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		Point2 currentLocation = searchQueue[i].first;
		int64_t currentSteps = searchQueue[i].second;

		if (visited.contains(currentLocation))
		{
			assert(visited[currentLocation] <= currentSteps);
			continue;
		}

		visited[currentLocation] = currentSteps;

		for (const Point2& neighbour : p.Edges[currentLocation])
		{
			if (visited.contains(neighbour) == false)
			{
				searchQueue.push_back(make_pair(neighbour, currentSteps + 1));
			}
		}
	}

	int64_t answer = 0;

	for (int64_t y = 0; y < p.SizeY; y++)
	{
		InOutState currentState = InOutState::Outside;
		for (int64_t x = 0; x < p.SizeX; x++)
		{
			Point2 currentPoint{ x, y };
			char c = p.RawMap[currentPoint];
			bool isLoop = visited.contains(currentPoint);

			switch (currentState)
			{
			case InOutState::Outside:
				{
					if (isLoop)
					{
						if (c == 'F')
						{
							currentState = InOutState::InsideTop;
						}
						else if (c == '|')
						{
							currentState = InOutState::Inside;
						}
						else if (c == 'L')
						{
							currentState = InOutState::InsideBottom;
						}
						else
						{
							assert(false);
						}
					}
				}
				break;

			case InOutState::InsideTop:
				{
					if (isLoop)
					{
						if (c == 'J')
						{
							currentState = InOutState::Inside;
						}
						else if (c == '7')
						{
							currentState = InOutState::Outside;
						}
						else
						{
							assert(c == '-');
						}
					}
					else
					{
						assert(false);
					}
				}
				break;

			case InOutState::InsideBottom:
				{
					if (isLoop)
					{
						if (c == 'J')
						{
							currentState = InOutState::Outside;
						}
						else if (c == '7')
						{
							currentState = InOutState::Inside;
						}
						else
						{
							assert(c == '-');
						}
					}
					else
					{
						assert(false);
					}
				}
				break;

			case InOutState::Inside:
				{
					if (isLoop)
					{
						if (c == 'F')
						{
							currentState = InOutState::InsideBottom;
						}
						else if (c == '|')
						{
							currentState = InOutState::Outside;
						}
						else if (c == 'L')
						{
							currentState = InOutState::InsideTop;
						}
						else
						{
							assert(false);
						}
					}
					else
					{
						answer++;
					}
				}
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	printf("[2023] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2023(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
