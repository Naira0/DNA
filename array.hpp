#pragma once

#include <initializer_list>
#include <cassert>
#include <exception>

template<class T, size_t arr_size>
class Array
{
public:

	Array(std::initializer_list<T> elements)
		: m_capacity(arr_size),
		m_size(elements.size())
	{
		static_assert(arr_size > 0, "Array size must be greater then zero");

		for (int i = 0; const auto& e : elements)
		{
			m_elements[i] = e;
			i++;
		}
	}

	size_t size() const { return m_size; };

	T at(size_t index) const { return get_element(index); }
	T operator[](size_t index) const { return get_element(index); }

	T& at(size_t index) { return get_element(index); }
	T& operator[](size_t index) { return get_element(index); }

	T front() const { return m_elements[0]; }
	T back()  const { return m_elements[m_size - 1]; }

	T* begin() { return m_elements; }
	T* end()   { return &m_elements[m_size]; }

	T* data() noexcept { return m_elements; }

	bool empty() const { return m_elements == &m_elements[m_size];  }

	size_t max_size() const { return m_capacity; }

private:
	T	   m_elements[arr_size];
	size_t m_size;
	size_t m_capacity;

	inline T get_element(size_t index) const
	{
		if (index >= m_capacity)
			throw std::out_of_range("Provided index is out of bounds");
		return m_elements[index];
	}

	inline T& get_element(size_t index) 
	{
		if (index >= m_capacity)
			throw std::out_of_range("Provided index is out of bounds");
		return m_elements[index];
	}
};