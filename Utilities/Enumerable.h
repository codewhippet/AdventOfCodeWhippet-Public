//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#include "Point2.h"
#include "Vector3.h"

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <optional>
#include <string>
#include <regex>

//////////////////////////////////////////////////////////////////////////

template <typename>
struct is_pair : std::false_type {};

template <typename T1, typename T2>
struct is_pair<std::pair<T1, T2>> : std::true_type {};

//////////////////////////////////////////////////////////////////////////

template <typename T>
class IEnumerator : public std::enable_shared_from_this<IEnumerator<T>>
{
public:

	IEnumerator()
		: m_source(nullptr)
	{
	}

	IEnumerator(std::shared_ptr<IEnumerator> source)
		: m_source(source)
	{
	}

	virtual ~IEnumerator() = default;

	virtual bool MoveNext()
	{
		return m_source->MoveNext();
	}

	virtual void Reset()
	{
		m_source->Reset();
	}

	virtual bool GetCurrent(T* value)
	{
		return m_source->GetCurrent(value);
	}

	std::shared_ptr<IEnumerator<T>> Where(const std::function<bool(const T&)> &predicate);

	template <typename OUT_TYPE>
	std::shared_ptr<IEnumerator<OUT_TYPE>> Select(const std::function<OUT_TYPE(const T&)>& transform);

	template <typename OUT_TYPE>
	std::shared_ptr<IEnumerator<OUT_TYPE>> Convert();

	std::shared_ptr<IEnumerator<T>> Distinct();

	int64_t Count();
	T Min();
	T Max();
	T Sum();
	T Product();

	T First();

	std::vector<T> ToVector();
	std::set<T> ToSet();

	template <typename KEY, typename VALUE>
	std::map<KEY, VALUE> ToMap();

	void Execute();

protected:
	std::shared_ptr<IEnumerator<T>> m_source;
};

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Vector : public IEnumerator<T>
{
public:

	Enumerator_Vector(const std::vector<T>& source)
		: m_vectorSource(source)
		, m_currentPos(-1)
	{
	}

	virtual bool MoveNext() override
	{
		return (++m_currentPos < static_cast<int64_t>(m_vectorSource.size()));
	}

	virtual void Reset() override
	{
		m_currentPos = -1;
	}

	virtual bool GetCurrent(T* value) override
	{
		if (m_currentPos == -1)
			return false;

		if (m_currentPos < static_cast<int64_t>(m_vectorSource.size()))
		{
			*value = m_vectorSource[m_currentPos];
			return true;
		}

		return false;
	}

private:
	const std::vector<T>& m_vectorSource;
	int64_t m_currentPos;
};

