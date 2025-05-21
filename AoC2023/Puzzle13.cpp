#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle13_2023_Types
{
	struct LavaField
	{
		vector<string> Rows;
		vector<string> Columns;
	};
}

using namespace Puzzle13_2023_Types;

static vector<LavaField> ReadLavaFields(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	vector<LavaField> fields;
	fields.push_back(LavaField{});

	for (const string& line : lines)
	{
		LavaField& currentField = fields.back();
		if (line.empty())
		{
			fields.push_back(LavaField{});
		}
		else
		{
			currentField.Rows.push_back(line);
		}
	}

	for (LavaField& field : fields)
	{
		field.Columns.resize(field.Rows[0].size());

		for (size_t y = 0; y < field.Rows.size(); y++)
		{
			for (size_t x = 0; x < field.Rows[y].size(); x++)
			{
				field.Columns[x].append(1, field.Rows[y][x]);
			}
		}
	}

	return fields;
}

static bool DoesReflect(const string& line, int64_t around)
{
	string reflection = line.substr(0, around);
	reverse(reflection.begin(), reflection.end());

	string match = line.substr(around);

	if (reflection.size() > match.size())
	{
		return reflection.starts_with(match);
	}
	else
	{
		return match.starts_with(reflection);
	}
};

static bool FindUniqueReflectionIndex(const vector<string>& lines, int64_t* reflectionIndex)
{
	set<int64_t> candidates = Enumerable::Range(1ll, lines[0].size() - 1)->ToSet();
	for (const string& line : lines)
	{
		set<int64_t> testRange = candidates;
		for (int64_t i : testRange)
		{
			if (DoesReflect(line, i) == false)
			{
				candidates.erase(i);
			}
		}
	}

	if (candidates.size() == 1)
	{
		*reflectionIndex = *candidates.begin();
		return true;
	}

	return false;
}

static bool FindReflectionIndices(const vector<string>& lines, set<int64_t>* reflectionIndices)
{
	set<int64_t> candidates = Enumerable::Range(1ll, lines[0].size() - 1)->ToSet();
	for (const string& line : lines)
	{
		set<int64_t> testRange = candidates;
		for (int64_t i : testRange)
		{
			if (DoesReflect(line, i) == false)
			{
				candidates.erase(i);
			}
		}
	}

	if (candidates.size() > 0)
	{
		*reflectionIndices = candidates;
		return true;
	}

	return false;
}

static bool BruteForceRepair(const vector<string>& lines, int64_t previousSolution, int64_t* reflectionIndex)
{
	set<int64_t> solutions;

	vector<string> candidateLines = lines;
	for (string& line : candidateLines)
	{
		for (char& c : line)
		{
			set<int64_t> reflections;
			c = (c == '.' ? '#' : '.');
			if (FindReflectionIndices(candidateLines, &reflections))
			{
				solutions.insert(reflections.begin(), reflections.end());
			}
			c = (c == '.' ? '#' : '.');
		}
	}

	solutions.erase(previousSolution);

	assert(solutions.size() < 2);

	if (solutions.size() == 1)
	{
		*reflectionIndex = (*solutions.begin());
		return true;
	}

	return false;
}

static void Puzzle13_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<LavaField> fields = ReadLavaFields(input);

	int64_t answer = 0;
	for (const LavaField& field : fields)
	{
		auto eliminateCandidates = [](const vector<string>& lines, set<int64_t>* candidates)
			{
				for (const string& line : lines)
				{
					set<int64_t> testRange = *candidates;
					for (int64_t i : testRange)
					{
						if (DoesReflect(line, i) == false)
						{
							candidates->erase(i);
						}
					}
				}
			};

		set<int64_t> rowCandidates = Enumerable::Range(1ll, field.Rows[0].size() - 1)->ToSet();
		set<int64_t> columnCandidates = Enumerable::Range(1ll, field.Columns[0].size() - 1)->ToSet();

		eliminateCandidates(field.Rows, &rowCandidates);
		eliminateCandidates(field.Columns, &columnCandidates);

		assert((rowCandidates.size() + columnCandidates.size()) == 1);

		if (rowCandidates.size())
		{
			answer += *rowCandidates.begin();
		}
		else
		{
			answer += (*columnCandidates.begin()) * 100;
		}
	}

	printf("[2023] Puzzle13_A: %" PRId64 "\n", answer);
}

static void Puzzle13_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<LavaField> fields = ReadLavaFields(input);

	int64_t answer = 0;
	for (const LavaField& field : fields)
	{
		int64_t oldValue;
		if (FindUniqueReflectionIndex(field.Rows, &oldValue))
		{
			int64_t value;
			if (BruteForceRepair(field.Rows, oldValue, &value))
			{
				answer += value;
			}
			else if (BruteForceRepair(field.Columns, -1, &value))
			{
				answer += value * 100;
			}
			else
			{
				assert(false);
			}
		}
		else if (FindUniqueReflectionIndex(field.Columns, &oldValue))
		{
			int64_t value;
			if (BruteForceRepair(field.Rows, -1, &value))
			{
				answer += value;
			}
			else if (BruteForceRepair(field.Columns, oldValue, &value))
			{
				answer += value * 100;
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

	printf("[2023] Puzzle13_B: %" PRId64 "\n", answer);
}

void Puzzle13_2023(const string& filename)
{
	Puzzle13_A(filename);
	Puzzle13_B(filename);
}
