#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2019_Types
{
}

using namespace Puzzle17_2019_Types;

static ArrayMap2D ExtractMap(const vector<int64_t>& program)
{
	Intputer puter;
	puter.CopyProgram(program);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	puter.Execute();

	string startingMapEncoded;
	ranges::copy(out | views::transform([](int64_t c) { return (char)c; }), back_inserter(startingMapEncoded));

	istringstream pipeInput(startingMapEncoded);
	return ReadArrayMap(pipeInput);
}

static string FindFullPath(const ArrayMap2D& pipes)
{
	// Find the robot
	const map<char, Point2> directions
	{
		{ '^', Point2::Up() },
		{ 'v', Point2::Down() },
		{ '<', Point2::Left() },
		{ '>', Point2::Right() },
	};

	Point2 robotPos;
	Point2 robotDir;
	for (const auto& p : pipes.Grid())
	{
		if (directions.contains(p.second))
		{
			robotPos = p.first;
			robotDir = directions.at(p.second);
			break;
		}
	}

	// Orient the robot (note: a choice between double LL or double RR would
	// knacker up simple block search strategies, so I assume the puzzles
	// don't do that)
	auto checkAhead = [&]() { return pipes(robotPos + robotDir) == '#'; };
	auto checkLeft = [&]() { return pipes(robotPos + Point2::RotateAnticlockwise(robotDir)) == '#'; };
	auto checkRight = [&]() { return pipes(robotPos + Point2::RotateClockwise(robotDir)) == '#'; };
	assert(checkAhead() || checkLeft() || checkRight());

	string path;
	if (checkLeft())
	{
		path += 'L';
		robotDir = Point2::RotateAnticlockwise(robotDir);
	}
	else if (checkRight())
	{
		path += 'R';
		robotDir = Point2::RotateClockwise(robotDir);
	}

	while (true)
	{
		for (int step = 0; step < 100; step++)
		{
			if (checkAhead() == false)
			{
				path += to_string(step);
				break;
			}
			robotPos = robotPos + robotDir;
		}

		if (checkLeft())
		{
			path += 'L';
			robotDir = Point2::RotateAnticlockwise(robotDir);
		}
		else if (checkRight())
		{
			path += 'R';
			robotDir = Point2::RotateClockwise(robotDir);
		}
		else
		{
			break;
		}
	}

	return path;
}

static vector<string> RemoveParticle(const vector<string>& particles, const string& particle)
{
	vector<string> updatedParticles;
	for (string section : particles)
	{
		size_t pos;
		while ((pos = section.find(particle)) != string::npos)
		{
			if (pos > 0)
			{
				updatedParticles.push_back(section.substr(0, pos));
			}
			section = section.substr(pos + particle.size());
		}
		if (!section.empty())
		{
			updatedParticles.push_back(section);
		}
	}
	return updatedParticles;
}

static int64_t EncodedLength(const string& particle)
{
	int64_t encodedLength = particle.size();
	for (size_t i = 0; (i + 1) < particle.size(); i++)
	{
		encodedLength += isdigit(particle[i]) != isdigit(particle[i + 1]);
	}
	return encodedLength + 1;
}

static string EncodeMovementPiece(const string& original)
{
	string encoded;
	for (size_t i = 0; (i + 1) < original.size(); i++)
	{
		encoded += original[i];
		if (isdigit(original[i]) != isdigit(original[i + 1]))
		{
			encoded += ',';
		}
	}
	return encoded + original.back();
}

static bool Solve(const vector<string>& particles, int depth, vector<string> *answer)
{
	if (depth == 2)
	{
		bool allSame = ranges::all_of(particles | views::drop(1), [&](const string& p) { return p == particles.front(); });
		if (allSame && EncodedLength(particles.front()) <= 20)
		{
			answer->push_back(particles.front());
			return true;
		}
		return false;
	}

	for (size_t length = 10; length > 0; length--)
	{
		string candidateParticle = particles.front().substr(0, length);
		if (EncodedLength(candidateParticle) > 20)
			continue;

		answer->push_back(candidateParticle);
		vector<string> candidateParticles = RemoveParticle(particles, candidateParticle);
		if (Solve(candidateParticles, depth + 1, answer))
		{
			return true;
		}
		answer->pop_back();
	}
	return false;
}

static void Puzzle17_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	ArrayMap2D pipes = ExtractMap(ReadAsVectorOfNumbers(ReadSingleLine(input)));

	int64_t answer = 0;
	for (const auto& p : pipes.Grid() | views::filter([](const auto& p) { return p.second == '#'; }))
	{
		auto surroundingPipes = Point2::CardinalDirections()
			| views::transform([&](const Point2& dir)
				{
					return pipes(p.first + dir);
				});
		if (ranges::count(surroundingPipes, '#') == 4)
		{
			answer += p.first.X * p.first.Y;
		}
	}

	printf("[2019] Puzzle17_A: %" PRId64 "\n", answer);
}


static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> program = ReadAsVectorOfNumbers(ReadSingleLine(input));
	ArrayMap2D pipes = ExtractMap(program);
	string path = FindFullPath(pipes);

	vector<string> movementPieces;
	bool foundSolution = Solve({ path }, 0, &movementPieces);
	assert(foundSolution);
	(void)foundSolution;

	vector<size_t> movementSchedule;
	while (!path.empty())
	{
		for (size_t i = 0; i < movementPieces.size(); i++)
		{
			if (path.starts_with(movementPieces[i]))
			{
				movementSchedule.push_back(i);
				path = path.substr(movementPieces[i].size());
			}
		}
	}

	Intputer puter;
	program[0] = 2;
	puter.CopyProgram(program);

	deque<int64_t> in;
	deque<int64_t> out;
	puter.SetReadWriteQueues(&in, &out);

	for (size_t functionCall : movementSchedule)
	{
		in.push_back('A' + (char)functionCall);
		in.push_back(',');
	}
	in.back() = '\n';

	for (const string& function : movementPieces)
	{
		for (char c : EncodeMovementPiece(function))
		{
			in.push_back(c);
		}
		in.push_back('\n');
	}

	in.push_back('n');
	in.push_back('\n');

	auto exec = puter.Execute();

	assert(exec == Intputer::ExecutionResult::Finished);
	(void)exec;
	assert(out.empty() == false);

	int64_t answer = out.back();

	printf("[2019] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2019(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
