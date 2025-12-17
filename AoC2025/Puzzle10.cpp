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
		vector<int32_t> TargetJolts;
		vector<vector<int32_t>> Buttons;
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

static vector<int32_t> ParseTargetJolts(const string& line)
{
	const regex joltFormat(R"(\d+)");

	vector<int32_t> jolts;
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

static vector<int32_t> ParseCounterButton(const string& match)
{
	vector<int32_t> button;
	for (const char c : match)
	{
		if (isdigit(c))
		{
			button.push_back(c - '0');
		}
	}
	return button;
}

static vector<vector<int32_t>> ParseCounterButtons(const string& line)
{
	const regex buttonFormat(R"(\([^\)]+\))");

	vector<vector<int32_t>> buttons;
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

static void Scale(vector<int32_t>* v, int32_t s)
{
	ranges::for_each(*v, [s](int32_t& i) { i *= s; });
}

static vector<vector<int32_t>> CountersToAugmentedMatrix(const Counters& c)
{
	vector<vector<int32_t>> m(c.TargetJolts.size(), vector<int32_t>(c.Buttons.size() + 1));

	// Each button is a column
	for (size_t column = 0; column < c.Buttons.size(); column++)
	{
		for (int32_t row : c.Buttons[column])
		{
			m[row][column] = 1;
		}
	}

	for (size_t i = 0; i < c.TargetJolts.size(); i++)
	{
		m[i].back() = c.TargetJolts[i];
	}

	return m;
}

static vector<int32_t> CountersToConstraints(const Counters& c)
{
	vector<int32_t> constraints(c.Buttons.size(), numeric_limits<int32_t>::max());
	for (size_t button = 0; button < c.Buttons.size(); button++)
	{
		for (int32_t counter : c.Buttons[button])
		{
			constraints[button] = min<int32_t>(constraints[button], c.TargetJolts[counter]);
		}
	}
	return constraints;
}

static bool IsNonZeroRow(const vector<int32_t>& v)
{
	return ranges::count(v, 0) != (int32_t)v.size();
}

static bool IsZeroRow(const vector<int32_t>& v)
{
	return ranges::count(v, 0) == (int32_t)v.size();
}

static vector<int32_t> Reduce(vector<int32_t> rowToReduce, vector<int32_t> reducingRow, size_t reducingColumn)
{
	if (rowToReduce[reducingColumn] == 0)
	{
		// Nothing to do
		return rowToReduce;
	}

	// Make sure both rows have a positive leading value
	assert(reducingRow[reducingColumn] > 0);
	if (rowToReduce[reducingColumn] < 0)
	{
		Scale(&rowToReduce, -1);
	}

	int32_t scaleTo = lcm(rowToReduce[reducingColumn], reducingRow[reducingColumn]);
	Scale(&rowToReduce, scaleTo / rowToReduce[reducingColumn]);
	Scale(&reducingRow, scaleTo / reducingRow[reducingColumn]);
	assert(rowToReduce[reducingColumn] == reducingRow[reducingColumn]);

	for (size_t i = 0; i < rowToReduce.size(); i++)
	{
		rowToReduce[i] -= reducingRow[i];
	}

	return rowToReduce;
}

static bool ReduceAndTrim(vector<vector<int32_t>>* pm)
{
	vector<vector<int32_t>> m = *pm;

	size_t diagonalEnd = min(m.size(), m.front().size() - 1);
	for (size_t diagonal = 0; diagonal < diagonalEnd; diagonal++)
	{
		// Put empty rows at the bottom; might cause us to stop reducing early
		auto endOfNonZeroRows = partition(m.begin() + diagonal, m.end(), IsNonZeroRow);
		if (diagonal >= (size_t)distance(m.begin(), endOfNonZeroRows))
			break;

		// Find a row with a non-zero element in the column
		for (size_t reducingRow = diagonal; reducingRow < m.size(); reducingRow++)
		{
			if (m[reducingRow][diagonal] != 0)
			{
				swap(m[diagonal], m[reducingRow]);
				break;
			}
		}

		if (m[diagonal][diagonal] == 0)
		{
			// Could not find a non-zero pivot
			return false;
		}

		if (m[diagonal][diagonal] < 0)
		{
			Scale(&m[diagonal], -1);
		}

		for (size_t rowToReduce = diagonal + 1; rowToReduce < m.size(); rowToReduce++)
		{
			m[rowToReduce] = Reduce(m[rowToReduce], m[diagonal], diagonal);
		}
	}

	// Get rid of the empty rows
	m.erase(remove_if(m.begin(), m.end(), IsZeroRow), m.end());

	*pm = m;
	return true;
}

static void TrySolvedConstrained(const vector<vector<int32_t>>& m,
	size_t rowToSolve,
	size_t nextUnknown,
	const vector<int32_t>& constraints,
	vector<int32_t>* alreadyAssigned,
	int32_t *minimumPresses)
{
	vector<int32_t>& solution = *alreadyAssigned;

	if ((nextUnknown + 1) == 0)
	{
		*minimumPresses = min(*minimumPresses, ranges::fold_left(solution, 0, plus{}));
		return;
	}

	// If the matrix isn't big enough we're going to need to guess
	if (nextUnknown > rowToSolve)
	{
		for (int32_t guess = 0; guess <= constraints[nextUnknown]; guess++)
		{
			solution[nextUnknown] = guess;
			TrySolvedConstrained(m, rowToSolve, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
		}
		return;
	}

	assert(rowToSolve == nextUnknown);
	if (m[rowToSolve][nextUnknown] == 0)
	{
		// We're not able to solve directly so we need to guess
		for (int32_t guess = 0; guess <= constraints[nextUnknown]; guess++)
		{
			solution[nextUnknown] = guess;
			TrySolvedConstrained(m, rowToSolve - 1, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
		}
		return;
	}

	int32_t rowTargetSum = m[rowToSolve].back();

	// Eliminate everything we already know about
	for (size_t known = nextUnknown + 1; known < solution.size(); known++)
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

	int32_t tentativeSolution = rowTargetSum / m[rowToSolve][nextUnknown];
	if (tentativeSolution < 0)
	{
		// We're only looking for positive solutions
		return;
	}

	solution[nextUnknown] = tentativeSolution;

	TrySolvedConstrained(m, rowToSolve - 1, nextUnknown - 1, constraints, alreadyAssigned, minimumPresses);
}

static void SolveConstrained(const vector<vector<int32_t>>& m, const vector<int32_t>& constraints, int32_t* minimumPresses)
{
	vector<int32_t> solution(constraints.size(), -1);
	TrySolvedConstrained(m, min(solution.size() - 1, m.size() - 1), solution.size() - 1, constraints, &solution, minimumPresses);
}

static void Puzzle10_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int32_t answer = 0;

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
			for (int32_t buttonIndex = 0; buttonIndex < (int32_t)m.Buttons.size(); buttonIndex++)
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

	printf("[2025] Puzzle10_A: %" PRId32 "\n", answer);
}

static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int32_t answer = 0;

	string line;
	while (getline(input, line))
	{
		Counters counters = ParseCounters(line);

		vector<vector<int32_t>> matrix = CountersToAugmentedMatrix(counters);
		vector<int32_t> constraints = CountersToConstraints(counters);

		while (ReduceAndTrim(&matrix) == false)
		{
			for (size_t buttonToSwap = 0; buttonToSwap < counters.Buttons.size(); buttonToSwap++)
			{
				size_t swapWith = rand() % counters.Buttons.size();
				swap(constraints[buttonToSwap], constraints[swapWith]);

				for (auto& row : matrix)
				{
					swap(row[buttonToSwap], row[swapWith]);
				}
			}
		}			

		int32_t minimumPresses = numeric_limits<int32_t>::max();
		SolveConstrained(matrix, constraints, &minimumPresses);
		assert(minimumPresses < numeric_limits<int32_t>::max());

		answer += minimumPresses;
	}

	printf("[2025] Puzzle10_B: %" PRId32 "\n", answer);
}

void Puzzle10_2025(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
