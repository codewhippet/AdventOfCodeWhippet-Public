#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle05_2023_Types
{
	struct Range
	{
		int64_t Begin;
		int64_t End;
	};

	bool operator<(const Range& a, const Range& b)
	{
		// We sort by End first so that we can use map::upper_bound to
		// search for an overlapping range

		if (a.End != b.End)
			return a.End < b.End;

		return a.Begin < b.Begin;
	}

	struct TransformationStage
	{
		string Name;
		map<Range, int64_t> RangeShift;
	};

	struct Puzzle
	{
		vector<int64_t> StartingSeeds;
		vector<TransformationStage> Stages;
	};
}

using namespace Puzzle05_2023_Types;

static Puzzle ParsePuzzle(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	Puzzle p;

	p.StartingSeeds = Enumerable::Regex(lines[0].substr(sizeof("seeds:") - 1), regex{ R"(\d+)" })
		->Select<int64_t>([](const smatch& m) { return stoll(m.str()); })
		->ToVector();

	const regex stageFormat(R"(([\w\-]+) map:)");
	const regex rangeFormat(R"((\d+) (\d+) (\d+))");

	for (size_t i = 1; i < lines.size(); i++)
	{
		const string& line = lines[i];
		if (line.empty())
		{
			continue;
		}

		smatch m;
		if (regex_match(line, m, stageFormat))
		{
			TransformationStage stage;
			stage.Name = m[1].str();
			p.Stages.emplace_back(move(stage));
		}
		else if (regex_match(line, m, rangeFormat))
		{
			int64_t destBegin = stoll(m[1].str());
			int64_t sourceBegin = stoll(m[2].str());
			int64_t rangeSize = stoll(m[3].str());

			Range source;
			source.Begin = sourceBegin;
			source.End = sourceBegin + rangeSize;

			int64_t offset = destBegin - sourceBegin;

			p.Stages.back().RangeShift[source] = offset;
		}
	}

	return p;
}

static int64_t TransformByStage(int64_t source, const TransformationStage& stage)
{
	// Is this affected by a range?
	map<Range, int64_t>::const_iterator candidateRange = stage.RangeShift.upper_bound(Range{ source, source });
	if (candidateRange != stage.RangeShift.end())
	{
		Range r = candidateRange->first;
		if ((source >= r.Begin) && (source < r.End))
		{
			return source = source + candidateRange->second;
		}
	}

	return source;
}

static list<Range> TransformByStage(const Range& source, const TransformationStage& stage)
{
	list<Range> out;

	Range activeRange = source;
	for (map<Range, int64_t>::const_reference rangePair : stage.RangeShift)
	{
		const Range& splittingRange = rangePair.first;
		int64_t rangeOffset = rangePair.second;

		// Range after current splitter, onto the next
		if (activeRange.Begin >= splittingRange.End)
		{
			continue;
		}

		// Range ends before splitter: finished
		if (activeRange.End <= splittingRange.Begin)
		{
			break;
		}

		// Emit the part of the range before the splitter
		if (activeRange.Begin < splittingRange.Begin)
		{
			out.push_back(Range{ activeRange.Begin, splittingRange.Begin });
			activeRange.Begin = splittingRange.Begin;
		}

		// Emit the mapped part of the range
		Range overlappingSection{ max(activeRange.Begin, splittingRange.Begin), min(activeRange.End, splittingRange.End) };
		out.push_back(Range{ overlappingSection.Begin + rangeOffset, overlappingSection.End + rangeOffset });

		activeRange.Begin = splittingRange.End;
		activeRange.End = max(activeRange.End, splittingRange.End);

		// Process what's left over
		if (activeRange.End > splittingRange.End)
		{
			activeRange = Range{ splittingRange.End, activeRange.End };
		}
	}

	// Do we have any range left over?
	if (activeRange.Begin < activeRange.End)
	{
		out.push_back(activeRange);
	}

	return out;
}

static list<Range> TransformByStage(const list<Range>& source, const TransformationStage& stage)
{
	list<Range> out;

	for (const Range& rangeToSplit : source)
	{
		list<Range> newRanges = TransformByStage(rangeToSplit, stage);
		out.insert(out.end(), newRanges.begin(), newRanges.end());
	}

	return out;
}

static void Puzzle05_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	int64_t answer = numeric_limits<int64_t>::max();
	for (int64_t seed : p.StartingSeeds)
	{
		for (size_t i = 0; i < p.Stages.size(); i++)
		{
			seed = TransformByStage(seed, p.Stages[i]);
		}
		answer = min(seed, answer);
	}

	printf("[2023] Puzzle05_A: %" PRId64 "\n", answer);
}

static void Puzzle05_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	list<Range> seedRanges;
	for (size_t i = 0; (i + 1) < p.StartingSeeds.size(); i += 2)
	{
		seedRanges.push_back(Range{ p.StartingSeeds[i], p.StartingSeeds[i] + p.StartingSeeds[i + 1] });
	}

	int64_t answer = numeric_limits<int64_t>::max();
	for (const Range& rangeToSplit : seedRanges)
	{
		list<Range> workingRanges = { rangeToSplit };
		for (size_t stage = 0; stage < p.Stages.size(); stage++)
		{
			list<Range> newRanges = TransformByStage(workingRanges, p.Stages[stage]);
			workingRanges = newRanges;
		}
		for (const Range& finalRange : workingRanges)
		{
			answer = min(finalRange.Begin, answer);
		}
	}

	printf("[2023] Puzzle05_B: %" PRId64 "\n", answer);
}

void Puzzle05_2023(const string& filename)
{
	Puzzle05_A(filename);
	Puzzle05_B(filename);
}
