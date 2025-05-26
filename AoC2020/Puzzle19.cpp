#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2020_Types
{
}

using namespace Puzzle19_2020_Types;


static int MakeAtom(int id)
{
	return id + 1000;
}

static bool IsAtom(int id)
{
	return id >= 1000;
}

static vector<int> Tokenise(const string& input, const map<char, int>& tokeniserRules)
{
	return MakeEnumerator(input)
		->Select<int>([&tokeniserRules](char c) { return tokeniserRules.at(c); })
		->ToVector();
}

static void Puzzle19_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int, set<vector<int>>> productionRules;
	map<char, int> tokeniserRules;
	vector<string> messages;
	{
		regex productionSingleFormat(R"((\d+): (\d+))");
		regex productionDoubleFormat(R"((\d+): (\d+) (\d+))");
		regex productionSingleChoiceFormat(R"((\d+): (\d+) \| (\d+))");
		regex productionDoubleChoiceFormat(R"((\d+): (\d+) (\d+) \| (\d+) (\d+))");
		regex productionAtomFormat(R"-((\d+): "(\w)")-");
		regex messageFormat(R"([ab]+)");

		for (const string& line : ReadAllLines(input))
		{
			smatch match;
			if (regex_match(line, match, productionSingleFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()) });
			}

			if (regex_match(line, match, productionDoubleFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()), stoi(match[3].str()) });
			}

			if (regex_match(line, match, productionSingleChoiceFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()) });
				productionRules[stoi(match[1].str())].insert({ stoi(match[3].str()) });
			}

			if (regex_match(line, match, productionDoubleChoiceFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()), stoi(match[3].str()) });
				productionRules[stoi(match[1].str())].insert({ stoi(match[4].str()), stoi(match[5].str()) });
			}

			if (regex_match(line, match, productionAtomFormat))
			{
				productionRules[stoi(match[1].str())].insert({ MakeAtom(match[2].str()[0]) });
				tokeniserRules[match[2].str()[0]] = MakeAtom(match[2].str()[0]);
			}

			if (regex_match(line, match, messageFormat))
			{
				messages.push_back(match.str());
			}
		}
	}

	// Generate
	map<int, set<vector<int>>> fullGrammar;
	function<const set<vector<int>>&(int id)> generate;
	generate = [&generate, &fullGrammar, &productionRules](int id) -> const set<vector<int>> &
	{
		map<int, set<vector<int>>>::const_iterator cachedIt = fullGrammar.find(id);
		if (cachedIt != fullGrammar.end())
		{
			return cachedIt->second;
		}

		if (IsAtom(id))
		{			
			vector<int> singleElement;
			singleElement.push_back(id);
			fullGrammar[id].insert(move(singleElement));
			return fullGrammar[id];
		}

		const set<vector<int>>& possibleProductions = productionRules.at(id);
		for (const vector<int> &production : possibleProductions)
		{
			if (production.size() == 1)
			{
				const set<vector<int>>& gen = generate(production[0]);
				fullGrammar[id].insert(gen.begin(), gen.end());
				continue;
			}

			assert(production.size() == 2);
			for (const vector<int>& gen1 : generate(production[0]))
			{
				for (const vector<int>& gen2 : generate(production[1]))
				{
					vector<int> combined = gen1;
					combined.insert(combined.end(), gen2.begin(), gen2.end());
					fullGrammar[id].insert(move(combined));
				}
			}
		}

		return fullGrammar[id];
	};

	generate(0);

	int64_t answer = 0;
	for (const string& message : messages)
	{
		vector<int> tokenisedMessage = Tokenise(message, tokeniserRules);
		if (fullGrammar[0].find(tokenisedMessage) != fullGrammar[0].end())
		{
			answer++;
		}
	}

	printf("[2020] Puzzle19_A: %" PRId64 "\n", answer);
}