template <typename T>
inline std::shared_ptr<IEnumerator<T>> MakeEnumerator(const std::vector<T>& source)
{
	return std::make_shared<IEnumerator<T>>(std::make_shared<Enumerator_Vector<T>>(source));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_String : public IEnumerator<T>
{
public:

	Enumerator_String(const std::basic_string<T>& source)
		: m_stringSource(source)
		, m_currentPos(-1)
	{
	}

	virtual bool MoveNext() override
	{
		return (++m_currentPos < static_cast<int64_t>(m_stringSource.size()));
	}

	virtual void Reset() override
	{
		m_currentPos = -1;
	}

	virtual bool GetCurrent(T* value) override
	{
		if (m_currentPos == -1)
			return false;

		if (m_currentPos < static_cast<int64_t>(m_stringSource.size()))
		{
			*value = m_stringSource[m_currentPos];
			return true;
		}

		return false;
	}

private:
	const std::basic_string<T>& m_stringSource;
	int64_t m_currentPos;
};

template <typename T>
inline std::shared_ptr<IEnumerator<T>> MakeEnumerator(const std::basic_string<T>& source)
{
	return std::make_shared<IEnumerator<T>>(std::make_shared<Enumerator_String<T>>(source));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Set : public IEnumerator<T>
{
public:

	Enumerator_Set(const std::set<T>& source)
		: m_setSource(source)
		, m_current()
	{
	}

	virtual bool MoveNext() override
	{
		typename std::set<T>::const_iterator next;
		if (m_current.has_value())
		{
			next = m_current.value();
			if (next != m_setSource.end())
			{
				++next;
			}
		}
		else
		{
			next = m_setSource.begin();
		}
		m_current = next;
		return (next != m_setSource.end());
	}

	virtual void Reset() override
	{
		m_current.reset();
	}

	virtual bool GetCurrent(T* value) override
	{
		if (m_current.has_value() == false)
			return false;

		typename std::set<T>::const_iterator current = m_current.value();
		if (current != m_setSource.end())
		{
			*value = *current;
			return true;
		}

		return false;
	}

private:
	const std::set<T>& m_setSource;
	std::optional<typename std::set<T>::const_iterator> m_current;
};

template <typename T>
inline std::shared_ptr<IEnumerator<T>> MakeEnumerator(const std::set<T>& source)
{
	return std::make_shared<IEnumerator<T>>(std::make_shared<Enumerator_Set<T>>(source));
}

//////////////////////////////////////////////////////////////////////////

template <typename KEY, typename VALUE>
class Enumerator_Map : public IEnumerator<typename std::pair<KEY, VALUE>>
{
public:

	Enumerator_Map(const std::map<KEY, VALUE>& source)
		: m_mapSource(source)
		, m_current()
	{
	}

	virtual bool MoveNext() override
	{
		typename std::map<KEY, VALUE>::const_iterator next;
		if (m_current.has_value())
		{
			next = m_current.value();
			if (next != m_mapSource.end())
			{
				++next;
			}
		}
		else
		{
			next = m_mapSource.begin();
		}
		m_current = next;
		return (next != m_mapSource.end());
	}

	virtual void Reset() override
	{
		m_current.reset();
	}

	virtual bool GetCurrent(std::pair<KEY, VALUE>* value) override
	{
		if (m_current.has_value() == false)
			return false;

		typename std::map<KEY, VALUE>::const_iterator current = m_current.value();
		if (current != m_mapSource.end())
		{
			*value = *current;
			return true;
		}

		return false;
	}

private:
	const std::map<KEY, VALUE>& m_mapSource;
	std::optional<typename std::map<KEY, VALUE>::const_iterator> m_current;
};

template <typename KEY, typename VALUE>
inline std::shared_ptr<IEnumerator<std::pair<KEY, VALUE>>> MakeEnumerator(const std::map<KEY, VALUE>& source)
{
	return std::make_shared<IEnumerator<std::pair<KEY, VALUE>>>(std::make_shared<Enumerator_Map<KEY, VALUE>>(source));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Range : public IEnumerator<T>
{
public:

	Enumerator_Range(int64_t start, int64_t count)
		: m_start(start)
		, m_end(start + count)
		, m_current(start - 1)
	{
	}

	virtual bool MoveNext() override
	{
		return (++m_current < m_end);
	}

	virtual void Reset() override
	{
		m_current = m_start - 1;
	}

	virtual bool GetCurrent(T* value) override
	{
		if (m_current < m_start || m_current >= m_end)
			return false;

		*value = (T)m_current;
		return true;
	}

private:
	int64_t m_start;
	int64_t m_end;
	int64_t m_current;
};

namespace Enumerable
{
	inline std::shared_ptr<IEnumerator<int>> Range(int start, int count)
	{
		return std::make_shared<IEnumerator<int>>(std::make_shared<Enumerator_Range<int>>(start, count));
	}

	inline std::shared_ptr<IEnumerator<int64_t>> Range(int64_t start, int64_t count)
	{
		return std::make_shared<IEnumerator<int64_t>>(std::make_shared<Enumerator_Range<int64_t>>(start, count));
	}
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Line : public IEnumerator<T>
{
public:

	Enumerator_Line(const Point2& first, const Point2& last)
		: m_diff({ 0, 0 })
	{
		if (first.X != last.X)
		{
			m_diff.X = (first.X < last.X) ? 1 : -1;
		}
		if (first.Y != last.Y)
		{
			m_diff.Y = (first.Y < last.Y) ? 1 : -1;
		}

		m_start = first - m_diff;
		m_stop = last + m_diff;

		m_current = m_start;
	}

	virtual bool MoveNext() override
	{
		m_current = m_current + m_diff;
		return (m_current != m_stop);
	}

	virtual void Reset() override
	{
		m_current = m_start;
	}

	virtual bool GetCurrent(T* value) override
	{
		if ((m_current == m_start) || (m_current == m_stop))
			return false;

		*value = m_current;
		return true;
	}

private:
	Point2 m_start;
	Point2 m_stop;
	Point2 m_diff;
	Point2 m_current;
};

namespace Enumerable
{
	inline std::shared_ptr<IEnumerator<Point2>> Line(const Point2 &first, const Point2 &last)
	{
		return std::make_shared<IEnumerator<Point2>>(std::make_shared<Enumerator_Line<Point2>>(first, last));
	}
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Line3D : public IEnumerator<T>
{
public:

	Enumerator_Line3D(const Vector3& first, const Vector3& last)
		: m_diff({ 0, 0, 0 })
	{
		if (first.X != last.X)
		{
			m_diff.X = (first.X < last.X) ? 1 : -1;
		}
		if (first.Y != last.Y)
		{
			m_diff.Y = (first.Y < last.Y) ? 1 : -1;
		}
		if (first.Z != last.Z)
		{
			m_diff.Z = (first.Z < last.Z) ? 1 : -1;
		}

		m_start = first - m_diff;
		m_stop = last + m_diff;

		m_current = m_start;
	}

	virtual bool MoveNext() override
	{
		m_current = m_current + m_diff;
		return (m_current != m_stop);
	}

	virtual void Reset() override
	{
		m_current = m_start;
	}

	virtual bool GetCurrent(T* value) override
	{
		if ((m_current == m_start) || (m_current == m_stop))
			return false;

		*value = m_current;
		return true;
	}

private:
	Vector3 m_start;
	Vector3 m_stop;
	Vector3 m_diff;
	Vector3 m_current;
};

namespace Enumerable
{
	inline std::shared_ptr<IEnumerator<Vector3>> Line3D(const Vector3& first, const Vector3& last)
	{
		return std::make_shared<IEnumerator<Vector3>>(std::make_shared<Enumerator_Line3D<Vector3>>(first, last));
	}
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Where : public IEnumerator<T>
{
public:

	Enumerator_Where(const std::function<bool(const T&)>& predicate, std::shared_ptr<IEnumerator<T>> source)
		: IEnumerator<T>(source)
		, m_predicate(predicate)
	{
	}

	virtual bool MoveNext() override
	{
		while (true)
		{
			if (this->m_source->MoveNext() == false)
			{
				return false;
			}

			T candidate;
			this->m_source->GetCurrent(&candidate);
			if (m_predicate(candidate))
			{
				return true;
			}
		}
	}

private:

	std::function<bool(const T&)> m_predicate;
};

//////////////////////////////////////////////////////////////////////////

template <typename T, typename IN_TYPE>
class Enumerator_Select : public IEnumerator<T>
{
public:

	Enumerator_Select(const std::function<T(const IN_TYPE&)>& transform, std::shared_ptr<IEnumerator<IN_TYPE>> source)
		: IEnumerator<T>(nullptr)
		, m_wrappedSource(source)
		, m_transform(transform)
	{
	}

	virtual bool MoveNext() override
	{
		return m_wrappedSource->MoveNext();
	}

	virtual void Reset() override
	{
		m_wrappedSource->Reset();
	}

	virtual bool GetCurrent(T* value) override
	{
		IN_TYPE original;
		if (m_wrappedSource->GetCurrent(&original))
		{
			*value = m_transform(original);
			return true;
		}
		return false;
	}

private:

	std::shared_ptr<IEnumerator<IN_TYPE>> m_wrappedSource;
	std::function<T(const IN_TYPE&)> m_transform;
};

//////////////////////////////////////////////////////////////////////////

template <typename T, typename IN_TYPE>
class Enumerator_Convert : public IEnumerator<T>
{
public:
	Enumerator_Convert(std::shared_ptr<IEnumerator<IN_TYPE>> source)
		: IEnumerator<T>(nullptr)
		, m_wrappedSource(source)
	{
	}

	virtual bool MoveNext() override
	{
		return m_wrappedSource->MoveNext();
	}

	virtual void Reset() override
	{
		m_wrappedSource->Reset();
	}

	virtual bool GetCurrent(T* value) override;

private:

	std::shared_ptr<IEnumerator<IN_TYPE>> m_wrappedSource;
};

//////////////////////////////////////////////////////////////////////////

template <typename T>
class Enumerator_Distinct : public IEnumerator<T>
{
public:

	Enumerator_Distinct(std::shared_ptr<IEnumerator<T>> source)
		: IEnumerator<T>(source)
	{
	}

	virtual bool MoveNext() override
	{
		while (true)
		{
			if (this->m_source->MoveNext() == false)
			{
				return false;
			}

			T candidate;
			this->m_source->GetCurrent(&candidate);
			if (m_uniqueElements.contains(candidate) == false)
			{
				m_uniqueElements.insert(candidate);
				return true;
			}
		}
	}

	virtual void Reset() override
	{
		this->m_source->Reset();
		m_uniqueElements.clear();
	}

private:
	std::set<T> m_uniqueElements;
};

//////////////////////////////////////////////////////////////////////////

class Enumerator_Regex : public IEnumerator<std::smatch>
{
public:

	Enumerator_Regex(const std::string &source, const std::regex &pattern)
		: m_stringSource(source)
		, m_pattern(pattern)
	{
	}

	virtual bool MoveNext() override
	{
		std::sregex_iterator next;
		if (m_current.has_value())
		{
			next = m_current.value();
			if (next != std::sregex_iterator{})
			{
				++next;
			}
		}
		else
		{
			next = std::regex_iterator(m_stringSource.begin(), m_stringSource.end(), m_pattern);
		}
		m_current = next;
		return (next != std::sregex_iterator{});
	}

	virtual void Reset() override
	{
		m_current.reset();
	}

	virtual bool GetCurrent(std::smatch* value) override
	{
		if (m_current.has_value() == false)
			return false;

		std::sregex_iterator current = m_current.value();
		if (current != std::sregex_iterator{})
		{
			*value = *current;
			return true;
		}

		return false;
	}

private:

	const std::string &m_stringSource;
	const std::regex &m_pattern;
	std::optional<std::sregex_iterator> m_current;
};

namespace Enumerable
{
	inline std::shared_ptr<IEnumerator<std::smatch>> Regex(const std::string& source, const std::regex& pattern)
	{
		return std::make_shared<IEnumerator<std::smatch>>(std::make_shared<Enumerator_Regex>(source, pattern));
	}
}

//////////////////////////////////////////////////////////////////////////

class Enumerator_Token : public IEnumerator<std::string>
{
public:
	Enumerator_Token(const std::string& source, const std::string& delimiters)
		: m_tokenSource(source)
		, m_delimiters(delimiters)
		, m_currentToken(nullptr)
	{
	}

	virtual bool MoveNext() override
	{
		m_currentToken = strtok(m_currentToken.has_value() ? nullptr : m_tokenSourceCopy.data(), m_delimiters.c_str());
		return m_currentToken.value() != nullptr;
	}

	virtual void Reset() override
	{
		m_tokenSourceCopy = m_tokenSource;
		m_currentToken.reset();
	}

	virtual bool GetCurrent(std::string* value) override
	{
		if ((m_currentToken.has_value() == false) ||
			(m_currentToken.value() == nullptr))
		{
			return false;
		}

		*value = m_currentToken.value();
		return true;
	}

private:
	const std::string& m_tokenSource;
	const std::string& m_delimiters;

	std::string m_tokenSourceCopy;
	std::optional<char*> m_currentToken;
};

namespace Enumerable
{
	inline std::shared_ptr<IEnumerator<std::string>> Tokens(const std::string& source, const std::string& delimiters)
	{
		return std::make_shared<IEnumerator<std::string>>(std::make_shared<Enumerator_Token>(source, delimiters));
	}
}

//////////////////////////////////////////////////////////////////////////

#include "Enumerable.hpp"
#include "Enumerable_Cast.hpp"

//////////////////////////////////////////////////////////////////////////
// EOF
