#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle04_2020_Types
{
}

using namespace Puzzle04_2020_Types;


static void Puzzle04_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	lines.push_back("");

	int64_t answer = 0;

	set<string> requiredFields = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid" };

	regex fieldFormat(R"((\w{3}):)");

	set<string> seenFields = { "cid" };
	for (string line : lines)
	{
		if (line.empty())
		{
			if (seenFields == requiredFields)
			{
				answer++;
			}
			seenFields.clear();
			seenFields.insert("cid");
		}

		smatch match;
		while (regex_search(line, match, fieldFormat))
		{
			seenFields.insert(match[1].str());
			line = match.suffix();
		}
	}

	printf("[2020] Puzzle04_A: %" PRId64 "\n", answer);
}

static void Puzzle04_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<string> lines = ReadAllLines(input);
	lines.push_back("");

	int64_t answer = 0;

	map<string, bool> requiredFields =
	{
		{ "byr", true },
		{ "iyr", true },
		{ "eyr", true },
		{ "hgt", true },
		{ "hcl", true },
		{ "ecl", true },
		{ "pid", true },
		{ "cid", true },
	};

	map<string, function<bool(const string&)>> validation;

	validation["byr"] = [](const string& s) { int year = stoi(s); return (year >= 1920) && (year <= 2002); };
	validation["iyr"] = [](const string& s) { int year = stoi(s); return (year >= 2010) && (year <= 2020); };
	validation["eyr"] = [](const string& s) { int year = stoi(s); return (year >= 2020) && (year <= 2030); };
	validation["hgt"] = [](const string& s)
		{
			int height = stoi(s);
			if (s.find("cm") != string::npos)
			{
				return (height >= 150) && (height <= 193);
			}
			if (s.find("in") != string::npos)
			{
				return (height >= 59) && (height <= 76);
			}
			return false;
		};
	validation["hcl"] = [](const string& s)
		{
			if (s.length() != 7)
				return false;

			if (s[0] != '#')
				return false;

			return count_if(s.begin() + 1, s.end(), [](char c) { return isxdigit(c); }) == 6;
		};
	validation["ecl"] = [](const string& s)
		{
			return
				(s == "amb") ||
				(s == "blu") ||
				(s == "brn") ||
				(s == "gry") ||
				(s == "grn") ||
				(s == "hzl") ||
				(s == "oth");
		};
	validation["pid"] = [](const string& s)
		{
			return count_if(s.begin(), s.end(), [](char c) { return isdigit(c); }) == 9;
		};
	validation["cid"] = [](const string&) { return true; };

	regex fieldFormat(R"((\w{3}):([^\s]+))");

	int64_t invalid = 0;

	map<string, bool> seenFields = { { "cid", true } };
	for (string line : lines)
	{
		if (line.empty())
		{
			if (seenFields == requiredFields)
			{
				answer++;
			}
			else
			{
				invalid++;
			}
			seenFields.clear();
			seenFields["cid"] = true;
		}

		smatch match;
		while (regex_search(line, match, fieldFormat))
		{
			string field = match[1].str();
			seenFields[field] = validation[field](match[2].str());
			line = match.suffix();
		}
	}

	printf("[2020] Puzzle04_B: %" PRId64 "\n", answer);
}

void Puzzle04_2020(const string& filename)
{
	Puzzle04_A(filename);
	Puzzle04_B(filename);
}
