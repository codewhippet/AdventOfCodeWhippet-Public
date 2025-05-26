#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

// Vertex numbering:
// 
//    1------2
//   /|     /|
//  3------4 |
//  | |    | |
//  | 5----|-6
//  |/     |/
//  7------8
//
// Faces are identified by clockwise vertex ordering
// The edge "24" corresponds to face "1243" and not the one on face "4268"
// The IDs "3412" and "2431" identify the same face, but we prefer the
// smallest number first ("1234") for a unique identifier where needed.

namespace Puzzle22_2022_Types
{
	struct IrregularMap
	{
		Point2 Dimensions;
		map<Point2, char> Maze;
		Point2 StartPos;
	};

	struct Face
	{
		int64_t Index;
		map<Point2, char> Tiles;
		Point2 Offset;
		string Id;
	};
}

using namespace Puzzle22_2022_Types;

static IrregularMap ReadIrregularMap(const vector<string> &mapSource)
{
	IrregularMap irregularMap;
	for (auto const [y, line] : views::enumerate(mapSource))
	{
		for (auto const [x, c] : views::enumerate(line))
		{
			if (c != ' ')
			{
				irregularMap.Maze.insert({ { x, y }, c });
			}
		}
	}

	irregularMap.StartPos = Point2{ (int64_t)mapSource[0].find_first_not_of(' '), 0 };
	irregularMap.Dimensions.X = ranges::max(irregularMap.Maze | views::keys | views::transform(&Point2::GetX)) + 1;
	irregularMap.Dimensions.Y = ranges::max(irregularMap.Maze | views::keys | views::transform(&Point2::GetY)) + 1;

	return irregularMap;
}

static vector<string> SplitInstructions(const string &line)
{
	vector<string> instructions{ 1ull };

	const char *c = line.c_str();
	instructions.back().push_back(*c);

	for (c++; *c != '\0'; c++)
	{
		if (isdigit(c[-1]) != isdigit(c[0]))
		{
			instructions.push_back({});
		}
		instructions.back().push_back(*c);
	}

	return instructions;
}

static int64_t ScoreDirection(const Point2& dir)
{
	const map<Point2, int64_t> directionScore =
	{
		{ Point2::Right(), 0 },
		{ Point2::Down(), 1 },
		{ Point2::Left(), 2 },
		{ Point2::Up(), 3 },
	};
	return directionScore.at(dir);
}

static int64_t FindFaceWidth(const vector<string> &source)
{
	return ranges::min(source | views::transform([](const string &s) { return Trim(s).size(); }));
}

static map<Point2, shared_ptr<Face>> ReadSourceFaces(const vector<string>& source, int64_t faceSize)
{
	map<Point2, shared_ptr<Face>> faces;
	for (int64_t faceY = 0; faceY < (int64_t)source.size(); faceY += faceSize)
	{
		for (int64_t faceX = 0; faceX < (int64_t)source[faceY].size(); faceX += faceSize)
		{
			assert((faceX % faceSize) == 0);
			assert((faceY % faceSize) == 0);

			if (source[faceY][faceX] == ' ')
				continue;

			const Point2 faceOffset{ faceX, faceY };

			shared_ptr<Face> newFace = make_shared<Face>();
			newFace->Index = faces.size() + 1;
			newFace->Offset = faceOffset;
			for (int64_t y = 0; y < faceSize; y++)
			{
				for (int64_t x = 0; x < faceSize; x++)
				{
					newFace->Tiles.insert({ Point2{ x, y }, source[faceY + y][faceX + x] });
				}
			}
			faces.insert({ Point2{ faceOffset.X / faceSize, faceOffset.Y / faceSize }, newFace });
		}
	}
	return faces;
}

static string Reverse(string s)
{
	ranges::reverse(s);
	return s;
}

static map<string, string> BuildOppositeTable()
{
	map<string, string> opposites;
	for (const string &face : { "1243", "3487", "2684", "1562", "1375", "5786" })
	{
		string doubleFace = face + face;
		for (size_t i = 0; i < face.size(); i++)
		{
			opposites[doubleFace.substr(i, 2)] = doubleFace.substr(i + 2, 2);
		}
	}
	return opposites;
}

