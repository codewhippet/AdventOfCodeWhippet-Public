#pragma once

#include <vector>
#include <fstream>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

std::string ReadSingleLine(std::istream& input);
std::vector<std::string> ReadAllLines(std::istream& input);
std::vector<std::string> ReadAllLines(const char* filename);

//////////////////////////////////////////////////////////////////////////

class ILineSource
{
public:
	virtual ~ILineSource();
	virtual const char* GetNextLine(const char* currentLine) const = 0;

	static std::shared_ptr<ILineSource> CreateFromFile(const char* filename);
	static std::shared_ptr<ILineSource> CreateFromString(const std::string& s);
	static std::shared_ptr<ILineSource> CreateFromStream(std::istream &input);
};

//////////////////////////////////////////////////////////////////////////

struct InputLineIterator
{
	using value_type = std::string;
	using difference_type = ptrdiff_t;

	std::shared_ptr<ILineSource> Input;
	const char* CurrentLine = nullptr;
	std::string Line;

	InputLineIterator() = default;
	explicit InputLineIterator(const std::shared_ptr<ILineSource>& input)
		: Input(input)
	{
		ReadNextLine();
	}

	InputLineIterator& operator++()
	{
		ReadNextLine();
		return *this;
	}

	InputLineIterator operator++(int)
	{
		InputLineIterator copy(*this);
		ReadNextLine();
		return copy;
	}

	const std::string& operator*() const
	{
		return Line;
	}

	const std::string* operator->() const
	{
		return &Line;
	}

private:
	void ReadNextLine()
	{
		CurrentLine = Input->GetNextLine(CurrentLine);
		if (CurrentLine == nullptr)
		{
			Input.reset();
			Line.clear();
			return;
		}

		Line = CurrentLine;
	}
};

inline bool operator==(const InputLineIterator& a, const InputLineIterator& b)
{
	// Minimum support for comparing against end sentinel
	return a.CurrentLine == b.CurrentLine;
}

inline bool operator!=(const InputLineIterator& a, const InputLineIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<InputLineIterator>);

struct InputLineRange
{
	std::shared_ptr<ILineSource> Input;

	InputLineRange() = default;
	explicit InputLineRange(const std::shared_ptr<ILineSource>& input)
		: Input(input)
	{
	}

	InputLineIterator begin() const
	{
		return InputLineIterator{ Input };
	}

	InputLineIterator end() const
	{
		return {};
	}
};

static_assert(std::ranges::input_range<InputLineRange>);

InputLineRange ReadEachLine(std::istream& input);

//////////////////////////////////////////////////////////////////////////

struct InputLineWithNumberIterator
{
	using value_type = std::pair<std::string, int64_t>;
	using difference_type = ptrdiff_t;

	std::shared_ptr<ILineSource> Input;
	const char* CurrentLine = nullptr;
	std::pair<std::string, int64_t> Line = { {}, -1 };

	InputLineWithNumberIterator() = default;
	explicit InputLineWithNumberIterator(const std::shared_ptr<ILineSource>& input)
		: Input(input)
	{
		ReadNextLine();
	}

	InputLineWithNumberIterator& operator++()
	{
		ReadNextLine();
		return *this;
	}

	InputLineWithNumberIterator operator++(int)
	{
		InputLineWithNumberIterator copy(*this);
		ReadNextLine();
		return copy;
	}

	const std::pair<std::string, int64_t>& operator*() const
	{
		return Line;
	}

	const std::pair<std::string, int64_t>* operator->() const
	{
		return &Line;
	}

private:
	void ReadNextLine()
	{
		Line.second++;
		CurrentLine = Input->GetNextLine(CurrentLine);
		if (CurrentLine == nullptr)
		{
			Input.reset();
			Line.first.clear();
			return;
		}

		Line.first = CurrentLine;
	}
};

inline bool operator==(const InputLineWithNumberIterator& a, const InputLineWithNumberIterator& b)
{
	// Minimum support for comparing against end sentinel
	return a.CurrentLine == b.CurrentLine;
}

inline bool operator!=(const InputLineWithNumberIterator& a, const InputLineWithNumberIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<InputLineWithNumberIterator>);

struct InputLineWithNumberRange
{
	std::shared_ptr<ILineSource> Input;

	InputLineWithNumberRange() = default;
	explicit InputLineWithNumberRange(const std::shared_ptr<ILineSource>& input)
		: Input(input)
	{
	}

	InputLineWithNumberIterator begin() const
	{
		return InputLineWithNumberIterator{ Input };
	}

	InputLineWithNumberIterator end() const
	{
		return {};
	}
};

static_assert(std::ranges::input_range<InputLineWithNumberRange>);

InputLineWithNumberRange ReadEachLineWithNumber(std::istream& input);

//////////////////////////////////////////////////////////////////////////

struct InputLineRegexIterator
{
	using value_type = std::smatch;
	using difference_type = ptrdiff_t;

	std::shared_ptr<ILineSource> Input;
	const char* CurrentLine = nullptr;
	std::string Line;
	std::regex Pattern;
	std::smatch Match;

	InputLineRegexIterator() = default;
	InputLineRegexIterator(const std::shared_ptr<ILineSource>& input, const std::regex& pattern)
		: Input(input)
		, Pattern(pattern)
	{
		ReadNextLine();
	}

