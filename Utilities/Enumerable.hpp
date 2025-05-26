//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#include "Enumerable.h"

//////////////////////////////////////////////////////////////////////////

template <typename T>
std::vector<T> IEnumerator<T>::ToVector()
{
	std::vector<T> out;

	m_source->Reset();
	while (m_source->MoveNext())
	{
		T next;
		m_source->GetCurrent(&next);
		out.emplace_back(std::move(next));
	}

	return out;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
std::set<T> IEnumerator<T>::ToSet()
{
	std::set<T> out;

	m_source->Reset();
	while (m_source->MoveNext())
	{
		T next;
		m_source->GetCurrent(&next);
		out.emplace(std::move(next));
	}

	return out;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename KEY, typename VALUE>
std::map<KEY, VALUE> IEnumerator<T>::ToMap()
{
	std::map<KEY, VALUE> out;

	m_source->Reset();
	while (m_source->MoveNext())
	{
		T next;
		m_source->GetCurrent(&next);
		out.insert(std::move(next));
	}

	return out;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<IEnumerator<T>> IEnumerator<T>::Where(const std::function<bool(const T&)> &predicate)
{
	return std::make_shared<IEnumerator<T>>(std::make_shared<Enumerator_Where<T>>(predicate, this->shared_from_this()));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename OUT_TYPE>
std::shared_ptr<IEnumerator<OUT_TYPE>> IEnumerator<T>::Select(const std::function<OUT_TYPE(const T&)>& transform)
{
	return std::make_shared<IEnumerator<OUT_TYPE>>(std::make_shared<Enumerator_Select<OUT_TYPE, T>>(transform, this->shared_from_this()));
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename OUT_TYPE>
inline std::shared_ptr<IEnumerator<OUT_TYPE>> IEnumerator<T>::Convert()
{
	return std::make_shared<IEnumerator<OUT_TYPE>>(std::make_shared<Enumerator_Convert<OUT_TYPE, T>>(this->shared_from_this()));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
int64_t IEnumerator<T>::Count()
{
	int64_t count = 0;

	m_source->Reset();
	while (m_source->MoveNext())
	{
		count++;
	}

	return count;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
T IEnumerator<T>::Min()
{
	m_source->Reset();
	m_source->MoveNext();
	T min;
	m_source->GetCurrent(&min);
	while (m_source->MoveNext())
	{
		T v;
		m_source->GetCurrent(&v);
		if (v < min)
			min = v;
	}
	return min;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
T IEnumerator<T>::Max()
{
	m_source->Reset();
	m_source->MoveNext();
	T max;
	m_source->GetCurrent(&max);
	while (m_source->MoveNext())
	{
		T v;
		m_source->GetCurrent(&v);
		if (max < v)
			max = v;
	}
	return max;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
T IEnumerator<T>::First()
{
	m_source->Reset();
	m_source->MoveNext();
	T first;
	m_source->GetCurrent(&first);
	return first;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
T IEnumerator<T>::Sum()
{
	m_source->Reset();

	T sum{ 0 };
	while (m_source->MoveNext())
	{
		T v;
		m_source->GetCurrent(&v);
		sum += v;
	}

	return sum;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
T IEnumerator<T>::Product()
{
	m_source->Reset();

	T product{ 1 };
	while (m_source->MoveNext())
	{
		T v;
		m_source->GetCurrent(&v);
		product *= v;
	}

	return product;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<IEnumerator<T>> IEnumerator<T>::Distinct()
{
	return std::make_shared<IEnumerator<T>>(std::make_shared<Enumerator_Distinct<T>>(this->shared_from_this()));
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
void IEnumerator<T>::Execute()
{
	m_source->Reset();
	while (m_source->MoveNext())
	{
		T next;
		m_source->GetCurrent(&next);
	}
}

//////////////////////////////////////////////////////////////////////////
