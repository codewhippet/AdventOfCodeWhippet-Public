#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2023_Types
{
	struct Part
	{
		int64_t Xmas[4];
	};

	struct PartRange
	{
		pair<int64_t, int64_t> Xmas[4];
	};

	struct Rule
	{
		char Type;
		int XmasIndex;
		int64_t Value;
		string Goto;
	};

	struct Workflow
	{
		vector<Rule> Rules;
	};

	struct Puzzle
	{
		map<string, Workflow> Workflows;
		vector<Part> Parts;
	};

	static int IndexLookup[26] = { 2, 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 1, 'n', 'o', 'p', 'q', 'r', 3, 't', 'u', 'v', 'w', 0, 'y', 'z' };
}

using namespace Puzzle19_2023_Types;

static Rule ReadRule(const string& ruleText)
{
	Rule r;
	char index;
	char gotoId[64];
	int scanned = sscanf(ruleText.c_str(), R"(%c%c%lld:%63s)", &index, &r.Type, &r.Value, &gotoId[0]);
	assert(scanned == 4);
	(void)scanned;
	r.XmasIndex = IndexLookup[index - 'a'];
	r.Goto = &gotoId[0];
	return r;
}

static Rule MakeGotoRule(const string& ruleText)
{
	return Rule{ 'J', 0, 0, ruleText };
}

static Rule MakeAcceptRule()
{
	return Rule{ 'A', 0, 0, "" };
}

static Rule MakeRejectRule()
{
	return Rule{ 'R', 0, 0, "" };
}

static void ReadWorkflow(const string& line, map<string, Workflow>* workflows)
{
	char id[64] = { 0 };
	char rules[1024] = { 0 };
	int scanned = sscanf(line.c_str(), R"(%63[^{]{%1023[^}]})", &id[0], &rules[0]);
	assert(scanned == 2);
	(void)scanned;

	Workflow workflow;

	vector<string> ruleText = StringSplit(string{ rules }, ',');
	for (size_t ruleIndex = 0; ruleIndex < (ruleText.size() - 1); ruleIndex++)
	{
		workflow.Rules.push_back(ReadRule(ruleText[ruleIndex]));
	}
	workflow.Rules.push_back(MakeGotoRule(ruleText.back()));

	workflows->insert({ string{id}, workflow });
}

static void ReadPart(const string& line, vector<Part>* parts)
{
	Part p;

	int scanned = sscanf(line.c_str(), R"({x=%lld,m=%lld,a=%lld,s=%lld})", &p.Xmas[0], &p.Xmas[1], &p.Xmas[2], &p.Xmas[3]);
	assert(scanned == 4);
	(void)scanned;

	parts->push_back(p);
}

static Puzzle ReadPuzzle(istream& input)
{
	Puzzle ret;

	vector<string> lines = ReadAllLines(input);

	size_t line = 0;
	for (/***/; lines[line].empty() == false; line++)
	{
		ReadWorkflow(lines[line], &ret.Workflows);
	}

	for (line++; line < lines.size(); line++)
	{
		ReadPart(lines[line], &ret.Parts);
	}

	ret.Workflows["A"] = Workflow{ { MakeAcceptRule() } };
	ret.Workflows["R"] = Workflow{ { MakeRejectRule() } };

	return ret;
}

static bool RuleApplied(const Part& part, const Rule& rule, const map<string, Workflow>& workflows, vector<Part>* accepted);

static bool EvaluatePart(const Part& part, const string& workflowId, const map<string, Workflow>& workflows, vector<Part>* accepted)
{
	const Workflow& workflow = workflows.at(workflowId);
	for (const Rule& rule : workflow.Rules)
	{
		if (RuleApplied(part, rule, workflows, accepted))
		{
			return true;
		}
	}

	assert(false);
	return false;
}

static bool RuleApplied(const Part& part, const Rule& rule, const map<string, Workflow>& workflows, vector<Part>* accepted)
{
	switch (rule.Type)
	{
	case 'J':
		return EvaluatePart(part, rule.Goto, workflows, accepted);

	case 'A':
		accepted->push_back(part);
		return true;

	case 'R':
		return true;

	case '>':
		if (part.Xmas[rule.XmasIndex] > rule.Value)
		{
			return EvaluatePart(part, rule.Goto, workflows, accepted);
		}
		return false;

	case '<':
		if (part.Xmas[rule.XmasIndex] < rule.Value)
		{
			return EvaluatePart(part, rule.Goto, workflows, accepted);
		}
		return false;
	}

	assert(false);
	return false;
}

