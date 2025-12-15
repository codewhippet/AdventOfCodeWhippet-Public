#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2025_Types
{
	struct Machine
	{
		uint32_t Target;
		vector<uint32_t> Buttons;
	};

	struct Counters
	{
		vector<int16_t> TargetJolts;
		vector<vector<int8_t>> Buttons;
	};
}

using namespace Puzzle10_2025_Types;

static int32_t ParseTarget(const string& line)
{
	int32_t target = 0;

	int32_t light = 1;
	for (size_t i = 1; i < line.size(); i++)
	{
		char c = line[i];
		if (c == ']')
			break;

		if (c == '#')
		{
			target |= light;
		}

		light <<= 1;
	}

	return target;
}

static int32_t ParseMachineButton(const string& match)
{
	uint32_t button = 0;
	for (const char c : match)
	{
		if (isdigit(c))
		{
			button |= 1 << (c - '0');
		}
	}
	return button;
}

static vector<uint32_t> ParseMachineButtons(const string& line)
{
	const regex buttonFormat(R"(\([^\)]+\))");

	vector<uint32_t> buttons;
	for (sregex_iterator it = sregex_iterator(line.begin(), line.end(), buttonFormat); it != sregex_iterator{}; ++it)
	{
		buttons.push_back(ParseMachineButton(it->str()));
	}

	return buttons;
}

static vector<int16_t> ParseTargetJolts(const string& line)
{
	const regex joltFormat(R"(\d+)");

	vector<int16_t> jolts;
	for (sregex_iterator it = sregex_iterator(line.begin() + line.find('{'), line.end(), joltFormat); it != sregex_iterator{}; ++it)
	{
		jolts.push_back((int16_t)stoi(it->str()));
	}

	return jolts;
}

static Machine ParseMachine(const string& line)
{
	Machine m;
	m.Target = ParseTarget(line);
	m.Buttons = ParseMachineButtons(line);
	return m;
}

static vector<int8_t> ParseCounterButton(const string& match)
{
	vector<int8_t> button;
	for (const char c : match)
	{
		if (isdigit(c))
		{
			button.push_back(c - '0');
		}
	}
	return button;
}

static vector<vector<int8_t>> ParseCounterButtons(const string& line)
{
	const regex buttonFormat(R"(\([^\)]+\))");

	vector<vector<int8_t>> buttons;
	for (sregex_iterator it = sregex_iterator(line.begin(), line.end(), buttonFormat); it != sregex_iterator{}; ++it)
	{
		buttons.push_back(ParseCounterButton(it->str()));
	}

	return buttons;
}

static Counters ParseCounters(const string& line)
{
	Counters c;
	c.TargetJolts = ParseTargetJolts(line);
	c.Buttons = ParseCounterButtons(line);
	return c;
}

static void Scale(vector<int64_t>* v, int64_t s)
{
	ranges::for_each(*v, [s](int64_t& i) { i *= s; });
}

static vector<vector<int64_t>> CountersToAugmentedMatrix(const Counters& c)
{
	vector<vector<int64_t>> m(c.TargetJolts.size(), vector<int64_t>(c.Buttons.size() + 1));

	// Each button is a column
	for (int64_t column = 0; column < (int64_t)c.Buttons.size(); column++)
	{
		for (int8_t row : c.Buttons[column])
		{
			m[row][column] = 1;
		}
	}

	for (int64_t i = 0; i < (int64_t)c.TargetJolts.size(); i++)
	{
		m[i].back() = c.TargetJolts[i];
	}

	return m;
}

static vector<int64_t> CountersToConstraints(const Counters& c)
{
	vector<int64_t> constraints(c.Buttons.size(), numeric_limits<int64_t>::max());
	for (int64_t button = 0; button < (int64_t)c.Buttons.size(); button++)
	{
		for (int8_t counter : c.Buttons[button])
		{
			constraints[button] = min<int64_t>(constraints[button], c.TargetJolts[counter]);
		}
	}
	return constraints;
}

static bool IsNonZeroRow(const vector<int64_t>& v)
{
	return ranges::count(v, 0) != (int64_t)v.size();
}

static bool IsZeroRow(const vector<int64_t>& v)
{
	return ranges::count(v, 0) == (int64_t)v.size();
}

static vector<int64_t> Reduce(vector<int64_t> rowToReduce, vector<int64_t> reducingRow, int64_t reducingColumn)
{
	if (rowToReduce[reducingColumn] == 0)
	{
		// Nothing to do
		return rowToReduce;
	}

	assert(reducingRow[reducingColumn] > 0);
	if (rowToReduce[reducingColumn] < 0)
	{
		Scale(&rowToReduce, -1);
	}

	int64_t scaleTo = lcm(rowToReduce[reducingColumn], reducingRow[reducingColumn]);
	Scale(&rowToReduce, scaleTo / rowToReduce[reducingColumn]);
	Scale(&reducingRow, scaleTo / reducingRow[reducingColumn]);
	assert(rowToReduce[reducingColumn] == reducingRow[reducingColumn]);

	for (int i = 0; i < (int64_t)rowToReduce.size(); i++)
	{
		rowToReduce[i] -= reducingRow[i];
	}

	return rowToReduce;
}

