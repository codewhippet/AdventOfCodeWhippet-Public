#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2024_Types
{
}

using namespace Puzzle03_2024_Types;

static void Puzzle03_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	int64_t answer = 0;

	for (const string& line : ReadAllLines(input))
	{
		answer += Enumerable::Regex(line, regex(R"(mul\((\d{1,3}),(\d{1,3})\))"))
			->Select<pair<int64_t, int64_t>>([](const smatch& m)
				{
					return make_pair(atoll(m[1].str().c_str()), atoll(m[2].str().c_str()));
				})
			->Select<int64_t>([](const pair<int64_t, int64_t>& p)
				{
					return p.first * p.second;
				})
			->Sum();
	}

	printf("[2024] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	auto fileContents = ReadEachLine(input);
	string wholeFile = accumulate(fileContents.begin(), fileContents.end(), string{});

	int64_t answer = 0;
	int64_t multiplier = 1;

	auto multiplyOperations = Enumerable::Regex(wholeFile, regex(R"(mul\((\d{1,3}),(\d{1,3})\))"))
		->Select<pair<ptrdiff_t, function<void()>>>([&answer, &multiplier](const smatch& m)
			{
				int64_t a = atoll(m[1].str().c_str());
				int64_t b = atoll(m[2].str().c_str());
				return make_pair<ptrdiff_t, function<void()>>(m.position(),
					[&answer, &multiplier, a, b]()
					{
						answer += a * b * multiplier;
					});
			})
		->ToVector();

	auto enableOperations = Enumerable::Regex(wholeFile, regex(R"(do\(\))"))
		->Select<pair<ptrdiff_t, function<void()>>>([&multiplier](const smatch& m)
			{
				return make_pair<ptrdiff_t, function<void()>>(m.position(),
					[&multiplier]()
					{
						multiplier = 1;
					});
			})
		->ToVector();

	auto disableOperations = Enumerable::Regex(wholeFile, regex(R"(don't\(\))"))
		->Select<pair<ptrdiff_t, function<void()>>>([&multiplier](const smatch& m)
			{
				return make_pair<ptrdiff_t, function<void()>>(m.position(),
					[&multiplier]()
					{
						multiplier = 0;
					});
			})
		->ToVector();

	auto allOperations = multiplyOperations;
	allOperations.insert(allOperations.end(), enableOperations.begin(), enableOperations.end());
	allOperations.insert(allOperations.end(), disableOperations.begin(), disableOperations.end());
	sort(allOperations.begin(), allOperations.end(),
		[](const pair<ptrdiff_t, function<void()>>& a, const pair<ptrdiff_t, function<void()>>& b)
		{
			return a.first < b.first;
		});

	for (const auto& op : allOperations)
	{
		op.second();
	}

	printf("[2024] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2024(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
