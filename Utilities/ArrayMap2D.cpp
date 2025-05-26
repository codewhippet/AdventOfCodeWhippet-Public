#include "stdafx.h"

ArrayMap2D::ArrayMap2D(Point2 origin, int64_t width, int64_t height, char invalid)
{
	m_origin = origin;
	m_width = width;
	m_height = height;

	int64_t dataSize = GetDataSize();
	m_pStorage = new char[dataSize];
	memset(m_pStorage, invalid, dataSize);

	m_invalid = invalid;
}

ArrayMap2D::ArrayMap2D(const ArrayMap2D& other)
{
	m_origin = other.m_origin;
	m_width = other.m_width;
	m_height = other.m_height;

	int64_t dataSize = GetDataSize();
	m_pStorage = new char[dataSize];
	memcpy(m_pStorage, other.m_pStorage, dataSize);

	m_invalid = other.m_invalid;
}

ArrayMap2D::ArrayMap2D(ArrayMap2D&& other) noexcept
{
	m_origin = other.m_origin;
	m_width = other.m_width;
	m_height = other.m_height;

	m_pStorage = other.m_pStorage;
	other.m_pStorage = nullptr;

	m_invalid = other.m_invalid;
}

ArrayMap2D::ArrayMap2D(ArrayMap2DOptions options, const ArrayMap2D& other)
{
	m_origin = other.m_origin;
	m_width = other.m_width;
	m_height = other.m_height;

	int64_t dataSize = GetDataSize();
	m_pStorage = new char[dataSize];
	m_invalid = other.m_invalid;

	switch (options)
	{
	case ArrayMap2DOptions::CloneAsNull:
		memset(m_pStorage, 0, dataSize);
		break;
	case ArrayMap2DOptions::CloneAsInvalid:
		memset(m_pStorage, m_invalid, dataSize);
		break;

	case ArrayMap2DOptions::CloneAsUninitialised:
		break;
	}
}

ArrayMap2D::~ArrayMap2D()
{
	delete[] m_pStorage;
}

ArrayMap2D& ArrayMap2D::operator=(ArrayMap2D&& other) noexcept
{
	delete[] m_pStorage;

	m_origin = other.m_origin;
	m_width = other.m_width;
	m_height = other.m_height;

	m_pStorage = other.m_pStorage;
	other.m_pStorage = nullptr;

	m_invalid = other.m_invalid;

	return *this;
}

ArrayMap2D& ArrayMap2D::operator=(const ArrayMap2D& other)
{
	delete[] m_pStorage;

	m_origin = other.m_origin;
	m_width = other.m_width;
	m_height = other.m_height;

	int64_t dataSize = GetDataSize();
	m_pStorage = new char[dataSize];
	memcpy(m_pStorage, other.m_pStorage, dataSize);

	m_invalid = other.m_invalid;

	return *this;
}

char& ArrayMap2D::operator()(Point2 p)
{
	return this->operator()(p.X, p.Y);
}

char& ArrayMap2D::operator()(int64_t x, int64_t y)
{
	if (x < m_origin.X)
		return m_invalid;
	if (x >= (m_origin.X + m_width))
		return m_invalid;

	if (y < m_origin.Y)
		return m_invalid;
	if (y >= (m_origin.Y + m_height))
		return m_invalid;

	x = x - m_origin.X;
	y = y - m_origin.Y;

	int64_t rowStart = y * m_width;
	int64_t index = rowStart + x;

	assert(index >= 0 && index < GetDataSize());
	return m_pStorage[index];
}

const char& ArrayMap2D::operator()(Point2 p) const
{
	return this->operator()(p.X, p.Y);
}

const char& ArrayMap2D::operator()(int64_t x, int64_t y) const
{
	if (x < m_origin.X)
		return m_invalid;
	if (x >= (m_origin.X + m_width))
		return m_invalid;

	if (y < m_origin.Y)
		return m_invalid;
	if (y >= (m_origin.Y + m_height))
		return m_invalid;

	x = x - m_origin.X;
	y = y - m_origin.Y;

	int64_t rowStart = y * m_width;
	int64_t index = rowStart + x;

	assert(index >= 0 && index < GetDataSize());
	return m_pStorage[index];
}

ArrayMap2DAxis ArrayMap2D::AxisRangeX() const
{
	return ArrayMap2DAxis{ m_origin.X, m_origin.X + m_width };
}

ArrayMap2DAxis ArrayMap2D::AxisRangeY() const
{
	return ArrayMap2DAxis{ m_origin.Y, m_origin.Y + m_height };
}

Point2 ArrayMap2D::GetOrigin() const
{
	return m_origin;
}

int64_t ArrayMap2D::GetWidth() const
{
	return m_width;
}

int64_t ArrayMap2D::GetHeight() const
{
	return m_height;
}

Point2 ArrayMap2D::GetDimensions() const
{
	return { m_width, m_height };
}

bool ArrayMap2D::IsInside(Point2 p) const
{
	return
		(p.X >= m_origin.X) &&
		(p.X < (m_origin.X + m_width)) &&
		(p.Y >= m_origin.Y) &&
		(p.Y < (m_origin.Y + m_height));
}

ArrayMap2DGrid ArrayMap2D::Grid() const
{
	return ArrayMap2DGrid(this);
}

char ArrayMap2D::GetInvalidCharacter() const
{
	return m_invalid;
}

void ArrayMap2D::Replace(char from, char to)
{
	char* current = m_pStorage;
	char* end = current + GetDataSize();
	while (current != end)
	{
		if (*current == from)
		{
			*current = to;
		}
		current++;
	}
}

int64_t ArrayMap2D::Count(char value) const
{
	return std::count(m_pStorage, m_pStorage + GetDataSize(), value);
}

