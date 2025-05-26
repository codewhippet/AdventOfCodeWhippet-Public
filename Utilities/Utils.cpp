#include "stdafx.h"
#include "Utils.h"

std::vector<std::string> StringSplit(std::string source, char delim)
{
	std::vector<std::string> splits;
	splits.reserve((source.size() / 3) + 1);
	while (source.size())
	{
		size_t delimPos = source.find(delim);
		std::string split = source.substr(0, delimPos);
		size_t splitSize = split.size();
		if (splitSize > 0)
		{
			splits.push_back(std::move(split));
		}
		if (splitSize < source.size())
		{
			source = source.substr(splitSize + 1);
		}
		else
		{
			source.clear();
		}
	}
	return splits;
}

std::vector<std::string> StringSplitTrimmed(std::string source, char delim)
{
	std::vector<std::string> splits;
	splits.reserve((source.size() / 3) + 1);
	while (source.size())
	{
		size_t delimPos = source.find(delim);
		std::string split = source.substr(0, delimPos);
		size_t splitSize = split.size();
		if (splitSize > 0)
		{
			splits.push_back(Trim(split));
		}
		if (splitSize < source.size())
		{
			source = source.substr(splitSize + 1);
		}
		else
		{
			source.clear();
		}
	}
	return splits;
}

std::string Trim(const std::string& s)
{
	std::string trimmed = s;
	size_t firstChar = trimmed.find_first_not_of(" \n\r\t");
	if (firstChar == std::string::npos)
	{
		return {};
	}

	size_t lastChar = trimmed.find_last_not_of(" \n\r\t");
	return trimmed.substr(firstChar, lastChar - firstChar + 1);
}

int64_t ReadFirstNumber(const char* c)
{
	while (c[0] != '\0' && !isdigit(c[0]) && c[0] != '-')
		c++;

	if (c[0] == '\0')
		return 0;

	return atoll(c);
}

int64_t ReadFirstNumber(const std::string& s)
{
	return ReadFirstNumber(s.c_str());
}

std::vector<int64_t> ReadAsVectorOfNumbers(const char* c)
{
	std::vector<int64_t> numbers;

	while (true)
	{
		while (c[0] != '\0' && !isdigit(c[0]) && c[0] != '-')
			c++;

		if (c[0] == '\0')
			break;

		numbers.push_back(atoll(c));

		while (isdigit(c[0]) || c[0] == '-')
			c++;
	}

	return numbers;
}

std::vector<int64_t> ReadAsVectorOfNumbers(const std::string& s)
{
	return ReadAsVectorOfNumbers(s.data());
}

int64_t Utils::ToNumber(const std::ssub_match& m)
{
	return atoll(m.str().c_str());
}

AllUnorderedPairsRange AllUnorderedPairs(int64_t size)
{
	return AllUnorderedPairsRange{ size };
}
