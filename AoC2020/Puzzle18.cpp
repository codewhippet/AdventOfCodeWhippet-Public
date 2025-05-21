#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2020_Types
{
}

using namespace Puzzle18_2020_Types;

static string CovertToReversePolishNotation(const string& formula)
{
	string output;
	output.reserve(formula.size());

	stack<char> operatorStack;
	for (char c : formula)
	{
		if (isdigit(c))
		{
			output.append(1, c);
		}
		else if ((c == '+') || (c == '*'))
		{
			while ((operatorStack.empty() == false) && (operatorStack.top() != '('))
			{
				output.append(1, operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(c);
		}
		else if (c == '(')
		{
			operatorStack.push(c);
		}
		else if (c == ')')
		{
			assert(operatorStack.empty() == false);
			while (operatorStack.top() != '(')
			{
				output.append(1, operatorStack.top());
				operatorStack.pop();
			}
			assert(operatorStack.top() == '(');
			operatorStack.pop();
		}
	}

	while (operatorStack.empty() == false)
	{
		output.append(1, operatorStack.top());
		operatorStack.pop();
	}

	return output;
}

static string CovertToReversePolishNotationSillyPrecedence(const string& formula)
{
	string output;
	output.reserve(formula.size());

	stack<char> operatorStack;
	for (char c : formula)
	{
		if (isdigit(c))
		{
			output.append(1, c);
		}
		else if (c == '+')
		{
			while ((operatorStack.empty() == false) && (operatorStack.top() != '(') && (operatorStack.top() != '*'))
			{
				output.append(1, operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(c);
		}
		else if (c == '*')
		{
			while ((operatorStack.empty() == false) && (operatorStack.top() != '('))
			{
				output.append(1, operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(c);
		}
		else if (c == '(')
		{
			operatorStack.push(c);
		}
		else if (c == ')')
		{
			assert(operatorStack.empty() == false);
			while (operatorStack.top() != '(')
			{
				output.append(1, operatorStack.top());
				operatorStack.pop();
			}
			assert(operatorStack.top() == '(');
			operatorStack.pop();
		}
	}

	while (operatorStack.empty() == false)
	{
		output.append(1, operatorStack.top());
		operatorStack.pop();
	}

	return output;
}

static int64_t EvaluateReversePolishNotationSum(const string& sum)
{
	stack<int64_t> evaluationStack;

	for (char c : sum)
	{
		if (isdigit(c))
		{
			evaluationStack.push(c - '0');
		}
		else if (c == '+')
		{
			int64_t a = evaluationStack.top();
			evaluationStack.pop();

			int64_t b = evaluationStack.top();
			evaluationStack.pop();

			evaluationStack.push(a + b);
		}
		else if (c == '*')
		{
			int64_t a = evaluationStack.top();
			evaluationStack.pop();

			int64_t b = evaluationStack.top();
			evaluationStack.pop();

			evaluationStack.push(a * b);
		}
	}

	assert(evaluationStack.size() == 1);
	return evaluationStack.top();
}

static int64_t EvaluateSum(const string &sum)
{
	string rpnSum = CovertToReversePolishNotation(sum);
	return EvaluateReversePolishNotationSum(rpnSum);
}

static void Puzzle18_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> values = MakeEnumerator(ReadAllLines(input))
		->Select<int64_t>([](const string& line) { return EvaluateSum(line); })
		->ToVector();

	int64_t answer = MakeEnumerator(values)->Sum();

	printf("[2020] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> rpn = MakeEnumerator(ReadAllLines(input))
		->Select<string>([](const string& line) { return CovertToReversePolishNotationSillyPrecedence(line); })
		->ToVector();

	vector<int64_t> values = MakeEnumerator(rpn)
		->Select<int64_t>([](const string& line) { return EvaluateReversePolishNotationSum(line); })
		->ToVector();

	int64_t answer = MakeEnumerator(values)->Sum();

	printf("[2020] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2020(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
