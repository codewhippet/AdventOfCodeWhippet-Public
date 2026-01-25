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

	enum class ConstraintType
	{
		Equal,
		LessOrEqual,
		GreaterOrEqual,
	};

	struct Constraint
	{
		vector<size_t> Variables;
		ConstraintType Type;
		double Value;
	};

	struct IlpProblem
	{
		size_t NumVariables;
		vector<Constraint> Constraints;
	};

	struct IlpSolution
	{
		vector<double> VariableValues;
		double ObjectiveValue = 0;
		bool Feasible = true;
	};

	enum class VariableType
	{
		Real,
		Slack,
		Artificial,
		RHS,
	};

	struct AugmentedConstraint
	{
		pair<VariableType, size_t> Basis;
		vector<tuple<VariableType, size_t, double>> Variables;
		double Value;
	};

	// Notation used from https://www.pmcalculators.com/simplex-method-calculator/
	// Most reliable online calculator for checking: https://www.emathhelp.net/calculators/linear-programming/simplex-method-calculator/
	struct Tableau
	{
		size_t NumVariables = 0;
		size_t NumSlackVariables = 0;
		size_t NumArtificialVariables = 0;
		size_t RHS = 0; // Should always be the last entry in the row. Here for convenience

		vector<VariableType> VariableTypes;

		vector<int32_t> Cj;

		vector<size_t> Basis;
		vector<int32_t> Cb;

		vector<vector<double>> Matrix;

		vector<double> Z;

		bool Feasible = true;
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

static IlpProblem ParseProblem(const string& line)
{
	IlpProblem p;

	const regex joltFormat(R"(\d+)");
	for (sregex_iterator it = sregex_iterator(line.begin() + line.find('{'), line.end(), joltFormat); it != sregex_iterator{}; ++it)
	{
		p.Constraints.push_back({ {}, ConstraintType::Equal, stod(it->str()) });
	}

	const regex buttonFormat(R"(\([^\)]+\))");
	size_t buttonIndex = 0;
	for (sregex_iterator it = sregex_iterator(line.begin(), line.end(), buttonFormat); it != sregex_iterator{}; ++it)
	{
		for (const char c : it->str())
		{
			if (isdigit(c))
			{
				p.Constraints[(c - '0')].Variables.push_back(buttonIndex);
			}
		}
		buttonIndex++;
	}

	p.NumVariables = buttonIndex;

	return p;
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

static const double ARBITRARY_SMALL_NUMBER = 0.0001;

static bool IsIntegerIsh(double d)
{
	bool isCloseToInteger = (fabs(d - round(d)) < ARBITRARY_SMALL_NUMBER); // Arbitrary small number
	return isCloseToInteger;
}

static bool IsZeroIsh(double d)
{
	return fabs(d) < ARBITRARY_SMALL_NUMBER;
}

static void AllocateTableauStorage(Tableau *t, size_t matrixHeight)
{
	const size_t matrixWidth = t->NumVariables + t->NumSlackVariables + t->NumArtificialVariables + 1;

	t->VariableTypes = vector<VariableType>(matrixWidth);
	t->Cj = vector<int32_t>(matrixWidth);
	t->Basis = vector<size_t>(matrixHeight);
	t->Cb = vector<int32_t>(t->Basis.size());
	t->Matrix = vector<vector<double>>(t->Basis.size(), vector<double>(t->Cj.size(), 0.0));
	t->Z = vector<double>(t->Matrix.front().size());
}

static Tableau CreateTableau(const IlpProblem& problem)
{
	Tableau t;
	t.NumVariables = problem.NumVariables;
	t.NumSlackVariables = 0;
	t.NumArtificialVariables = 0;

	// Count to see if any variables can be used as a basis
	vector<size_t> variableUseCount = vector<size_t>(problem.NumVariables);
	for (const Constraint& c : problem.Constraints)
	{
		for (size_t variable : c.Variables)
		{
			variableUseCount[variable]++;
		}
	}

	// Augment the constraints with slack variables and artificial variables
	vector<AugmentedConstraint> augmentedConstraints(problem.Constraints.size());
	for (size_t row = 0; row < problem.Constraints.size(); row++)
	{
		const Constraint& sourceConstraint = problem.Constraints[row];
		AugmentedConstraint& destConstraint = augmentedConstraints[row];

		// Copy across the real variables
		ranges::copy(sourceConstraint.Variables | views::transform([](size_t v) { return tuple<VariableType, size_t, double>{ VariableType::Real, v, 1 }; }),
			back_inserter(destConstraint.Variables));

		// Check to see if there's a real variable we might be able to use as a basis
		const size_t invalidBasis = numeric_limits<size_t>::max();
		auto candidateBasisIt = ranges::find_if(sourceConstraint.Variables, [&](size_t v) { return variableUseCount[v] == 1; });
		size_t candidateBasis = (candidateBasisIt == sourceConstraint.Variables.end() ? invalidBasis : *candidateBasisIt);

		switch (sourceConstraint.Type)
		{
		case ConstraintType::LessOrEqual:
			if (candidateBasis == invalidBasis)
			{
				const size_t newSlackVariable = t.NumSlackVariables++;
				destConstraint.Variables.push_back({ VariableType::Slack, newSlackVariable, 1 });
				destConstraint.Basis = { VariableType::Slack, newSlackVariable };
			}
			else
			{
				destConstraint.Basis = { VariableType::Real, candidateBasis };
			}
			break;
		case ConstraintType::Equal:
			if (candidateBasis == invalidBasis)
			{
				const size_t newArtificialVariable = t.NumArtificialVariables++;
				destConstraint.Variables.push_back({ VariableType::Artificial, newArtificialVariable, 1 });
				destConstraint.Basis = { VariableType::Artificial, newArtificialVariable };
			}
			else
			{
				destConstraint.Basis = { VariableType::Real, candidateBasis };
			}
			break;
		case ConstraintType::GreaterOrEqual:
			{
				const size_t newSlackVariable = t.NumSlackVariables++;
				const size_t newArtificialVariable = t.NumArtificialVariables++;
				destConstraint.Variables.push_back({ VariableType::Slack, newSlackVariable, -1 });
				destConstraint.Variables.push_back({ VariableType::Artificial, newArtificialVariable, 1 });
				destConstraint.Basis = { VariableType::Artificial, newArtificialVariable };
			}
			break;
		}

		destConstraint.Value = sourceConstraint.Value;
	}

	t.RHS = t.NumVariables + t.NumSlackVariables + t.NumArtificialVariables;
	AllocateTableauStorage(&t, problem.Constraints.size());

	const map<VariableType, size_t> variableTypeOffset =
	{
		{ VariableType::Real, 0 },
		{ VariableType::Slack, t.NumVariables },
		{ VariableType::Artificial, t.NumVariables + t.NumSlackVariables },
		{ VariableType::RHS, t.RHS }
	};

	// Variable types
	for (size_t i = 0; i < t.VariableTypes.size(); i++)
	{
		if (i < t.NumVariables)
		{
			t.VariableTypes[i] = VariableType::Real;
		}
		else if (i < (t.NumVariables + t.NumSlackVariables))
		{
			t.VariableTypes[i] = VariableType::Slack;
		}
		else if (i < (t.NumVariables + t.NumSlackVariables + t.NumArtificialVariables))
		{
			t.VariableTypes[i] = VariableType::Artificial;
		}
		else
		{
			t.VariableTypes[i] = VariableType::RHS;
		}
	}

	// Objective function
	for (size_t i = 0; i < t.Cj.size(); i++)
	{
		t.Cj[i] = (t.VariableTypes[i] == VariableType::Artificial ? 1 : 0);
	}

	// Main data
	for (size_t row = 0; row < augmentedConstraints.size(); row++)
	{
		// Coefficients
		for (const auto& variable : augmentedConstraints[row].Variables)
		{
			const size_t variableOffset = variableTypeOffset.at(get<0>(variable));
			t.Matrix[row][get<1>(variable) + variableOffset] = get<2>(variable);
		}

		// Values
		t.Matrix[row][t.RHS] = augmentedConstraints[row].Value;

		// Basis
		t.Basis[row] = augmentedConstraints[row].Basis.second + variableTypeOffset.at(augmentedConstraints[row].Basis.first);
		t.Cb[row] = t.Cj[t.Basis[row]];
	}

	return t;
}

static Tableau TrimTableau(const Tableau& t1)
{
	Tableau t2;
	t2.NumVariables = t1.NumVariables;
	t2.NumSlackVariables = t1.NumSlackVariables;
	t2.NumArtificialVariables = 0;
	t2.RHS = t2.NumVariables + t2.NumSlackVariables + t2.NumArtificialVariables;

	const size_t matrixHeight = ranges::count_if(t1.Basis, [&](size_t b) { return t1.VariableTypes[b] != VariableType::Artificial; });
	AllocateTableauStorage(&t2, matrixHeight);

	ranges::copy_n(t1.VariableTypes.begin(), t1.NumVariables + t1.NumSlackVariables, t2.VariableTypes.begin());

	// Real variable coefficients
	for (size_t column = 0; column < t2.NumVariables; column++)
	{
		t2.Cj[column] = 1;
	}

	// Sub-matrix
	const size_t artificialVariableOffset = t1.NumVariables + t1.NumSlackVariables;
	const size_t numNonArtificialVariables = t1.NumVariables + t1.NumSlackVariables;
	size_t destRow = 0;
	for (size_t sourceRow = 0; sourceRow < t1.Matrix.size(); sourceRow++)
	{
		if (t1.Basis[sourceRow] < artificialVariableOffset)
		{
			ranges::copy_n(t1.Matrix[sourceRow].begin(), numNonArtificialVariables, t2.Matrix[destRow].begin());
			t2.Matrix[destRow].back() = t1.Matrix[sourceRow].back();

			t2.Basis[destRow] = t1.Basis[sourceRow];
			t2.Cb[destRow] = t2.Cj[t2.Basis[destRow]];

			destRow++;
		}
	}

	// Basis coefficients
	for (size_t row = 0; row < t2.Cb.size(); row++)
	{
		t2.Cb[row] = t2.Cj[t2.Basis[row]];
	}

	return t2;
}

static void PivotTableau(Tableau* tableau, size_t pivotColumn, size_t pivotRow)
{
	Tableau& t = *tableau;

	// Normalise the pivot row
	{
		double scale = 1.0 / t.Matrix[pivotRow][pivotColumn];
		for (size_t column = 0; column < t.Matrix[pivotRow].size(); column++)
		{
			t.Matrix[pivotRow][column] *= scale;
		}
		t.Matrix[pivotRow][pivotColumn] = 1.0; // Force value to stop inaccuracies buiding up
	}

	for (size_t row = 0; row < t.Matrix.size(); row++)
	{
		if (row == pivotRow)
			continue;

		if (t.Matrix[row][pivotColumn] != 0)
		{
			double rowScale = t.Matrix[row][pivotColumn];
			for (size_t column = 0; column < t.Matrix[row].size(); column++)
			{
				t.Matrix[row][column] -= t.Matrix[pivotRow][column] * rowScale;
			}
			t.Matrix[row][pivotColumn] = 0.0; // Force value to stop inaccuracies buiding up
		}
	}

	t.Basis[pivotRow] = pivotColumn;
	t.Cb[pivotRow] = t.Cj[pivotColumn];
}

static void ScoreSolution(Tableau* tableau)
{
	Tableau& t = *tableau;

	// Score the solution
	for (size_t column = 0; column < t.Z.size(); column++)
	{
		t.Z[column] = 0;
		for (size_t row = 0; row < t.Matrix.size(); row++)
		{
			t.Z[column] += t.Matrix[row][column] * t.Cb[row];
		}
		t.Z[column] -= t.Cj[column];
	}
}

static size_t PickPivotColumn(Tableau* tableau)
{
	Tableau& t = *tableau;

	size_t pivotColumn = numeric_limits<size_t>::max();
	for (size_t column = 0; column + 1 < t.Z.size(); column++)
	{
		if ((t.Z[column] - ARBITRARY_SMALL_NUMBER) > 0)
		{
			if ((pivotColumn == numeric_limits<size_t>::max()) ||
				(t.Z[column] > t.Z[pivotColumn]))
			{
				pivotColumn = column;
			}
		}
	}

	return pivotColumn;
}

static size_t PickPivotRow(Tableau* tableau, size_t pivotColumn)
{
	Tableau& t = *tableau;

	size_t pivotRow = numeric_limits<size_t>::max();
	for (size_t row = 0; row < t.Matrix.size(); row++)
	{
		if ((t.Matrix[row][pivotColumn] - ARBITRARY_SMALL_NUMBER) > 0)
		{
			if ((pivotRow == numeric_limits<size_t>::max()) ||
				((t.Matrix[row].back() / t.Matrix[row][pivotColumn]) < (t.Matrix[pivotRow].back() / t.Matrix[pivotRow][pivotColumn])))
			{
				pivotRow = row;
			}
		}
	}

	return pivotRow;
}

static bool StepTableau(Tableau* tableau)
{
	Tableau& t = *tableau;

	// Can we improve the solution?
	size_t pivotColumn = PickPivotColumn(tableau);
	if (pivotColumn >= t.Z.size() - 1)
	{
		// There should be no artificial variables left in the base for feasible solutions
		for (size_t i = 0; i < t.Basis.size(); i++)
		{
			const bool isArtificialVariable = (t.VariableTypes[t.Basis[i]] == VariableType::Artificial);
			const bool isNonZero = !IsZeroIsh(t.Matrix[i].back());
			if (isArtificialVariable && isNonZero)
			{
				t.Feasible = false;
			}
		}

		return false;
	}

	// Pick the pivot row
	size_t pivotRow = PickPivotRow(tableau, pivotColumn);

	PivotTableau(tableau, pivotColumn, pivotRow);
	ScoreSolution(tableau);

	return true;
}

static void PivotArtificialVariables(Tableau* tableau)
{
	Tableau& t = *tableau;

	const size_t numNonArtificialVariables = t.NumVariables + t.NumSlackVariables;

	vector<bool> isVariableInBase(numNonArtificialVariables, false);
	for (size_t i = 0; i < t.Basis.size(); i++)
	{
		if (t.Basis[i] < isVariableInBase.size())
		{
			isVariableInBase[t.Basis[i]] = true;
		}
	}

	vector<size_t> variablesNotInBase;
	variablesNotInBase.reserve(isVariableInBase.size());
	for (size_t i = 0; i < numNonArtificialVariables; i++)
	{
		if (isVariableInBase[i] == false)
		{
			variablesNotInBase.push_back(i);
		}
	}

	if (variablesNotInBase.empty())
		return;

	// Try to pivot non-artificial variables into the solution
	for (size_t pivotColumn : variablesNotInBase)
	{
		for (size_t pivotRow = 0; pivotRow < t.Basis.size(); pivotRow++)
		{
			const bool isArtificialVariable = t.Basis[pivotRow] >= numNonArtificialVariables;
			const bool canPivot = !IsZeroIsh(t.Matrix[pivotRow][pivotColumn]);
			if (isArtificialVariable && canPivot)
			{
				PivotTableau(tableau, pivotColumn, pivotRow);
			}
		}
	}
}

static IlpSolution Solve(const IlpProblem& p)
{
	IlpSolution s;

	Tableau t1 = CreateTableau(p);
	ScoreSolution(&t1);
	while (StepTableau(&t1))
		;

	if (!t1.Feasible)
	{
		s.Feasible = false;
		return s;
	}

	PivotArtificialVariables(&t1);
	ScoreSolution(&t1);

	Tableau t2 = TrimTableau(t1);
	ScoreSolution(&t2);

	while (StepTableau(&t2))
		;

	assert(t2.Feasible);

	s.VariableValues = vector<double>(t2.NumVariables);
	for (size_t row = 0; row < t2.Basis.size(); row++)
	{
		const size_t base = t2.Basis[row];
		if (base < s.VariableValues.size())
		{
			if (IsZeroIsh(t2.Matrix[row][base]))
			{
				s.VariableValues[base] = 0;
			}
			else
			{
				s.VariableValues[base] = t2.Matrix[row].back() / t2.Matrix[row][base];
			}
		}
	}

	s.ObjectiveValue = ranges::fold_left(s.VariableValues, 0, plus{});

	return s;
}

static void AddOrUpdateUpperBoundCuttingPlane(IlpProblem* p, size_t variable, double value)
{
	for (Constraint& c : p->Constraints)
	{
		const bool affectsThisVariableOnly = (c.Variables.size() == 1) && (c.Variables.front() == variable);
		if ((c.Type == ConstraintType::LessOrEqual) && affectsThisVariableOnly)
		{
			c.Value = min(c.Value, floor(value));
			return;
		}
	}

	p->Constraints.push_back({ { variable }, ConstraintType::LessOrEqual, floor(value) });
}

static void AddOrUpdateLowerBoundCuttingPlane(IlpProblem* p, size_t variable, double value)
{
	for (Constraint& c : p->Constraints)
	{
		const bool affectsThisVariableOnly = (c.Variables.size() == 1) && (c.Variables.front() == variable);
		if ((c.Type == ConstraintType::GreaterOrEqual) && affectsThisVariableOnly)
		{
			c.Value = max(c.Value, ceil(value));
			return;
		}
	}

	p->Constraints.push_back({ { variable }, ConstraintType::GreaterOrEqual, ceil(value) });
}

static bool AreCuttingPlanesConsistent(const IlpProblem& p)
{
	vector<double> upperBounds(p.NumVariables, numeric_limits<double>::max());
	vector<double> lowerBounds(p.NumVariables, 0);

	for (const Constraint& c : p.Constraints)
	{
		if (c.Variables.size() == 1)
		{
			if (c.Type == ConstraintType::LessOrEqual)
			{
				upperBounds[c.Variables.front()] = c.Value + ARBITRARY_SMALL_NUMBER;
			}
			else if (c.Type == ConstraintType::GreaterOrEqual)
			{
				lowerBounds[c.Variables.front()] = c.Value - ARBITRARY_SMALL_NUMBER;
			}
		}
	}

	for (size_t i = 0; i < upperBounds.size(); i++)
	{
		if (upperBounds[i] < lowerBounds[i])
		{
			return false;
		}
	}

	return true;
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

static void Puzzle10_C(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int32_t answer = 0;

	string line;
	while (getline(input, line))
	{
		IlpProblem p = ParseProblem(line);

		int32_t minimumIntegerSolution = numeric_limits<int32_t>::max();
		for (list<IlpProblem> activeProblems{ p }; !activeProblems.empty(); activeProblems.pop_front())
		{
			IlpSolution s = Solve(activeProblems.front());
			if (!s.Feasible)
				continue;

			if (s.ObjectiveValue > minimumIntegerSolution)
				continue;

			bool isIntegerSolution = true;
			for (size_t variable = 0; variable < s.VariableValues.size(); variable++)
			{
				if (!IsIntegerIsh(s.VariableValues[variable]))
				{
					IlpProblem a = activeProblems.front();
					AddOrUpdateUpperBoundCuttingPlane(&a, variable, s.VariableValues[variable]);
					if (AreCuttingPlanesConsistent(a))
					{
						activeProblems.push_back(a);
					}

					IlpProblem b = activeProblems.front();
					AddOrUpdateLowerBoundCuttingPlane(&b, variable, s.VariableValues[variable]);
					if (AreCuttingPlanesConsistent(b))
					{
						activeProblems.push_back(b);
					}

					isIntegerSolution = false;
					break;
				}
			}

			if (isIntegerSolution)
			{
				minimumIntegerSolution = min(minimumIntegerSolution, static_cast<int32_t>(round(s.ObjectiveValue)));
			}
		}

		assert(minimumIntegerSolution != numeric_limits<int32_t>::max());
		answer += minimumIntegerSolution;
	}

	printf("[2025] Puzzle10_C: %" PRId32 "\n", answer);
}

void Puzzle10_2025(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
	Puzzle10_C(filename);
}