static void ReduceAndTrim(vector<vector<int64_t>>* pm)
{
	vector<vector<int64_t>>& m = *pm;
	int64_t diagonalEnd = min<int64_t>(m.size(), m.front().size() - 1);
	for (int64_t diagonal = 0; diagonal < diagonalEnd; diagonal++)
	{
		// Put empty rows at the bottom
		partition(m.begin() + diagonal, m.end(), IsNonZeroRow);

		// Find a row with a non-zero element in the column
		for (int64_t reducingRow = diagonal; reducingRow < (int64_t)m.size(); reducingRow++)
		{
			if (m[reducingRow][diagonal] != 0)
			{
				swap(m[diagonal], m[reducingRow]);
				break;
			}
		}

		if (m[diagonal][diagonal] < 0)
		{
			Scale(&m[diagonal], -1);
		}

		for (int64_t rowToReduce = diagonal + 1; rowToReduce < (int64_t)m.size(); rowToReduce++)
		{
			m[rowToReduce] = Reduce(m[rowToReduce], m[diagonal], diagonal);
		}
	}

	// Get rid of the empty rows
	m.erase(remove_if(m.begin(), m.end(), IsZeroRow), m.end());
}

static void TrySolvedConstrained(const Counters& counters,
	const vector<vector<int64_t>>& m,
	int64_t rowToSolve,
	int64_t nextUnknown,
	const vector<int64_t>& constraints,
	vector<int64_t>* alreadyAssigned,
	int64_t *minimumPresses)
{
	vector<int64_t>& solution = *alreadyAssigned;

	if (nextUnknown == -1)
	{
		vector<int16_t> accumulatedJolts(counters.TargetJolts.size(), 0);
		for (int64_t button = 0; button < (int64_t)counters.Buttons.size(); button++)
		{
			for (int8_t counter : counters.Buttons[button])
			{
				accumulatedJolts[counter] += (int16_t)solution[button];
			}
		}

		if (accumulatedJolts == counters.TargetJolts)
		{
			*minimumPresses = min(*minimumPresses, ranges::fold_left(solution, 0, plus{}));
		}

		return;
	}

	// If the matrix isn't big enough we're going to need to guess
	if (nextUnknown > rowToSolve)
	{
		for (int64_t guess = 0; guess <= constraints[nextUnknown]; guess++)
		{
			solution[nextUnknown] = guess;
			TrySolvedConstrained(counters, m, rowToSolve, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
		}
		return;
	}

	assert(rowToSolve == nextUnknown);
	if (m[rowToSolve][nextUnknown] == 0)
	{
		// We're not able to solve directly so we need to guess
		for (int64_t guess = 0; guess <= constraints[nextUnknown]; guess++)
		{
			solution[nextUnknown] = guess;
			TrySolvedConstrained(counters, m, rowToSolve - 1, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
		}
		return;
	}

	int64_t rowTargetSum = m[rowToSolve].back();

	// Eliminate everything we already know about
	for (int64_t known = nextUnknown + 1; known < (int64_t)solution.size(); known++)
	{
		rowTargetSum -= m[rowToSolve][known] * solution[known];
		//m[rowToSolve][known] = 0; // For debugging sanity
	}

	// Do we have a valid integer solution?
	if ((rowTargetSum % m[rowToSolve][nextUnknown]) != 0)
	{
		// We don't have a valid integer solution, probably an incorrect guess from earlier, so we should bail out
		return;
	}

	int64_t tentativeSolution = rowTargetSum / m[rowToSolve][nextUnknown];
	if (tentativeSolution < 0)
	{
		// We're only looking for positive solutions
		return;
	}

	solution[nextUnknown] = tentativeSolution;

	TrySolvedConstrained(counters, m, rowToSolve - 1, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
}

static void SolveConstrained(const Counters& counters, const vector<vector<int64_t>>& m, const vector<int64_t>& constraints, int64_t *minimumPresses)
{
	vector<int64_t> solution(constraints.size(), -1);
	TrySolvedConstrained(counters, m, min(solution.size() - 1, m.size() - 1), solution.size() - 1, constraints, &solution, minimumPresses);
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	string line;
	while (getline(input, line))
	{
		Machine m = ParseMachine(line);

		int32_t minimumPresses = numeric_limits<int32_t>::max();

		uint32_t combinationEnd = (1 << m.Buttons.size());
		for (uint32_t combination = 1; combination < combinationEnd; combination++)
		{
			uint32_t c = combination;
			uint32_t lights = 0;
			for (int64_t buttonIndex = 0; buttonIndex < (int64_t)m.Buttons.size(); buttonIndex++)
			{
				if (c & 1)
				{
					lights ^= m.Buttons[buttonIndex];
				}
				c >>= 1;
			}
			if (lights == m.Target)
			{
				minimumPresses = min(minimumPresses, popcount(combination));
			}
		}

		answer += minimumPresses;
	}

	printf("[2025] Puzzle10_A: %" PRId64 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	string line;
	while (getline(input, line))
	{
		Counters counters = ParseCounters(line);

		vector<vector<int64_t>> matrix;
		while (true)
		{
			matrix = CountersToAugmentedMatrix(counters);
			ReduceAndTrim(&matrix);

			bool allLeadingNonZero = true;
			for (int i = 0; i < (int)matrix.size(); i++)
			{
				if (matrix[i][i] == 0)
				{
					allLeadingNonZero = false;
					break;
				}
			}

			if (allLeadingNonZero)
				break;

			for (int i = 0; i < (int)counters.Buttons.size(); i++)
			{
				swap(counters.Buttons[i], counters.Buttons[rand() % (int)counters.Buttons.size()]);
			}
		}

		auto constraints = CountersToConstraints(counters);

		int64_t minimumPresses = numeric_limits<int64_t>::max();
		SolveConstrained(counters, matrix, constraints, &minimumPresses);

		assert(minimumPresses < numeric_limits<int64_t>::max());

		answer += minimumPresses;
	}

	printf("[2025] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2025(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
