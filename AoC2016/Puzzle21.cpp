#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2016_Types
{
	using Operation = function<string(const string&)>;
}

using namespace Puzzle21_2016_Types;

static vector<Operation> ParseProgram(istream& input)
{
	vector<Operation> program;

	const regex swapPosition{ R"(swap position (\d+) with position (\d+))" };
	const regex swapLetter{ R"(swap letter (\w) with letter (\w))" };
	const regex rotate{ R"(rotate (\w+) (\d+) steps?)" };
	const regex rotateByLetter{ R"(rotate based on position of letter (\w))" };
	const regex reverse{ R"(reverse positions (\d+) through (\d+))" };
	const regex move{ R"(move position (\d+) to position (\d+))" };

	for (const string& line : ReadEachLine(input))
	{
		smatch m;
		if (regex_match(line, m, swapPosition))
		{
			program.emplace_back(
				[a = atoll(m[1].str().c_str()), b = atoll(m[2].str().c_str())](const string& s)
				{
					string t = s;
					swap(t[a], t[b]);
					return t;
				});
		}
		else if (regex_match(line, m, swapLetter))
		{
			program.emplace_back(
				[a = m[1].str()[0], b = m[2].str()[0]](const string& s)
				{
					string t = s;
					swap(t[t.find(a)], t[t.find(b)]);
					return t;
				});
		}
		else if (regex_match(line, m, rotate))
		{
			bool right = (m[1].str() == "right"sv);
			program.emplace_back(
				[right, a = atoll(m[2].str().c_str())](const string& s)
				{
					assert(a <= (int64_t)s.size());

					string t = s;
					for (size_t sourceIndex = 0; sourceIndex < s.size(); sourceIndex++)
					{
						size_t destinationIndex = (right ? (sourceIndex + a) % t.size() : (sourceIndex + t.size() - a) % t.size());
						t[destinationIndex] = s[sourceIndex];
					}

					return t;
				});
		}
		else if (regex_match(line, m, rotateByLetter))
		{
			program.emplace_back(
				[a = m[1].str()[0]](const string& s)
				{
					size_t position = s.find(a);
					size_t rotation = 1 + position + (position >= 4 ? 1 : 0);

					string t = s;
					for (size_t sourceIndex = 0; sourceIndex < s.size(); sourceIndex++)
					{
						size_t destinationIndex = (sourceIndex + rotation) % t.size();
						t[destinationIndex] = s[sourceIndex];
					}

					return t;
				});
		}
		else if (regex_match(line, m, reverse))
		{
			program.emplace_back(
				[a = atoll(m[1].str().c_str()), b = atoll(m[2].str().c_str())](const string& s)
				{
					assert(a < b);

					auto i = a;
					auto j = b;

					string t = s;
					while (i < j)
					{
						swap(t[i++], t[j--]);
					}

					return t;
				});
		}
		else if (regex_match(line, m, move))
		{
			program.emplace_back(
				[a = atoll(m[1].str().c_str()), b = atoll(m[2].str().c_str())](const string& s)
				{
					string t = s;
					char letter = t[a];
					t.erase(a, 1);
					t.insert(t.begin() + b, letter);
					return t;
				});
		}
		else
		{
			assert(false);
		}
	}

	return program;
}

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Operation> program = ParseProgram(input);

	string s = "abcdefgh";
	for (const Operation& op : program)
	{
		s = op(s);
	}

	string answer = s;

	printf("[2016] Puzzle21_A: %s\n", answer.c_str());
}


static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Operation> program = ParseProgram(input);

	string perm = "abcdefgh";
	do
	{
		string scrambled = perm;
		for (const Operation& op : program)
		{
			scrambled = op(scrambled);
		}

		if (scrambled == "fbgdceah"sv)
		{
			break;
		}

	} while (next_permutation(perm.begin(), perm.end()));

	string answer = perm;

	printf("[2016] Puzzle21_B: %s\n", answer.c_str());
}

void Puzzle21_2016(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}