static string OppositeEdge(const string &edge)
{
	static const map<string, string> opposites = BuildOppositeTable();
	return opposites.at(edge);
}

static string FormNeigbouringFaceId(const string &currentId, const Point2& direction)
{
	string doubleId = currentId + currentId;
	if (direction == Point2::Up())
	{
		string reverseEdge = Reverse(doubleId.substr(0, 2));
		string oppositeEdge = OppositeEdge(reverseEdge);
		string id = oppositeEdge + reverseEdge;
		return id;
	}
	else if (direction == Point2::Right())
	{
		string reverseEdge = Reverse(doubleId.substr(1, 2));
		string oppositeEdge = OppositeEdge(reverseEdge);
		string id = reverseEdge[1] + oppositeEdge + reverseEdge[0];
		return id;
	}
	else if (direction == Point2::Down())
	{
		string reverseEdge = Reverse(doubleId.substr(2, 2));
		string oppositeEdge = OppositeEdge(reverseEdge);
		string id = reverseEdge + oppositeEdge;
		return id;
	}
	else if (direction == Point2::Left())
	{
		string reverseEdge = Reverse(doubleId.substr(3, 2));
		string oppositeEdge = OppositeEdge(reverseEdge);
		string id = oppositeEdge[1] + reverseEdge + oppositeEdge[0];
		return id;
	}

	assert(false);
	return "";
}

static void AssignFaceIds(map<Point2, shared_ptr<Face>>* faces)
{
	// Start with the face "1243" and spider outwards
	const Point2 startAt = ranges::find_if(*faces, [](const auto& p) { return p.second->Index == 1; })->first;

	vector<pair<Point2, string>> assignmentQueue{ { startAt, "1243" } };
	for (size_t i = 0; i < assignmentQueue.size(); i++)
	{
		auto [pos, id] = assignmentQueue[i];
		if (faces->at(pos)->Id.empty() == false)
		{
			assert(faces->at(pos)->Id == id);
			continue;
		}

		faces->at(pos)->Id = id;
		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 neighbour = pos + dir;
			if (faces->contains(neighbour))
			{
				assignmentQueue.push_back({ neighbour, FormNeigbouringFaceId(id, dir) });
			}
		}
	}
}

static pair<string, int64_t> GetExitAlongEdge(const string &faceId, const Point2& from, const Point2& dir, const int64_t faceSize)
{
	if (dir == Point2::Up())
	{
		return { faceId.substr(0, 2), from.X };
	}
	else if (dir == Point2::Right())
	{
		return { faceId.substr(1, 2), from.Y };
	}
	else if (dir == Point2::Down())
	{
		return { faceId.substr(2, 2), (faceSize - 1) - from.X };
	}
	else if (dir == Point2::Left())
	{
		return { (faceId + faceId).substr(3, 2), (faceSize - 1) - from.Y };
	}
	return { "", -1 };
}

static shared_ptr<Face> FindFaceWithEdge(const vector<shared_ptr<Face>>& faces, const string &edge)
{
	assert(edge.size() == 2);
	return *ranges::find_if(faces,
		[&](const auto& f)
		{
			string doubleId = f->Id + f->Id;
			return doubleId.contains(edge);
		});
}

static pair<Point2, Point2> GetEntryAlongEdge(const string &faceId, const string& edge, int64_t distance, const int64_t faceSize)
{
	assert(edge.size() == 2);
	switch ((faceId + faceId).find(edge))
	{
	case 0:
		return { Point2{ distance, 0 }, Point2::Down() };
	case 1:
		return { Point2{ (faceSize - 1), distance }, Point2::Left() };
	case 2:
		return { Point2{ (faceSize - 1) - distance, (faceSize - 1) }, Point2::Up() };
	case 3:
		return { Point2{ 0, (faceSize - 1) - distance }, Point2::Right() };
	default:
		assert(false);
	}

	return { {}, {} };
}

