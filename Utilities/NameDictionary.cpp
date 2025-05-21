#include "stdafx.h"
#include "NameDictionary.h"

int64_t NameDictionary::IdFromName(const std::string& name) const
{
	std::map<std::string, int64_t>::const_iterator existingId = NameToId.find(name);
	if (existingId == NameToId.end())
	{
		int64_t newId = NameToId.size();
		existingId = NameToId.insert({ name, newId }).first;
		IdToName.insert({ newId, name });
	}
	return existingId->second;
}

std::string NameDictionary::NameFromId(int64_t id) const
{
	return IdToName.at(id);
}
