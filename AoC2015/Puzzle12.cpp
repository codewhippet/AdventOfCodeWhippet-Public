#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle12_2015_Types
{
	enum ElfJsonType
	{
		Integer,
		String,
		Object,
		Array
	};

	struct ElfJsonValue
	{
		ElfJsonType Type;

		int AsInt;
		string AsString;
		map<string, shared_ptr<ElfJsonValue>> AsObject;
		vector<shared_ptr<ElfJsonValue>> AsArray;
	};
}

using namespace Puzzle12_2015_Types;

// Because Parsing JSON is a Minefield [https://seriot.ch/projects/parsing_json.html] we
// only parse the subset that's used in the puzzle input: ElfJson. There's no whitespace,
// no escaped characters in the strings, no constants other than positive and negative
// integers.

static const char* TryParseStringLiteral(const char* buffer, string* value);

static const char* TryParseElfJsonValue(const char* buffer, shared_ptr<ElfJsonValue>* value);
static const char* TryParseElfJsonInteger(const char* buffer, shared_ptr<ElfJsonValue>* value);
static const char* TryParseElfJsonString(const char* buffer, shared_ptr<ElfJsonValue>* value);
static const char* TryParseElfJsonObject(const char* buffer, shared_ptr<ElfJsonValue>* value);
static const char* TryParseElfJsonArray(const char* buffer, shared_ptr<ElfJsonValue>* value);

static const char *TryParseElfJsonValue(const char* buffer, shared_ptr<ElfJsonValue> *value)
{
	switch (buffer[0])
	{
	case '[':
		return TryParseElfJsonArray(buffer, value);

	case '{':
		return TryParseElfJsonObject(buffer, value);

	case '"':
		return TryParseElfJsonString(buffer, value);

	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return TryParseElfJsonInteger(buffer, value);
	}

	return nullptr;
}

static const char* TryParseElfJsonInteger(const char* buffer, shared_ptr<ElfJsonValue>* value)
{
	*value = make_shared<ElfJsonValue>();
	(*value)->Type = ElfJsonType::Integer;
	(*value)->AsInt = atoi(buffer);

	do
	{
		buffer++;

	} while (isdigit(buffer[0]));

	return buffer;
}

static const char* TryParseStringLiteral(const char* buffer, string* value)
{
	assert(buffer[0] == '"');

	const char* openingCharcter = buffer + 1;
	const char* closingQuote = openingCharcter;
	do
	{
		closingQuote++;

	} while (closingQuote[0] != '"');

	*value = string{ openingCharcter, closingQuote };

	return closingQuote + 1;
}

static const char* TryParseElfJsonString(const char* buffer, shared_ptr<ElfJsonValue>* value)
{
	*value = make_shared<ElfJsonValue>();
	(*value)->Type = ElfJsonType::String;

	return TryParseStringLiteral(buffer, &(*value)->AsString);
}

static const char* TryParseElfJsonObject(const char* buffer, shared_ptr<ElfJsonValue>* value)
{
	*value = make_shared<ElfJsonValue>();
	(*value)->Type = ElfJsonType::Object;

	assert(buffer[0] == '{');
	while ((buffer[0] == '{') || (buffer[0] == ','))
	{
		buffer++;
		if (buffer[0] == '}')
			break;

		string key;
		buffer = TryParseStringLiteral(buffer, &key);

		assert(buffer[0] == ':');
		buffer++;

		shared_ptr<ElfJsonValue> subValue = make_shared<ElfJsonValue>();
		buffer = TryParseElfJsonValue(buffer, &subValue);
		assert(buffer != nullptr);

		(*value)->AsObject[key] = subValue;
	}
	assert(buffer[0] == '}');

	return buffer + 1;
}

static const char* TryParseElfJsonArray(const char* buffer, shared_ptr<ElfJsonValue>* value)
{
	*value = make_shared<ElfJsonValue>();
	(*value)->Type = ElfJsonType::Array;

	assert(buffer[0] == '[');
	while ((buffer[0] == '[') || (buffer[0] == ','))
	{
		buffer++;
		if (buffer[0] == ']')
			break;

		shared_ptr<ElfJsonValue> subValue = make_shared<ElfJsonValue>();
		buffer = TryParseElfJsonValue(buffer, &subValue);
		assert(buffer != nullptr);

		(*value)->AsArray.push_back(subValue);
	}
	assert(buffer[0] == ']');

	return buffer + 1;
}