static void Puzzle22_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	const regex mazePattern{ R"(\s*[\.#]+\s*)" };
	vector<string> mapSource = lines
		| views::filter([&](const auto &l) { return regex_match(l, mazePattern); })
		| ranges::to<vector>();

	IrregularMap irregularMap = ReadIrregularMap(mapSource);

	Point2 currentPos = irregularMap.StartPos;
	Point2 currentDir = Point2::Right();
	for (const string& instruction : SplitInstructions(lines.back()))
	{
		if (instruction[0] == 'R')
		{
			currentDir = Point2::RotateClockwise(currentDir);
		}
		else if (instruction[0] == 'L')
		{
			currentDir = Point2::RotateAnticlockwise(currentDir);
		}
		else
		{
			int64_t steps = atoll(instruction.c_str());
			for (int64_t step = 0; step < steps; step++)
			{
				Point2 nextPos = currentPos;
				do
				{
					nextPos.X = (nextPos.X + currentDir.X + irregularMap.Dimensions.X) % irregularMap.Dimensions.X;
					nextPos.Y = (nextPos.Y + currentDir.Y + irregularMap.Dimensions.Y) % irregularMap.Dimensions.Y;

				} while (irregularMap.Maze.contains(nextPos) == false);

				if (irregularMap.Maze.at(nextPos) == '#')
					break;

				currentPos = nextPos;
			}
		}
	}

	int64_t answer = (4 * (currentPos.X + 1)) + (1000 * (currentPos.Y + 1)) + ScoreDirection(currentDir);

	printf("[2022] Puzzle22_A: %" PRId64 "\n", answer);
}

static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	const regex mazePattern{ R"(\s*[\.#]+\s*)" };
	vector<string> mapSource = lines
		| views::filter([&](const auto &l) { return regex_match(l, mazePattern); })
		| ranges::to<vector>();

	int64_t faceWidth = FindFaceWidth(mapSource);
	assert((mapSource.size() % faceWidth) == 0); // Assume square faces

	map<Point2, shared_ptr<Face>> sourceFaces = ReadSourceFaces(mapSource, faceWidth);
	assert(sourceFaces.size() == 6);

	AssignFaceIds(&sourceFaces);

	vector<shared_ptr<Face>> faces = sourceFaces | views::values | ranges::to<vector>();

	Point2 currentPos;
	Point2 currentDir = Point2::Right();
	shared_ptr<Face> currentFace = *ranges::find_if(faces, [](const auto& f) { return f->Index == 1; });

	for (const string& instruction : SplitInstructions(lines.back()))
	{
		if (instruction[0] == 'R')
		{
			currentDir = Point2::RotateClockwise(currentDir);
		}
		else if (instruction[0] == 'L')
		{
			currentDir = Point2::RotateAnticlockwise(currentDir);
		}
		else
		{
			int64_t steps = atoll(instruction.c_str());
			for (int64_t step = 0; step < steps; step++)
			{
				Point2 nextPos = currentPos + currentDir;
				Point2 nextDir = currentDir;

				shared_ptr<Face> nextFace;
				if (currentFace->Tiles.contains(nextPos))
				{
					// Didn't wrap
					nextFace = currentFace;
				}
				else
				{
					auto [exitEdge, exitDistance] = GetExitAlongEdge(currentFace->Id, currentPos, currentDir, faceWidth);
					string entryEdge = Reverse(exitEdge);
					nextFace = FindFaceWithEdge(faces, entryEdge);
					auto [entryPos, entryDir] = GetEntryAlongEdge(nextFace->Id, entryEdge, (faceWidth - 1) - exitDistance, faceWidth);

					nextPos = entryPos;
					nextDir = entryDir;
				}

				if (nextFace->Tiles.at(nextPos) == '#')
					break;

				currentPos = nextPos;
				currentDir = nextDir;
				currentFace = nextFace;
			}
		}
	}

	currentPos += currentFace->Offset;
	int64_t answer = (4 * (currentPos.X + 1)) + (1000 * (currentPos.Y + 1)) + ScoreDirection(currentDir);

	printf("[2022] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2022(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
