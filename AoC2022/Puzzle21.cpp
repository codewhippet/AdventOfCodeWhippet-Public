#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2022_Types
{
	struct Monkey
	{
		string Name;
		string A;
		string B;
		char Operation;
	};

	struct Polynomial
	{
		map<int, double> Coefficients;
	};

	Polynomial operator+(const Polynomial& a, const Polynomial& b)
	{
		Polynomial result = a;
		for (const auto& bCoeff : b.Coefficients)
		{
			result.Coefficients[bCoeff.first] += bCoeff.second;
		}
		return result;
	}

	Polynomial operator-(const Polynomial& a, const Polynomial& b)
	{
		Polynomial result = a;
		for (const auto& bCoeff : b.Coefficients)
		{
			result.Coefficients[bCoeff.first] -= bCoeff.second;
		}
		return result;
	}

	Polynomial operator*(const Polynomial& a, const Polynomial& b)
	{
		Polynomial result;
		for (const auto& aCoeff : a.Coefficients)
		{
			for (const auto& bCoeff : b.Coefficients)
			{
				result.Coefficients[aCoeff.first + bCoeff.first] += aCoeff.second * bCoeff.second;
			}
		}
		return result;
	}

	Polynomial operator/(const Polynomial& a, const Polynomial& b)
	{
		Polynomial result;
		for (const auto& aCoeff : a.Coefficients)
		{
			for (const auto& bCoeff : b.Coefficients)
			{
				result.Coefficients[aCoeff.first - bCoeff.first] += aCoeff.second / bCoeff.second;
			}
		}
		return result;
	}
}

using namespace Puzzle21_2022_Types;

static pair<map<string, Monkey>, map<string, Polynomial>> ReadPuzzle(istream& input)
{
	map<string, Monkey> monkeys;
	map<string, Polynomial> results;

	const regex monkeyRule{ R"((\w+): (\w+) (.) (\w+))" };
	const regex monkeyAtom{ R"((\w+): (\d+))" };
	for (const auto& line : ReadEachLine(input))
	{
		smatch m;
		if (regex_match(line, m, monkeyRule))
		{
			Monkey monkey;
			monkey.Name = m[1].str();
			monkey.A = m[2].str();
			monkey.Operation = m[3].str()[0];
			monkey.B = m[4].str();
			monkeys.insert({ monkey.Name, monkey });
		}
		else if (regex_match(line, m, monkeyAtom))
		{
			Polynomial p;
			p.Coefficients[0] = (double)Utils::ToNumber(m[2]);
			results.insert({ m[1].str(), p });
		}
	}

	return { monkeys, results };
}

static Polynomial Evaluate(const string& monkeyName, const map<string, Monkey>& monkeys, map<string, Polynomial>* results)
{
	const auto& existingAnswer = results->find(monkeyName);
	if (existingAnswer != results->end())
	{
		return existingAnswer->second;
	}

	const Monkey& thisMonkey = monkeys.at(monkeyName);
	Polynomial aValue = Evaluate(thisMonkey.A, monkeys, results);
	Polynomial bValue = Evaluate(thisMonkey.B, monkeys, results);

	Polynomial result;
	switch (thisMonkey.Operation)
	{
	case '+':
		result = aValue + bValue;
		break;
	case '-':
		result = aValue - bValue;
		break;
	case '*':
		result = aValue * bValue;
		break;
	case '/':
		result = aValue / bValue;
		break;
	}

	results->insert({ monkeyName, result });

	return result;
}

static void Puzzle21_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [monkeys, results] = ReadPuzzle(input);

	Polynomial rootValue = Evaluate("root", monkeys, &results);
	assert(rootValue.Coefficients.size() == 1);
	assert(rootValue.Coefficients.contains(0));

	int64_t answer = (int64_t)rootValue.Coefficients.at(0);

	printf("[2022] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto [monkeys, results] = ReadPuzzle(input);
	results["humn"] = Polynomial{ .Coefficients{ { 1, 1.0 } } };

	Polynomial rootValue = Evaluate("root", monkeys, &results);

	Polynomial aValue = results.at(monkeys["root"].A);
	Polynomial bValue = results.at(monkeys["root"].B);

	int64_t answer = (int64_t)((aValue.Coefficients[0] - bValue.Coefficients[0]) / (bValue.Coefficients[1] - aValue.Coefficients[1]));

	printf("[2022] Puzzle21_B: %" PRId64 "\n", answer);
}

void Puzzle21_2022(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