static int64_t AddAllNumbersExcludingRedObjects(const shared_ptr<ElfJsonValue>& value)
{
	switch (value->Type)
	{
	case ElfJsonType::Integer:
		return value->AsInt;

	case ElfJsonType::Object:
		{
			int64_t total = 0;
			for (const auto& kvp : value->AsObject)
			{
				if (kvp.second->Type == ElfJsonType::String && kvp.second->AsString == "red")
					return 0;

				total += AddAllNumbersExcludingRedObjects(kvp.second);
			}
			return total;
		}

	case ElfJsonType::Array:
		{
			int64_t total = 0;
			for (const auto& subValue : value->AsArray)
			{
				total += AddAllNumbersExcludingRedObjects(subValue);
			}
			return total;
		}
	}

	return 0;
}

static void Puzzle12_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string document;
	getline(input, document);

	int64_t answer = 0;
	regex number(R"(-?\d+)");
	for (sregex_iterator it = sregex_iterator(document.begin(), document.end(), number); it != sregex_iterator{}; ++it)
	{
		answer += atoi(it->str().c_str());
	}

	printf("[2015] Puzzle12_A: %" PRId64 "\n", answer);
}


static void Puzzle12_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	{
		string test;
		assert(TryParseStringLiteral(R"("abc")", &test)[0] == '\0');
		assert(test == "abc");
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonInteger(R"(123)", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Integer);
		assert(test->AsInt == 123);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonString(R"("abc")", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::String);
		assert(test->AsString == "abc");
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonObject(R"({})", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Object);
		assert(test->AsObject.size() == 0);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonObject(R"({"abc":123})", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Object);
		assert(test->AsObject.size() == 1);
		assert(test->AsObject.at("abc")->Type == ElfJsonType::Integer);
		assert(test->AsObject.at("abc")->AsInt == 123);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonObject(R"({"abc":123,"def":{},"ghj":[]})", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Object);
		assert(test->AsObject.size() == 3);
		assert(test->AsObject.at("abc")->Type == ElfJsonType::Integer);
		assert(test->AsObject.at("abc")->AsInt == 123);
		assert(test->AsObject.at("def")->Type == ElfJsonType::Object);
		assert(test->AsObject.at("def")->AsObject.size() == 0);
		assert(test->AsObject.at("ghj")->Type == ElfJsonType::Array);
		assert(test->AsObject.at("ghj")->AsArray.size() == 0);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonArray(R"([])", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Array);
		assert(test->AsObject.size() == 0);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonArray(R"([123])", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Array);
		assert(test->AsArray.size() == 1);
		assert(test->AsArray[0]->Type == ElfJsonType::Integer);
		assert(test->AsArray[0]->AsInt == 123);
	}

	{
		shared_ptr<ElfJsonValue> test;
		assert(TryParseElfJsonArray(R"([123,{},[]])", &test)[0] == '\0');
		assert(test->Type == ElfJsonType::Array);
		assert(test->AsArray.size() == 3);
		assert(test->AsArray[0]->Type == ElfJsonType::Integer);
		assert(test->AsArray[0]->AsInt == 123);
		assert(test->AsArray[1]->Type == ElfJsonType::Object);
		assert(test->AsArray[1]->AsObject.size() == 0);
		assert(test->AsArray[2]->Type == ElfJsonType::Array);
		assert(test->AsArray[2]->AsArray.size() == 0);
	}

	string documentSource;
	getline(input, documentSource);

	shared_ptr<ElfJsonValue> document;
	const char* endOfDocument = TryParseElfJsonValue(documentSource.c_str(), &document);
	assert(endOfDocument[0] == '\0');
	(void)endOfDocument;

	int64_t answer = AddAllNumbersExcludingRedObjects(document);

	printf("[2015] Puzzle12_B: %" PRId64 "\n", answer);
}

void Puzzle12_2015(const string& filename)
{
	Puzzle12_A(filename);
	Puzzle12_B(filename);
}
