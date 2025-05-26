#pragma once

#include <string>
#include <map>
#include <stdint.h>

class NameDictionary
{
public:

	int64_t IdFromName(const std::string& name) const;
	std::string NameFromId(int64_t id) const;

private:

	//#TODO: make this thread safe, just in case we want to thread any puzzles in future
	mutable std::map<int64_t, std::string> IdToName;
	mutable std::map<std::string, int64_t> NameToId;
};
