#include "stdafx.h"
#include "FileInput.h"

//////////////////////////////////////////////////////////////////////////

std::string ReadSingleLine(std::istream& input)
{
	std::string line;
	std::getline(input, line);
	return line;
}

std::vector<std::string> ReadAllLines(std::istream& input)
{
	std::vector<std::string> lines;
	while (true)
	{
		std::string line;
		if (!std::getline(input, line))
			break;

		lines.push_back(line);
	}

	return lines;
}

std::vector<std::string> ReadAllLines(const char* filename)
{
	std::ifstream input(filename);
	return ReadAllLines(input);
}

//////////////////////////////////////////////////////////////////////////

InputLineRange ReadEachLine(std::istream& input)
{
	return InputLineRange{ ILineSource::CreateFromStream(input) };
}

InputLineWithNumberRange ReadEachLineWithNumber(std::istream& input)
{
	return InputLineWithNumberRange{ ILineSource::CreateFromStream(input) };
}

InputLineRegexRange ReadEachLine(std::istream& input, const std::regex& pattern)
{
	return InputLineRegexRange{ ILineSource::CreateFromStream(input), pattern };
}

//////////////////////////////////////////////////////////////////////////

ILineSource::~ILineSource()
{
}

//////////////////////////////////////////////////////////////////////////

class LineSource_String : public ILineSource
{
public:
	LineSource_String(const std::string &source)
	{
		CopyString(source);
	}

	const char* GetNextLine(const char* currentLine) const override
	{
		if (currentLine == nullptr)
		{
			return Data.data();
		}

		const char* end = Data.data() + Data.size();

		assert(currentLine >= Data.data());
		assert(currentLine < end);

		// Find the end of the line (we're guaranteed a null terminator)
		while (*currentLine != '\0')
			currentLine++;

		// Next line
		currentLine++;

		return (currentLine >= end ? nullptr : currentLine);
	}

private:

	void CopyString(const std::string& source)
	{
		Data.resize(source.size() + 1);
		Data.back() = '\0';
		memcpy(Data.data(), source.data(), source.size());
		std::ranges::replace(Data, '\n', '\0');
	}

	std::vector<char> Data;
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

class LineSource_Stream : public ILineSource
{
public:
	LineSource_Stream(std::istream& input)
	{
		ReadStream(input);
	}

	const char* GetNextLine(const char* currentLine) const override
	{
		if (currentLine == nullptr)
		{
			return Data.data();
		}

		const char* end = Data.data() + Data.size();

		assert(currentLine >= Data.data());
		assert(currentLine < end);

		// Find the end of the line (we're guaranteed a null terminator)
		while (*currentLine != '\0')
			currentLine++;

		// Next line
		currentLine++;

		return (currentLine >= end ? nullptr : currentLine);
	}

private:

	void ReadStream(std::istream& input)
	{
		Data.reserve(1024);
		for (int c = input.get(); c != EOF; c = input.get())
		{
			Data.push_back((char)c);
		}
		std::ranges::replace(Data, '\n', '\0');
		if (Data.back() != '\0')
		{
			Data.push_back('\0');
		}
	}

	std::vector<char> Data;
};

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<ILineSource> ILineSource::CreateFromFile(const char* filename)
{
	(void)filename;
	return std::shared_ptr<ILineSource>();
}

std::shared_ptr<ILineSource> ILineSource::CreateFromString(const std::string& s)
{
	return std::make_shared<LineSource_String>(s);
}

std::shared_ptr<ILineSource> ILineSource::CreateFromStream(std::istream& input)
{
	return std::make_shared<LineSource_Stream>(input);
}

//////////////////////////////////////////////////////////////////////////
