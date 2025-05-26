#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle03_2022_Types
{
}

using namespace Puzzle03_2022_Types;

static void Puzzle03_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<char, int64_t> priority;
	ranges::copy(ranges::iota_view(0, 26) | views::transform([](int i) -> pair<char, int64_t> { return { (char)('a' + i), i + 1 }; }), inserter(priority, priority.end()));
	ranges::copy(ranges::iota_view(0, 26) | views::transform([](int i) -> pair<char, int64_t> { return { (char)('A' + i), i + 27 }; }), inserter(priority, priority.end()));

	int64_t answer = 0;
	for (const auto &line : ReadEachLine(input))
	{
		size_t halfLen = line.size() / 2;
		string firstCompartment = line.substr(0, halfLen);
		string secondCompartment = line.substr(halfLen);

		vector<char> commonChars;
		ranges::sort(firstCompartment);
		ranges::sort(secondCompartment);
		ranges::set_intersection(firstCompartment, secondCompartment, back_inserter(commonChars));
		assert(commonChars.size() > 0);

		answer += priority[commonChars.front()];
	}

	printf("[2022] Puzzle03_A: %" PRId64 "\n", answer);
}

static void Puzzle03_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);

	map<char, int64_t> priority;
	ranges::copy(ranges::iota_view(0, 26) | views::transform([](int i) -> pair<char, int64_t> { return { (char)('a' + i), i + 1 }; }), inserter(priority, priority.end()));
	ranges::copy(ranges::iota_view(0, 26) | views::transform([](int i) -> pair<char, int64_t> { return { (char)('A' + i), i + 27 }; }), inserter(priority, priority.end()));

	int64_t answer = 0;
	for (size_t i = 0; (i + 2) < lines.size(); i += 3)
	{
		vector<char> commonChars[2];

		ranges::sort(lines[i + 0]);
		ranges::sort(lines[i + 1]);
		ranges::sort(lines[i + 2]);

		ranges::set_intersection(lines[i + 0], lines[i + 1], back_inserter(commonChars[0]));
		ranges::set_intersection(commonChars[0], lines[i + 2], back_inserter(commonChars[1]));	
		assert(commonChars[1].size() > 0);

		answer += priority[commonChars[1].front()];
	}

	printf("[2022] Puzzle03_B: %" PRId64 "\n", answer);
}

void Puzzle03_2022(const string& filename)
{
	Puzzle03_A(filename);
	Puzzle03_B(filename);
}
