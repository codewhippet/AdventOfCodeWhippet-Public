#pragma once

#include "Enumerable.h"
#include "Point2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix43.h"
#include "ArrayMap2D.h"
#include "PointMap.h"
#include "MD5.h"
#include "NameDictionary.h"
#include "FileInput.h"
#include "Hex.h"
#include "PointIteration.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <regex>
#include <bitset>
#include <deque>
#include <array>
#include <ranges>
#include <bit>

#include <assert.h>
#include <inttypes.h>

//////////////////////////////////////////////////////////////////////////

std::vector<std::string> StringSplit(std::string source, char delim);
std::vector<std::string> StringSplitTrimmed(std::string source, char delim);
std::string Trim(const std::string& s);
int64_t ReadFirstNumber(const char *c);
int64_t ReadFirstNumber(const std::string& s);
std::vector<int64_t> ReadAsVectorOfNumbers(const char *c);
std::vector<int64_t> ReadAsVectorOfNumbers(const std::string& s);

//////////////////////////////////////////////////////////////////////////

namespace Utils
{
	int64_t ToNumber(const std::ssub_match &m);
}

//////////////////////////////////////////////////////////////////////////

struct AllUnorderedPairsIterator
{
	using value_type = std::pair<int64_t, int64_t>;
	using difference_type = ptrdiff_t;

	std::pair<int64_t, int64_t> Current = { 0, 0 };
	int64_t Size = 0;

	AllUnorderedPairsIterator() = default;
	explicit AllUnorderedPairsIterator(int64_t size)
		: Size(size)
	{
		assert(Size > 0);
		NextPair();
	}

	AllUnorderedPairsIterator& operator++()
	{
		NextPair();
		return *this;
	}

	AllUnorderedPairsIterator operator++(int)
	{
		AllUnorderedPairsIterator copy(*this);
		NextPair();
		return copy;
	}

	const std::pair<int64_t, int64_t>& operator*() const
	{
		return Current;
	}

	const std::pair<int64_t, int64_t>* operator->() const
	{
		return &Current;
	}

private:
	void NextPair()
	{
		if (Size > 0)
		{
			Current.second++;
			if (Current.second == Size)
			{
				Current.first++;
				Current.second = Current.first + 1;
				if (Current.second == Size)
				{
					Size = 0;
				}
			}
		}
	}
};

inline bool operator==(const AllUnorderedPairsIterator& a, const AllUnorderedPairsIterator& b)
{
	// Minimum support for comparing against end sentinel
	return a.Size == b.Size;
}

inline bool operator!=(const AllUnorderedPairsIterator& a, const AllUnorderedPairsIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<AllUnorderedPairsIterator>);

struct AllUnorderedPairsRange
{
	int64_t Size = 0;

	AllUnorderedPairsRange() = default;
	explicit AllUnorderedPairsRange(int64_t size)
		: Size(size)
	{
	}

	AllUnorderedPairsIterator begin() const
	{
		return Size > 1 ? AllUnorderedPairsIterator{ Size } : AllUnorderedPairsIterator{};
	}

	AllUnorderedPairsIterator end() const
	{
		return {};
	}
};

static_assert(std::ranges::input_range<InputLineRange>);

AllUnorderedPairsRange AllUnorderedPairs(int64_t size);

//////////////////////////////////////////////////////////////////////////

template <typename VALUE_TYPE>
struct ICustomIterator
{
	using value_type = VALUE_TYPE;

	enum class NextValueResult
	{
		MoreValuesAvailable,
		NoMoreValues,
	};

	virtual ~ICustomIterator() {}

	virtual NextValueResult Next(VALUE_TYPE *current) = 0;

	virtual NextValueResult Ctor(VALUE_TYPE *)
	{
		return NextValueResult::MoreValuesAvailable;
	}

	bool operator==(const ICustomIterator&) const = default;
};

struct ICustomIteratorTest : ICustomIterator<char>
{
	virtual NextValueResult Next(char *) override
	{
		return NextValueResult::NoMoreValues;
	}
};

template <typename CUSTOM_TYPE>
struct CustomIterator
{
	using value_type = CUSTOM_TYPE::value_type;
	using difference_type = ptrdiff_t;