static map<string, Workflow> Optimise(const map<string, Workflow>& workflows)
{
	map<string, Workflow> newWorkflows;

	set<string> alwaysAccepts;
	set<string> alwaysRejects;
	for (map<string, Workflow>::const_reference workflow : workflows)
	{
		size_t numAccepts = 0;
		size_t numRejects = 0;
		for (const Rule& rule : workflow.second.Rules)
		{
			if (rule.Goto == "A")
				numAccepts++;
			if (rule.Goto == "R")
				numRejects++;
		}
		if (numAccepts == workflow.second.Rules.size())
			alwaysAccepts.insert(workflow.first);
		if (numRejects == workflow.second.Rules.size())
			alwaysRejects.insert(workflow.first);
	}

	for (map<string, Workflow>::const_reference workflow : workflows)
	{
		if (alwaysAccepts.contains(workflow.first))
			continue;
		if (alwaysRejects.contains(workflow.first))
			continue;

		Workflow newWorkflow;

		for (const Rule& rule : workflow.second.Rules)
		{
			Rule newRule = rule;
			if (alwaysAccepts.contains(newRule.Goto))
				newRule.Goto = "A";
			if (alwaysRejects.contains(newRule.Goto))
				newRule.Goto = "R";
			newWorkflow.Rules.push_back(newRule);
		}

		newWorkflows.insert({ workflow.first, newWorkflow });
	}

	return newWorkflows;
}

static pair<bool, bool> SplitRange(const PartRange& source, const Rule& rule, PartRange* a, PartRange* b)
{
	switch (rule.Type)
	{
	case 'J':
	case 'A':
	case 'R':
		*a = source;
		return { true, false };

	case '>':
		{
			*a = source;
			*b = source;

			a->Xmas[rule.XmasIndex].first = max(a->Xmas[rule.XmasIndex].first, rule.Value + 1);
			b->Xmas[rule.XmasIndex].second = min(b->Xmas[rule.XmasIndex].second, rule.Value);

			return {
				(a->Xmas[rule.XmasIndex].first < a->Xmas[rule.XmasIndex].second),
				(b->Xmas[rule.XmasIndex].first < b->Xmas[rule.XmasIndex].second)
			};
		}
		break;

	case '<':
		{
			*a = source;
			*b = source;

			a->Xmas[rule.XmasIndex].second = min(a->Xmas[rule.XmasIndex].second, rule.Value - 1);
			b->Xmas[rule.XmasIndex].first = max(b->Xmas[rule.XmasIndex].first, rule.Value);

			return {
				(a->Xmas[rule.XmasIndex].first < a->Xmas[rule.XmasIndex].second),
				(b->Xmas[rule.XmasIndex].first < b->Xmas[rule.XmasIndex].second)
			};
		}
		break;
	}

	assert(false);
	return { false, false };
}

static void ApplyRule(const PartRange& part, const Rule& rule, const map<string, Workflow>& workflows, vector<PartRange>* accepted);

static void EvaluatePartRange(const PartRange& part, const string& workflowId, const map<string, Workflow>& workflows, vector<PartRange>* accepted)
{
	const Workflow& workflow = workflows.at(workflowId);

	PartRange range = part;
	for (const Rule& rule : workflow.Rules)
	{
		PartRange aRange, bRange;
		pair<bool, bool> split = SplitRange(range, rule, &aRange, &bRange);
		if (split.first)
		{
			ApplyRule(aRange, rule, workflows, accepted);
		}
		if (split.second == false)
		{
			break;
		}
		range = bRange;
	}
}

static void ApplyRule(const PartRange& part, const Rule& rule, const map<string, Workflow>& workflows, vector<PartRange>* accepted)
{
	switch (rule.Type)
	{
	case 'J':
	case '>':
	case '<':
		EvaluatePartRange(part, rule.Goto, workflows, accepted);
		break;

	case 'A':
		accepted->push_back(part);
		break;

	case 'R':
		break;
	}
}

static void Puzzle19_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);

	{
		puzzle.Workflows = Optimise(puzzle.Workflows);
		puzzle.Workflows = Optimise(puzzle.Workflows);
		puzzle.Workflows = Optimise(puzzle.Workflows);
	}

	vector<Part> accepted;
	for (const Part& part : puzzle.Parts)
	{
		EvaluatePart(part, "in", puzzle.Workflows, &accepted);
	}

	int64_t answer = 0;
	for (const Part& part : accepted)
	{
		answer += part.Xmas[0];
		answer += part.Xmas[1];
		answer += part.Xmas[2];
		answer += part.Xmas[3];
	}

	printf("[2023] Puzzle19_A: %" PRId64 "\n", answer);
}

static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle puzzle = ReadPuzzle(input);

	{
		puzzle.Workflows = Optimise(puzzle.Workflows);
		puzzle.Workflows = Optimise(puzzle.Workflows);
		puzzle.Workflows = Optimise(puzzle.Workflows);
	}

	PartRange fullRange;
	fullRange.Xmas[0] = { 1, 4000 };
	fullRange.Xmas[1] = { 1, 4000 };
	fullRange.Xmas[2] = { 1, 4000 };
	fullRange.Xmas[3] = { 1, 4000 };

	vector<PartRange> accepted;
	EvaluatePartRange(fullRange, "in", puzzle.Workflows, &accepted);

	int64_t answer = 0;
	for (const PartRange& range : accepted)
	{
		int64_t rangeCombinations = 1;
		for (int i = 0; i < 4; i++)
		{
			rangeCombinations *= (range.Xmas[i].second - range.Xmas[i].first + 1);
		}
		answer += rangeCombinations;
	}

	printf("[2023] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2023(const string& filename)
{
	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