	InputLineRegexIterator(const InputLineRegexIterator& other)
		: Input(other.Input)
		, Pattern(other.Pattern)
	{
		if (Input)
		{
			ReadNextLine();
		}
	}

	InputLineRegexIterator& operator++()
	{
		ReadNextLine();
		return *this;
	}

	InputLineRegexIterator operator++(int)
	{
		InputLineRegexIterator copy(*this);
		ReadNextLine();
		return copy;
	}

	const std::smatch& operator*() const
	{
		return Match;
	}

	const std::smatch* operator->() const
	{
		return &Match;
	}

private:
	void ReadNextLine()
	{
		CurrentLine = Input->GetNextLine(CurrentLine);
		if (CurrentLine == nullptr)
		{
			Input.reset();
			Line.clear();
			return;
		}

		Line = CurrentLine;
		std::regex_match(Line, Match, Pattern);
	}
};

inline bool operator==(const InputLineRegexIterator& a, const InputLineRegexIterator& b)
{
	// Minimum support for comparing against end sentinel
	return a.CurrentLine == b.CurrentLine;
}

inline bool operator!=(const InputLineRegexIterator& a, const InputLineRegexIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<InputLineRegexIterator>);

struct InputLineRegexRange
{
	std::shared_ptr<ILineSource> Input;
	std::regex Pattern;

	InputLineRegexRange() = default;
	InputLineRegexRange(const std::shared_ptr<ILineSource>& input, const std::regex& pattern)
		: Input(input)
		, Pattern(pattern)
	{
	}

	InputLineRegexIterator begin() const
	{
		return InputLineRegexIterator{ Input, Pattern };
	}

	InputLineRegexIterator end() const
	{
		return {};
	}
};

static_assert(std::ranges::input_range<InputLineRegexRange>);

InputLineRegexRange ReadEachLine(std::istream& input, const std::regex& pattern);

/////////////////////////////////////////////////////////////////////////

template <typename... Types>
struct InputLineScanfIterator
{
	using value_type = std::tuple<Types...>;
	using difference_type = ptrdiff_t;

	InputLineScanfIterator() = default;
	InputLineScanfIterator(const std::shared_ptr<ILineSource> &input, const std::string& format)
		: Input(input)
		, Format(format)
	{
		ReadNextLine();
	}

	InputLineScanfIterator& operator++()
	{
		ReadNextLine();
		return *this;
	}

	InputLineScanfIterator operator++(int)
	{
		InputLineRegexIterator copy(*this);
		ReadNextLine();
		return copy;
	}

	const value_type& operator*() const
	{
		return Current;
	}

	const value_type* operator->() const
	{
		return &Current;
	}

	inline bool operator==(const InputLineScanfIterator& other) const
	{
		// Minimum support for comparing against end sentinel
		return CurrentLine == other.CurrentLine;
	}

private:

	template <typename T1>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current));
		assert(scanned == 1);
		(void)scanned;
	}

	template <typename T1, typename T2>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current));
		assert(scanned == 2);
		(void)scanned;
	}

	template <typename T1, typename T2, typename T3>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current),
			&std::get<2>(Current));
		assert(scanned == 3);
		(void)scanned;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current),
			&std::get<2>(Current),
			&std::get<3>(Current));
		assert(scanned == 4);
		(void)scanned;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current),
			&std::get<2>(Current),
			&std::get<3>(Current),
			&std::get<4>(Current));
		assert(scanned == 5);
		(void)scanned;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current),
			&std::get<2>(Current),
			&std::get<3>(Current),
			&std::get<4>(Current),
			&std::get<5>(Current),
			&std::get<6>(Current));
		assert(scanned == 7);
		(void)scanned;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	void DoScanf()
	{
		int scanned = sscanf(CurrentLine, Format.c_str(),
			&std::get<0>(Current),
			&std::get<1>(Current),
			&std::get<2>(Current),
			&std::get<3>(Current),
			&std::get<4>(Current),
			&std::get<5>(Current),
			&std::get<6>(Current),
			&std::get<7>(Current),
			&std::get<8>(Current));
		assert(scanned == 9);
		(void)scanned;
	}

	void ReadNextLine()
	{
		CurrentLine = Input->GetNextLine(CurrentLine);
		if (CurrentLine == nullptr)
		{
			Input.reset();
			return;
		}
		DoScanf<Types...>();
	}

	std::shared_ptr<ILineSource> Input;
	const char* CurrentLine = nullptr;
	std::string Format;
	value_type Current;
};

template <typename... Types>
inline bool operator!=(const InputLineScanfIterator<Types...>& a, const InputLineScanfIterator<Types...>& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<InputLineScanfIterator<int>>);

template <typename... Types>
class InputLineScanfRange : public std::ranges::view_interface<InputLineScanfRange<Types...>>
{
public:
	InputLineScanfRange() = default;
	InputLineScanfRange(std::istream& input, const std::string& format)
		: Input(ILineSource::CreateFromStream(input))
		, Format(format)
	{
	}

	InputLineScanfIterator<Types...> begin() const
	{
		return { Input, Format };
	}

	InputLineScanfIterator<Types...> end() const
	{
		return {};
	}
private:
	std::shared_ptr<ILineSource> Input;
	std::string Format;
};

template <typename... Types>
InputLineScanfRange<Types...> ScanfEachLine(std::istream& input, const std::string& format)
{
	return { input, format };
}

/////////////////////////////////////////////////////////////////////////