static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int, set<vector<int>>> productionRules;
	map<char, int> tokeniserRules;
	vector<string> messages;
	{
		regex productionSingleFormat(R"((\d+): (\d+))");
		regex productionDoubleFormat(R"((\d+): (\d+) (\d+))");
		regex productionSingleChoiceFormat(R"((\d+): (\d+) \| (\d+))");
		regex productionDoubleChoiceFormat(R"((\d+): (\d+) (\d+) \| (\d+) (\d+))");
		regex productionAtomFormat(R"-((\d+): "(\w)")-");
		regex messageFormat(R"([ab]+)");

		for (const string& line : ReadAllLines(input))
		{
			smatch match;
			if (regex_match(line, match, productionSingleFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()) });
			}

			if (regex_match(line, match, productionDoubleFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()), stoi(match[3].str()) });
			}

			if (regex_match(line, match, productionSingleChoiceFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()) });
				productionRules[stoi(match[1].str())].insert({ stoi(match[3].str()) });
			}

			if (regex_match(line, match, productionDoubleChoiceFormat))
			{
				productionRules[stoi(match[1].str())].insert({ stoi(match[2].str()), stoi(match[3].str()) });
				productionRules[stoi(match[1].str())].insert({ stoi(match[4].str()), stoi(match[5].str()) });
			}

			if (regex_match(line, match, productionAtomFormat))
			{
				productionRules[stoi(match[1].str())].insert({ MakeAtom(match[2].str()[0]) });
				tokeniserRules[match[2].str()[0]] = MakeAtom(match[2].str()[0]);
			}

			if (regex_match(line, match, messageFormat))
			{
				messages.push_back(match.str());
			}
		}
	}

	// Clear out unwanted production rules
	productionRules.erase(8);
	productionRules.erase(11);

	// Non-recursive rules
	//productionRules[8].insert(vector<int>{ 42 });
	//productionRules[11].insert(vector<int>{ 42, 31 });

	// Generate
	set<vector<int>> emptyGrammar;

	map<int, set<vector<int>>> fullGrammar;
	function<const set<vector<int>>& (int id)> generate;
	generate = [&](int id) -> const set<vector<int>> &
	{
		map<int, set<vector<int>>>::const_iterator cachedIt = fullGrammar.find(id);
		if (cachedIt != fullGrammar.end())
		{
			return cachedIt->second;
		}

		if (IsAtom(id))
		{
			vector<int> singleElement;
			singleElement.push_back(id);
			fullGrammar[id].insert(move(singleElement));
			return fullGrammar[id];
		}

		if (productionRules.find(id) == productionRules.end())
		{
			return emptyGrammar;
		}

		const set<vector<int>>& possibleProductions = productionRules.at(id);
		for (const vector<int>& production : possibleProductions)
		{
			if (production.size() == 1)
			{
				const set<vector<int>>& gen = generate(production[0]);
				fullGrammar[id].insert(gen.begin(), gen.end());
				continue;
			}

			assert(production.size() == 2);
			for (const vector<int>& gen1 : generate(production[0]))
			{
				for (const vector<int>& gen2 : generate(production[1]))
				{
					vector<int> combined = gen1;
					combined.insert(combined.end(), gen2.begin(), gen2.end());
					fullGrammar[id].insert(move(combined));
				}
			}
		}

		return fullGrammar[id];
	};

	//generate(0);
	generate(42);
	generate(31);

	// Do 42 & 31 overlap?
	vector<vector<int>> common;
	set_intersection(fullGrammar[31].begin(), fullGrammar[31].end(),
		fullGrammar[42].begin(), fullGrammar[42].end(),
		back_inserter(common));
	assert(common.empty());

	// Make sure all fragments are the same size for easy parsing
	for (const vector<int> &fragment : fullGrammar[31])
	{
		assert(fragment.size() == fullGrammar[31].begin()->size());
		(void)fragment;
	}
	for (const vector<int>& fragment : fullGrammar[42])
	{
		assert(fragment.size() == fullGrammar[31].begin()->size());
		(void)fragment;
	}
	assert(fullGrammar[31].begin()->size() == fullGrammar[42].begin()->size());


	size_t fragmentSize = fullGrammar[31].begin()->size();

	int64_t answer = 0;
	for (const string& message : messages)
	{
		if (message.size() % fragmentSize != 0)
		{
			continue;
		}

		vector<int> tokenisedMessage = Tokenise(message, tokeniserRules);

		// Grammar is 42+ 42^n 31^n
		// A lots of 42 followed by B lots of 31, where A > B
		int num42s = 0;
		while (true)
		{
			if (tokenisedMessage.empty())
			{
				break;
			}

			vector<int> f;
			f.insert(f.end(), tokenisedMessage.begin(), tokenisedMessage.begin() + fragmentSize);
			if (fullGrammar[42].find(f) == fullGrammar[42].end())
			{
				break;
			}

			tokenisedMessage.erase(tokenisedMessage.begin(), tokenisedMessage.begin() + fragmentSize);
			num42s++;
		}

		bool valid = true;

		int num31s = 0;
		while (true)
		{
			if (tokenisedMessage.empty())
			{
				break;
			}

			vector<int> f;
			f.insert(f.end(), tokenisedMessage.begin(), tokenisedMessage.begin() + fragmentSize);
			if (fullGrammar[31].find(f) == fullGrammar[31].end())
			{
				valid = false;
				break;
			}

			tokenisedMessage.erase(tokenisedMessage.begin(), tokenisedMessage.begin() + fragmentSize);
			num31s++;
		}

		if (valid && (num42s > 0) && (num31s > 0) && (num42s > num31s))
		{
			answer++;
		}
	}

	printf("[2020] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2020(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
