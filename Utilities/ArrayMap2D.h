#pragma once

#include <stdint.h>

class ArrayMap2D;

class ArrayMap2DAxisIterator
{
public:
	using value_type = int64_t;
	using difference_type = ptrdiff_t;

	ArrayMap2DAxisIterator();
	explicit ArrayMap2DAxisIterator(int64_t current);

	ArrayMap2DAxisIterator& operator--();
	ArrayMap2DAxisIterator operator--(int);

	ArrayMap2DAxisIterator& operator++();
	ArrayMap2DAxisIterator operator++(int);

	int64_t operator*() const;

private:
	int64_t m_current;
};

bool operator==(const ArrayMap2DAxisIterator& a, const ArrayMap2DAxisIterator& b);
bool operator!=(const ArrayMap2DAxisIterator& a, const ArrayMap2DAxisIterator& b);

static_assert(std::input_or_output_iterator<ArrayMap2DAxisIterator>);

class ArrayMap2DAxis
{
public:
	ArrayMap2DAxis(int64_t begin, int64_t end);

	ArrayMap2DAxisIterator begin() const;
	ArrayMap2DAxisIterator end() const;
private:
	int64_t m_begin;
	int64_t m_end;
};

static_assert(std::ranges::input_range<ArrayMap2DAxis>);

class ArrayMap2DGridIterator
{
public:
	using difference_type = std::ptrdiff_t;
	using value_type = std::pair<Point2, char>;

	ArrayMap2DGridIterator();
	ArrayMap2DGridIterator(const ArrayMap2D *arrayMap);

	ArrayMap2DGridIterator& operator--();
	ArrayMap2DGridIterator operator--(int);

	ArrayMap2DGridIterator& operator++();
	ArrayMap2DGridIterator operator++(int);

	std::pair<Point2, char> operator*() const;

	bool operator==(const ArrayMap2DGridIterator& other) const;

private:
	bool IsEnd() const;

	const ArrayMap2D* m_arrayMap;
	int64_t m_current;
};

bool operator!=(const ArrayMap2DGridIterator& a, const ArrayMap2DGridIterator& b);

static_assert(std::input_or_output_iterator<ArrayMap2DGridIterator>);

class ArrayMap2DGrid
{
public:
	ArrayMap2DGrid(const ArrayMap2D* arrayMap);

	ArrayMap2DGridIterator begin() const;
	ArrayMap2DGridIterator end() const;
private:
	const ArrayMap2D* m_map;
};

static_assert(std::ranges::input_range<ArrayMap2DGrid>);

enum class ArrayMap2DOptions
{
	CloneAsNull,
	CloneAsInvalid,
	CloneAsUninitialised
};

class ArrayMap2D
{
public:

	ArrayMap2D(Point2 origin, int64_t width, int64_t height, char invalid);
	ArrayMap2D(const ArrayMap2D& other);
	ArrayMap2D(ArrayMap2DOptions options, const ArrayMap2D& other);
	ArrayMap2D(ArrayMap2D&& other) noexcept;
	~ArrayMap2D();

	ArrayMap2D& operator=(const ArrayMap2D& other);
	ArrayMap2D& operator=(ArrayMap2D&& other) noexcept;

	char &operator()(Point2 p);
	char &operator()(int64_t x, int64_t y);

	const char& operator()(Point2 p) const;
	const char& operator()(int64_t x, int64_t y) const;

	ArrayMap2DAxis AxisRangeX() const;
	ArrayMap2DAxis AxisRangeY() const;

	Point2 GetOrigin() const;
	int64_t GetWidth() const;
	int64_t GetHeight() const;
	Point2 GetDimensions() const;

	bool IsInside(Point2 p) const;

	friend class ArrayMap2DGridIterator;
	ArrayMap2DGrid Grid() const;

	char GetInvalidCharacter() const;
	void Replace(char from, char to);

	int64_t Count(char value) const;

	void Print() const;
	void Save(const char* filename) const;

	std::vector<char> GetData() const;

private:

	int64_t GetDataSize() const;

	Point2 m_origin;
	int64_t m_width;
	int64_t m_height;

	char* m_pStorage;
	char m_invalid;
};

ArrayMap2D ReadArrayMap(std::istream& input, char emptyChar = '.');
