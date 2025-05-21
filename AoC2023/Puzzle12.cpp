#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2023_Types
{
	struct Spring2
	{
		string Pattern;
		vector<uint8_t> Blocks;
	};
}

using namespace Puzzle12_2023_Types;

static Spring2 SpringCriteria2(const string& s, int duplicateN)
{
	char binaryConfig[33] = { 0 };
	char blockList[32] = { 0 };
	int extracted = sscanf(s.c_str(), "%32[.?#] %31[0-9,]", binaryConfig, blockList);
	assert(extracted == 2);
	(void)extracted;

	Spring2 spring;
	spring.Pattern = string{ binaryConfig };
	for (int i = 1; i < duplicateN; i++)
	{
		spring.Pattern += string{ "?" };
		spring.Pattern.append(binaryConfig);
	}

	string blocks{ blockList };
	replace(blocks.begin(), blocks.end(), ',', ' ');

	vector<uint8_t> blockVec;
	for (int64_t block : ReadAsVectorOfNumbers(blocks))
	{
		blockVec.push_back((uint8_t)block);
	}

	for (int i = 0; i < duplicateN; i++)
	{
		spring.Blocks.insert(spring.Blocks.end(), blockVec.begin(), blockVec.end());
	}

	return spring;
}

static bool DoesBlockFit(size_t blockIndex, size_t charIndex, const Spring2& spring)
{
	uint8_t blockSize = spring.Blocks[blockIndex];
	if ((charIndex + blockSize) > spring.Pattern.size())
	{
		return false;
	}

	for (size_t i = 0; i < blockSize; i++)
	{
		char c = spring.Pattern[charIndex + i];
		if (c == '.')
		{
			return false;
		}
	}

	return true;
}

static int64_t GetCombinations(size_t blockIndex, size_t charIndex, const Spring2& spring, map<pair<size_t, size_t>, int64_t>* cache, string debugString)
{
	pair<size_t, size_t> cacheKey{ blockIndex, charIndex };
	map<pair<size_t, size_t>, int64_t>::const_iterator existingSolution = cache->find(cacheKey);
	if (existingSolution != cache->end())
	{
		return existingSolution->second;
	}

	int64_t combinations = 0;

	// Does this block fit at this address?
	if (DoesBlockFit(blockIndex, charIndex, spring))
	{
		uint8_t blockSize = spring.Blocks[blockIndex];

		for (int i = 0; i < blockSize; i++)
		{
			debugString[charIndex + i] = '+';
		}

		size_t nextCharIndex = charIndex + blockSize + 1;
		size_t nextRequired = spring.Pattern.find('#', nextCharIndex - 1);

		// Is this our last block?
		if (blockIndex == spring.Blocks.size() - 1)
		{
			if (nextRequired == string::npos)
			{
				//printf("%s\n", spring.Pattern.c_str());
				//printf("%s\n\n", debugString.c_str());
				combinations = 1;
			}
		}
		else
		{
			nextRequired = (nextRequired == string::npos ? spring.Pattern.size() - 1 : nextRequired);
			for (size_t i = nextCharIndex; i <= nextRequired; i++)
			{
				combinations += GetCombinations(blockIndex + 1, i, spring, cache, debugString);
			}
		}
	}

	cache->insert(make_pair(cacheKey, combinations));
	return combinations;
}

static int64_t GetCombinations(const Spring2& spring)
{
	map<pair<size_t, size_t>, int64_t> combinationCache;
	int64_t combinations = 0;
	size_t nextRequired = spring.Pattern.find('#');
	nextRequired = (nextRequired == string::npos ? spring.Pattern.size() - 1 : nextRequired);
	for (size_t i = 0; i <= nextRequired; i++)
	{
		combinations += GetCombinations(0, i, spring, &combinationCache, spring.Pattern);
	}
	return combinations;
}

static void Puzzle12_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Spring2> springs = MakeEnumerator(ReadAllLines(input))
		->Select<Spring2>([](const string& s) { return SpringCriteria2(s, 1); })
		->ToVector();

	int64_t answer = 0;
	for (const Spring2& s : springs)
	{
		int64_t validConfigurations = GetCombinations(s);
		answer += validConfigurations;
	}

	printf("[2023] Puzzle12_A: %" PRId64 "\n", answer);
}

static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Spring2> springs = MakeEnumerator(ReadAllLines(input))
		->Select<Spring2>([](const string& s) { return SpringCriteria2(s, 5); })
		->ToVector();

	int64_t answer = 0;
	for (const Spring2& s : springs)
	{
		int64_t validConfigurations = GetCombinations(s);
		answer += validConfigurations;
	}

	printf("[2023] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2023(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
