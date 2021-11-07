#pragma once

#include <iterator>
#include <cstddef>

template<class T>
class Iterator
{
public:
	using category	 = std::bidirectional_iterator_tag;
	using diff_type	 = std::ptrdiff_t;
	using value_type = T;
	using ref		 = T&;
	using pointer	 = T*;

	Iterator(pointer ptr) : m_ptr(ptr) {};

	ref operator*() const { return *m_ptr; }

	pointer operator->() const { return m_ptr; }

	Iterator& operator++() { m_ptr++; return *this; }
	
	Iterator operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	Iterator& operator--() { m_ptr--; return *this; }

	Iterator operator--(T)
	{
		Iterator temp = *this;
		--(*this);
		return temp;
	}

	Iterator& operator+=(diff_type& value) { this->m_ptr += value; return *this; }
	Iterator& operator-=(diff_type& value) { this->m_ptr -= value; return *this; }

	friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }
	friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }

private:
	pointer m_ptr;
};

