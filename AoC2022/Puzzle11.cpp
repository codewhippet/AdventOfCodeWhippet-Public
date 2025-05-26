#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2022_Types
{
	struct Monkey
	{
		deque<int64_t> Items;
		function<int64_t(int64_t)> Operation;
		int64_t Div = 0;
		int64_t ThrowOnTrue = -1;
		int64_t ThrowOnFalse = -1;

		int64_t InspectionCount = 0;
	};

	template <typename CLASS, auto MEMBER>
	struct GetMember
	{
		auto operator()(const shared_ptr<CLASS> &ptr) const
		{
			return (ptr.get())->*MEMBER;
		}
	};
}

using namespace Puzzle11_2022_Types;

static vector<shared_ptr<Monkey>> ReadMonkeys(istream &input)
{
	vector<shared_ptr<Monkey>> monkeys;

	shared_ptr<Monkey> currentMonkey;
	for (const string &line : ReadEachLine(input))
	{
		if (line.starts_with("Monkey"sv))
		{
			currentMonkey = make_shared<Monkey>();
			monkeys.push_back(currentMonkey);
		}
		else if (line.starts_with("  Starting items:"sv))
		{
			currentMonkey->Items = ranges::to<deque>(ReadAsVectorOfNumbers(line));
		}
		else if (line.starts_with("  Operation: new = old * old"sv))
		{
			// Special case because we don't have a generalised eval mechanism and there aren't enough cases to write a full parser for arbitrary expressions
			currentMonkey->Operation = [](int64_t v) { return v * v; };
		}
		else if (line.starts_with("  Operation:"sv))
		{
			char operation;
			int64_t operand;
			int scanned = sscanf(line.c_str(), "  Operation: new = old %c %lld", &operation, &operand);
			assert(scanned == 2);
			(void)scanned;

			switch (operation)
			{
			case '+':
				currentMonkey->Operation = [operand](int64_t v) { return v + operand; };
				break;
			case '*':
				currentMonkey->Operation = [operand](int64_t v) { return v * operand; };
				break;
			default:
				assert(false);
			}
		}
		else if (line.starts_with("  Test:"sv))
		{
			currentMonkey->Div = ReadFirstNumber(line);
		}
		else if (line.starts_with("    If true:"sv))
		{
			currentMonkey->ThrowOnTrue = ReadFirstNumber(line);
		}
		else if (line.starts_with("    If false:"sv))
		{
			currentMonkey->ThrowOnFalse = ReadFirstNumber(line);
		}
	}

	return monkeys;
}

static void Puzzle11_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<shared_ptr<Monkey>> monkeys = ReadMonkeys(input);

	for (int64_t i = 0; i < 20; i++)
	{
		for (const shared_ptr<Monkey> &m : monkeys)
		{
			while (!m->Items.empty())
			{
				int64_t itemWorry = m->Items.front();
				m->Items.pop_front();

				itemWorry = m->Operation(itemWorry); // As monkey inspects item

				m->InspectionCount++;

				itemWorry /= 3; // After inspection is complete

				bool test = (itemWorry % m->Div) == 0;
				if (test)
				{
					monkeys[m->ThrowOnTrue]->Items.push_back(itemWorry);
				}
				else
				{
					monkeys[m->ThrowOnFalse]->Items.push_back(itemWorry);
				}
			}
		}
	}

	vector<int64_t> topTwo = monkeys | views::transform(GetMember<Monkey, &Monkey::InspectionCount>{}) | ranges::to<vector>();
	ranges::sort(topTwo);
	ranges::reverse(topTwo);

	int64_t answer = topTwo[0] * topTwo[1];

	printf("[2022] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<shared_ptr<Monkey>> monkeys = ReadMonkeys(input);

	int64_t reduction = ranges::fold_left(monkeys | views::transform(GetMember<Monkey, &Monkey::Div>{}), 1, multiplies{});

	for (int64_t i = 0; i < 10000; i++)
	{
		for (const shared_ptr<Monkey> &m : monkeys)
		{
			while (!m->Items.empty())
			{
				int64_t itemWorry = m->Items.front();
				m->Items.pop_front();

				itemWorry = m->Operation(itemWorry); // As monkey inspects item

				m->InspectionCount++;

				itemWorry = itemWorry % reduction; // After inspection is complete

				bool test = (itemWorry % m->Div) == 0;
				if (test)
				{
					monkeys[m->ThrowOnTrue]->Items.push_back(itemWorry);
				}
				else
				{
					monkeys[m->ThrowOnFalse]->Items.push_back(itemWorry);
				}
			}
		}
	}

	vector<int64_t> topTwo = monkeys | views::transform(GetMember<Monkey, &Monkey::InspectionCount>{}) | ranges::to<vector>();
	ranges::sort(topTwo);
	ranges::reverse(topTwo);

	int64_t answer = topTwo[0] * topTwo[1];

	printf("[2022] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2022(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
