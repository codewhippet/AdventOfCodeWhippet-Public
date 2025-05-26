#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2024_Types
{
}

using namespace Puzzle05_2024_Types;

static void ReadInput(istream& input, multimap<int, int>* constraints, vector<vector<int>>* lists)
{
	for (const string& line : ReadEachLine(input))
	{
		int first, second;
		if (sscanf(line.c_str(), "%d|%d", &first, &second) == 2)
		{
			constraints->insert(make_pair(second, first));
		}
		else
		{
			vector<int> list;
			regex digits{ R"(\d+)" };
			ranges::copy(ranges::subrange(sregex_iterator{ line.begin(), line.end(), digits }, sregex_iterator{})
				| views::transform([](const smatch& m)
					{
						return atoi(m.str().c_str());
					}),
				back_inserter(list));
			if (list.size() > 0)
			{
				lists->push_back(list);
			}
		}
	}
}

static multimap<int, int> GetActiveConstraints(const vector<int>&, const set<int>& contains, const multimap<int, int>& constraints)
{
	multimap<int, int> activeConstraints;
	ranges::copy(constraints
		| views::filter([&contains](multimap<int, int>::const_reference pair)
			{
				return contains.contains(pair.first) && contains.contains(pair.second);

			}),
		inserter(activeConstraints, activeConstraints.end()));

	return activeConstraints;
}

static int FirstPageWhichIsOutOfPlace(const vector<int>& list, const set<int>&, const multimap<int, int>&, const multimap<int, int>& activeConstraints)
{
	set<int> seen;
	for (int page : list)
	{
		auto requiredPages = activeConstraints.equal_range(page);
		for (multimap<int, int>::const_iterator it = requiredPages.first, end = requiredPages.second;
			it != end;
			++it)
		{
			if (seen.contains(it->second) == false)
			{
				return page;
			}
		}

		seen.insert(page);
	}
	return -1;
}

static int FirstPageWhichIsOutOfPlace(const vector<int>& list, const multimap<int, int>& constraints)
{
	set<int> contains;
	ranges::copy(list, inserter(contains, contains.end()));

	multimap<int, int> activeConstraints = GetActiveConstraints(list, contains, constraints);
	return FirstPageWhichIsOutOfPlace(list, contains, constraints, activeConstraints);
}

static vector<int> FixOrdering(vector<int> list, const multimap<int, int>& constraints)
{
	set<int> contains;
	ranges::copy(list, inserter(contains, contains.end()));

	multimap<int, int> activeConstraints = GetActiveConstraints(list, contains, constraints);

	while (true)
	{
		int pageToMove = FirstPageWhichIsOutOfPlace(list, contains, constraints, activeConstraints);
		if (pageToMove == -1)
		{
			return list;
		}

		// Get the set of pages which need to appear before the out of order page
		auto requiredPagesIt = activeConstraints.equal_range(pageToMove);
		assert(requiredPagesIt.first != requiredPagesIt.second);

		set<int> requiredPages;
		ranges::copy(ranges::subrange(requiredPagesIt.first, requiredPagesIt.second)
			| views::transform([](multimap<int, int>::const_reference kvp) -> int
				{
					return kvp.second;
				}),
			inserter(requiredPages, requiredPages.end()));

		// Remove the out of order page
		list.erase(ranges::find(list, pageToMove));

		// Look for the last page from the required set
		auto lastRequiredPageIt = ranges::find_if(list.rbegin(), list.rend(),
			[&requiredPages](vector<int>::const_reference page)
			{
				return requiredPages.contains(page);
			});
		int lastRequiredPage = *lastRequiredPageIt;

		// Put the page into the right place
		list.insert(ranges::find(list, lastRequiredPage) + 1, pageToMove);
	}

	return list;
}

static void Puzzle05_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	multimap<int, int> constraints;
	vector<vector<int>> lists;
	ReadInput(input, &constraints, &lists);

	auto middleDigits = lists
		| views::transform([&constraints](const vector<int>& list)
			{
				return FirstPageWhichIsOutOfPlace(list, constraints) == -1 ? list[list.size() / 2] : 0;
			});
	int64_t answer = accumulate(middleDigits.begin(), middleDigits.end(), 0);

	printf("[2024] Puzzle05_A: %" PRId64 "\n", answer);
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	multimap<int, int> constraints;
	vector<vector<int>> lists;
	ReadInput(input, &constraints, &lists);

	vector<vector<int>> unorderedLists;
	ranges::copy(lists
		| views::filter([&constraints](const vector<int>& list)
			{
				return FirstPageWhichIsOutOfPlace(list, constraints) != -1;
			}),
		back_inserter(unorderedLists));

	vector<vector<int>> fixedLists;
	ranges::copy(unorderedLists
		| views::transform([&constraints](vector<vector<int>>::const_reference brokenList)
			{
				return FixOrdering(brokenList, constraints);
			}),
		back_inserter(fixedLists));

	auto middleDigits = fixedLists
		| views::transform([](vector<vector<int>>::const_reference fixedList)
			{
				return fixedList[fixedList.size() / 2];
			});
	int64_t answer = accumulate(middleDigits.begin(), middleDigits.end(), 0);

	printf("[2024] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2024(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