	value_type Current = {};

	CustomIterator() = default;
	explicit CustomIterator(const std::shared_ptr<CUSTOM_TYPE> &iterator)
		: Iterator(iterator)
	{
		if (Iterator)
		{
			NextValue = Iterator->Ctor(&Current);
		}
	}

	CustomIterator(const CustomIterator& other)
		: Current(other.Current)
		, NextValue(other.NextValue)
		, Iterator(other.Iterator ? std::make_shared<CUSTOM_TYPE>(*other.Iterator) : nullptr)
	{
	}

	CustomIterator(const CustomIterator&& other) noexcept
		: Current(std::move(other.Current))
		, NextValue(other.NextValue)
		, Iterator(std::move(other.Iterator))
	{
	}

	CustomIterator &operator=(const CustomIterator& other)
	{
		Current = other.Current;
		NextValue = other.NextValue;
		Iterator = other.Iterator ? std::make_shared<CUSTOM_TYPE>(*other.Iterator) : nullptr;
	}

	CustomIterator &operator=(const CustomIterator&& other)
	{
		Current = std::move(other.Current);
		NextValue = other.NextValue;
		Iterator = std::move(other.Iterator);
	}

	CustomIterator& operator++()
	{
		NextValue = Iterator->Next(&Current);
		return *this;
	}

	CustomIterator operator++(int)
	{
		CustomIterator copy(std::make_unique<CUSTOM_TYPE>(*Iterator));
		NextValue = Iterator->Next(&Current);
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

	bool operator==(const CustomIterator& other) const
	{
		bool thisIsEndSentinel = (NextValue == ICustomIterator<value_type>::NextValueResult::NoMoreValues) || !Iterator;
		bool otherIsEndSentinel = (other.NextValue == ICustomIterator<value_type>::NextValueResult::NoMoreValues) || !other.Iterator;
		bool bothEndSentinels = (thisIsEndSentinel && otherIsEndSentinel);
		bool neitherAreEndSentinels = (!thisIsEndSentinel && !otherIsEndSentinel);
		return bothEndSentinels || (neitherAreEndSentinels && (*Iterator == *other.Iterator));
	}

	bool operator!=(const CustomIterator& other) const
	{
		return !(*this == other);
	}

private:
	ICustomIterator<value_type>::NextValueResult NextValue = ICustomIterator<value_type>::NextValueResult::NoMoreValues;
	std::shared_ptr<CUSTOM_TYPE> Iterator;
};

static_assert(std::input_or_output_iterator<CustomIterator<ICustomIteratorTest>>);

template <typename ITERATOR_TYPE, typename... ARGS>
std::ranges::subrange<CustomIterator<ITERATOR_TYPE>> CustomRange(ARGS&&... args)
{
	return std::ranges::subrange<CustomIterator<ITERATOR_TYPE>>(
		CustomIterator<ITERATOR_TYPE>{ std::make_shared<ITERATOR_TYPE>(std::forward<ARGS>(args)...) },
		CustomIterator<ITERATOR_TYPE>{}
		);
}

static_assert(std::ranges::input_range<decltype(CustomRange<ICustomIteratorTest>())>);

//////////////////////////////////////////////////////////////////////////

template <typename ID>
class MostCounter
{
public:

	void AddCount(const ID& id, int64_t count = 1);
	ID GetMostId() const;
	int64_t GetMostCount() const;

private:

	std::pair<ID, int64_t> CurrentMost = { {}, -1 };
	std::map<ID, int64_t> Counts;

};

template<typename ID>
inline void MostCounter<ID>::AddCount(const ID& id, int64_t count)
{
	int64_t newCount = (Counts[id] += count);
	if (newCount > CurrentMost.second)
	{
		CurrentMost.first = id;
		CurrentMost.second = newCount;
	}
}

template<typename ID>
inline ID MostCounter<ID>::GetMostId() const
{
	return CurrentMost.first;
}

template<typename ID>
inline int64_t MostCounter<ID>::GetMostCount() const
{
	return CurrentMost.second;
}

//////////////////////////////////////////////////////////////////////////