void ArrayMap2D::Print() const
{
	std::string s;

	int64_t size = GetDataSize();
	for (int64_t i = 0; i < size; i++)
	{
		if ((i > 0) && (i % m_width) == 0)
		{
			s += '\n';
		}
		s += m_pStorage[i];
	}

	printf("%s\n", s.c_str());
}

void ArrayMap2D::Save(const char* filename) const
{
	std::string s;

	int64_t size = GetDataSize();
	for (int64_t i = 0; i < size; i++)
	{
		if ((i > 0) && (i % m_width) == 0)
		{
			s += '\n';
		}
		s += m_pStorage[i];
	}

	FILE* f = fopen(filename, "w");
	fprintf(f, "%s\n", s.c_str());
	fclose(f);
}

std::vector<char> ArrayMap2D::GetData() const
{
	return std::vector<char>(&m_pStorage[0], &m_pStorage[GetDataSize()]);
}

int64_t ArrayMap2D::GetDataSize() const
{
	return m_width * m_height;
}

ArrayMap2DAxisIterator::ArrayMap2DAxisIterator()
	: m_current(-1)
{
}

ArrayMap2DAxisIterator::ArrayMap2DAxisIterator(int64_t current)
	: m_current(current)
{
}

ArrayMap2DAxisIterator& ArrayMap2DAxisIterator::operator--()
{
	--m_current;
	return *this;
}

ArrayMap2DAxisIterator ArrayMap2DAxisIterator::operator--(int)
{
	ArrayMap2DAxisIterator old{ m_current };
	m_current--;
	return old;
}

ArrayMap2DAxisIterator& ArrayMap2DAxisIterator::operator++()
{
	++m_current;
	return *this;
}

ArrayMap2DAxisIterator ArrayMap2DAxisIterator::operator++(int)
{
	ArrayMap2DAxisIterator old{ m_current };
	m_current++;
	return old;
}

int64_t ArrayMap2DAxisIterator::operator*() const
{
	return m_current;
}

bool operator==(const ArrayMap2DAxisIterator& a, const ArrayMap2DAxisIterator& b)
{
	return (*a == *b);
}

bool operator!=(const ArrayMap2DAxisIterator& a, const ArrayMap2DAxisIterator& b)
{
	return !(a == b);
}

ArrayMap2DAxis::ArrayMap2DAxis(int64_t begin, int64_t end)
	: m_begin(begin)
	, m_end(end)
{
}

ArrayMap2DAxisIterator ArrayMap2DAxis::begin() const
{
	return ArrayMap2DAxisIterator{ m_begin };
}

ArrayMap2DAxisIterator ArrayMap2DAxis::end() const
{
	return ArrayMap2DAxisIterator{ m_end };
}

ArrayMap2D ReadArrayMap(std::istream& input, char emptyChar)
{
	std::vector<std::string> lines = ReadAllLines(input);
	ArrayMap2D ret(Point2{ 0, 0 }, (int64_t)lines[0].size(), (int64_t)lines.size(), emptyChar);

	for (int64_t y = 0; y < (int64_t)lines.size(); y++)
	{
		for (int64_t x = 0; x < (int64_t)lines[y].size(); x++)
		{
			ret({ x, y }) = lines[y][x];
		}
	}

	return ret;
}

ArrayMap2DGridIterator::ArrayMap2DGridIterator()
	: m_arrayMap(nullptr)
	, m_current(0)
{
}

ArrayMap2DGridIterator::ArrayMap2DGridIterator(const ArrayMap2D* arrayMap)
	: m_arrayMap(arrayMap)
	, m_current(0)
{
}

ArrayMap2DGridIterator& ArrayMap2DGridIterator::operator--()
{
	assert(m_current > 0);
	--m_current;
	return *this;
}

ArrayMap2DGridIterator ArrayMap2DGridIterator::operator--(int)
{
	assert(m_current > 0);
	ArrayMap2DGridIterator old(*this);
	m_current--;
	return old;
}

ArrayMap2DGridIterator& ArrayMap2DGridIterator::operator++()
{
	assert(m_arrayMap);
	assert(m_current < m_arrayMap->GetDataSize());
	++m_current;
	return *this;
}

ArrayMap2DGridIterator ArrayMap2DGridIterator::operator++(int)
{
	assert(m_arrayMap);
	assert(m_current < m_arrayMap->GetDataSize());
	ArrayMap2DGridIterator old(*this);
	m_current++;
	return old;
}

std::pair<Point2, char> ArrayMap2DGridIterator::operator*() const
{
	assert(m_arrayMap);

	int64_t x = m_current % m_arrayMap->m_width;
	int64_t y = m_current / m_arrayMap->m_width;

	return { m_arrayMap->m_origin + Point2{ x, y }, m_arrayMap->m_pStorage[m_current] };
}

bool ArrayMap2DGridIterator::IsEnd() const
{
	return (m_arrayMap == nullptr) || (m_current == m_arrayMap->GetDataSize());
}

bool ArrayMap2DGridIterator::operator==(const ArrayMap2DGridIterator& other) const
{
	if (IsEnd() && other.IsEnd())
		return true;

	return m_arrayMap == other.m_arrayMap && m_current == other.m_current;
}

bool operator!=(const ArrayMap2DGridIterator& a, const ArrayMap2DGridIterator& b)
{
	return !(a == b);
}

ArrayMap2DGrid::ArrayMap2DGrid(const ArrayMap2D* arrayMap)
	: m_map(arrayMap)
{
}

ArrayMap2DGridIterator ArrayMap2DGrid::begin() const
{
	return ArrayMap2DGridIterator(m_map);
}

ArrayMap2DGridIterator ArrayMap2DGrid::end() const
{
	return ArrayMap2DGridIterator();
}
