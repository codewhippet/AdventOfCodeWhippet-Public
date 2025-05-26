#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle10_2016_Types
{
	struct Bot
	{
		int64_t Id;
		vector<int64_t> Values;
		int64_t LowTo;
		int64_t HighTo;
	};

	struct Puzzle
	{
		vector<pair<int64_t, int64_t>> Values;
		map<int64_t, shared_ptr<Bot>> Bots;
	};
}

using namespace Puzzle10_2016_Types;

struct Puzzle ReadPuzzle(istream& input, const int64_t outputIdOffset)
{
	Puzzle p;

	for (const string& line : ReadEachLine(input))
	{
		if (line[0] == 'v')
		{
			pair<int64_t, int64_t> value;
			int scanned = sscanf(line.c_str(), "value %lld goes to bot %lld",
				&value.first,
				&value.second);
			assert(scanned == 2);
			(void)scanned;

			p.Values.push_back(value);
		}
		else if (line[0] == 'b')
		{
			auto bot = make_shared<Bot>();
			char lowToEntity[16] = { 0 };
			char highToEntity[16] = { 0 };
			int scanned = sscanf(line.c_str(), "bot %lld gives low to %s %lld and high to %s %lld",
				&bot->Id,
				lowToEntity,
				&bot->LowTo,
				highToEntity,
				&bot->HighTo);
			assert(scanned == 5);
			(void)scanned;

			assert(bot->Id < outputIdOffset);
			p.Bots.insert({ bot->Id, bot });

			if (lowToEntity == "output"sv)
			{
				bot->LowTo += outputIdOffset;
				p.Bots.insert({ bot->LowTo, make_shared<Bot>(Bot{ bot->LowTo }) });
			}
			if (highToEntity == "output"sv)
			{
				bot->HighTo += outputIdOffset;
				p.Bots.insert({ bot->HighTo, make_shared<Bot>(Bot{ bot->HighTo }) });
			}
		}
		else
		{
			assert(false);
		}
	}

	return p;
}

static void Simulate(Puzzle* p)
{
	function<void(int64_t, int64_t)> GiveValueToBot =
		[&](int64_t value, int64_t botId)
		{
			shared_ptr<Bot> bot = p->Bots.at(botId);
			bot->Values.push_back(value);
			assert(bot->Values.size() < 3);
			if (bot->Values.size() == 2)
			{
				GiveValueToBot(min(bot->Values[0], bot->Values[1]), bot->LowTo);
				GiveValueToBot(max(bot->Values[0], bot->Values[1]), bot->HighTo);
			}
		};

	for (const auto& kvp : p->Values)
	{
		GiveValueToBot(kvp.first, kvp.second);
	}
}

static void Puzzle10_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t outputIdOffset = 1000;

	Puzzle p = ReadPuzzle(input, outputIdOffset);
	Simulate(&p);

	int64_t answer = 0;
	for (const auto& kvp : p.Bots)
	{
		shared_ptr<Bot> bot = kvp.second;
		if ((bot->Values.size() == 2) &&
			(min(bot->Values[0], bot->Values[1]) == 17) &&
			(max(bot->Values[0], bot->Values[1]) == 61))
		{
			answer = bot->Id;
			break;
		}
	}

	printf("[2016] Puzzle10_A: %" PRId64 "\n", answer);
}


static void Puzzle10_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t outputIdOffset = 1000;

	Puzzle p = ReadPuzzle(input, outputIdOffset);
	Simulate(&p);

	assert(p.Bots.at(outputIdOffset + 0)->Values.size() == 1);
	assert(p.Bots.at(outputIdOffset + 1)->Values.size() == 1);
	assert(p.Bots.at(outputIdOffset + 2)->Values.size() == 1);

	int64_t answer =
		p.Bots.at(outputIdOffset + 0)->Values[0] *
		p.Bots.at(outputIdOffset + 1)->Values[0] *
		p.Bots.at(outputIdOffset + 2)->Values[0];

	printf("[2016] Puzzle10_B: %" PRId64 "\n", answer);
}

void Puzzle10_2016(const string& filename)
{
	Puzzle10_A(filename);
	Puzzle10_B(filename);
}
