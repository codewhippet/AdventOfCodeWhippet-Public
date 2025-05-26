#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2023_Types
{
	enum class OperationType
	{
		Remove,
		Set,
	};

	struct Operation
	{
		string Label;
		int Power;
		OperationType Type;
	};
}

using namespace Puzzle15_2023_Types;

static int64_t Hash(const char* s)
{
	int64_t h = 0;
	for (/***/; *s != '\0'; s++)
	{
		h += *s;
		h *= 17;
		h %= 256;
	}
	return h;
}

static vector<string> ReadPuzzle(istream& input)
{
	vector<string> ret;

	char buffer[1024];
	while (input.getline(&buffer[0], sizeof(buffer), ','))
	{
		string part{ buffer };
		if (part.empty() == false)
		{
			ret.push_back(part);
		}
	}

	if (ret.back().ends_with('\n'))
	{
		ret.back() = ret.back().substr(0, ret.back().size() - 1);
	}

	return ret;
}

static Operation DecodeOperation(const string& s)
{
	size_t equalLocation = s.find('=');
	if (equalLocation == string::npos)
	{
		return Operation{ s.substr(0, s.size() - 1), -1, OperationType::Remove };
	}
	else
	{
		return Operation{ s.substr(0, equalLocation), stoi(s.substr(equalLocation + 1)), OperationType::Set };
	}
}

static void Puzzle15_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	assert(Hash("HASH") == 52);

	vector<string> puzzle = ReadPuzzle(input);

	int64_t answer = MakeEnumerator(puzzle)
		->Select<int64_t>([](const string& s) { return Hash(s.c_str()); })
		->Sum();

	printf("[2023] Puzzle15_A: %" PRId64 "\n", answer);
}

static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> puzzle = ReadPuzzle(input);

	vector<Operation> operations = MakeEnumerator(puzzle)
		->Select<Operation>([](const string& s) { return DecodeOperation(s); })
		->ToVector();

	vector<list<pair<string, int>>> boxes;
	boxes.resize(256);

	for (const Operation op : operations)
	{
		int64_t boxIndex = Hash(op.Label.c_str());
		list<pair<string, int>>& boxContents = boxes[boxIndex];
		switch (op.Type)
		{
		case OperationType::Remove:
			{
				boxContents.remove_if([&op](const auto& lens) { return lens.first == op.Label; });
			}
			break;

		case OperationType::Set:
			{
				auto existingLens = find_if(boxContents.begin(), boxContents.end(),
					[&op](const auto& lens) { return lens.first == op.Label; });
				if (existingLens != boxContents.end())
				{
					existingLens->second = op.Power;
				}
				else
				{
					boxContents.push_back(make_pair(op.Label, op.Power));
				}
			}
			break;
		}
	}

	int64_t answer = 0;

	for (int64_t boxIndex = 0; boxIndex < (int64_t)boxes.size(); boxIndex++)
	{
		int64_t boxPower = boxIndex + 1;

		int64_t slotIndex = 0;
		for (list<pair<string, int>>::const_reference lens : boxes[boxIndex])
		{
			slotIndex++;

			int64_t lensPower = boxPower * slotIndex * lens.second;
			answer += lensPower;
		}
	}

	printf("[2023] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2023(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}
