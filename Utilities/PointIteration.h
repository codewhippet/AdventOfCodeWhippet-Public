#pragma once
#include "Point2.h"

//////////////////////////////////////////////////////////////////////////

struct LineIterator
{
	using value_type = Point2;
	using difference_type = ptrdiff_t;

	Point2 Begin;
	Point2 End;
	Point2 Current;

	Point2 Direction;
	
	LineIterator& operator++()
	{
		NextLinePoint();
		return *this;
	}

	LineIterator operator++(int)
	{
		LineIterator copy(*this);
		NextLinePoint();
		return copy;
	}

	const Point2& operator*() const
	{
		return Current;
	}

	const Point2* operator->() const
	{
		return &Current;
	}

private:

	void NextLinePoint()
	{
		assert(Current != End);
		Current = Current + Direction;
	}
};

inline bool operator==(const LineIterator& a, const LineIterator& b)
{
	return a.Current == b.Current;
}

inline bool operator!=(const LineIterator& a, const LineIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<LineIterator>);

struct LineInclusiveRange
{
	Point2 First;
	Point2 Last;

	LineInclusiveRange(const Point2& first, const Point2& last)
		: First(first)
		, Last(last)
	{
	}

	LineIterator begin() const
	{
		Point2 direction = GetDirection();
		return { First, Last + direction, First, direction};
	}

	LineIterator end() const
	{
		Point2 direction = GetDirection();
		return { First, Last + direction, Last + direction, direction };
	}

private:
	Point2 GetDirection() const
	{
		if (First.X == Last.X)
		{
			return (First.Y < Last.Y) ? Point2::Down() : Point2::Up();
		}
		else
		{
			return (First.X < Last.X) ? Point2::Right() : Point2::Left();
		}
	}
};

static_assert(std::ranges::input_range<LineInclusiveRange>);

//////////////////////////////////////////////////////////////////////////

struct GridIterator
{
	using value_type = Point2;
	using difference_type = ptrdiff_t;

	Point2 Begin;
	Point2 End;
	Point2 Current;

	GridIterator& operator++()
	{
		NextGridPoint();
		return *this;
	}

	GridIterator operator++(int)
	{
		GridIterator copy(*this);
		NextGridPoint();
		return copy;
	}

	const Point2& operator*() const
	{
		return Current;
	}

	const Point2* operator->() const
	{
		return &Current;
	}

private:

	void NextGridPoint()
	{
		assert(Current != End);
		if (++Current.X == End.X)
		{
			Current.X = Begin.X;
			Current.Y++;
		}
	}
};

inline bool operator==(const GridIterator& a, const GridIterator& b)
{
	return a.Current == b.Current;
}

inline bool operator!=(const GridIterator& a, const GridIterator& b)
{
	return !(a == b);
}

static_assert(std::input_or_output_iterator<GridIterator>);

struct GridRange
{
	Point2 Begin;
	Point2 End;

	GridRange(const Point2& begin, const Point2& end)
		: Begin(begin)
		, End(end)
	{
	}

	GridIterator begin() const
	{
		return { Begin, End, Begin };
	}

	GridIterator end() const
	{
		return { Begin, End, { Begin.X, End.Y } };
	}
};

static_assert(std::ranges::input_range<GridRange>);

//////////////////////////////////////////////////////////////////////////
