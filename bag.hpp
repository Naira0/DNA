#pragma once

#include <cstdlib>
#include <cassert>

#include <iterator>
#include <initializer_list>

#include "iterators.hpp"

template<class T>
class Bag
{
public:

	Bag(std::initializer_list<T> items) 
	{
		m_size			 = items.size();
		m_capacity		 = m_size * 2;

		m_items = new T[m_capacity];

		int i = 0;
		for (const T& item : items)
		{
			m_items[i] = item;
			i++;
		}
	}

	Bag()
	{
		m_size		= 0;
		m_capacity	= 10;
		m_items		= new T[m_capacity];
	}

	~Bag()
	{
		delete[] m_items;
	}

	Bag& add(T item)
	{
		if (m_size >= m_capacity)
		{
			m_capacity *= 2;
			T* temp		= new T[m_capacity];

			for (int i = 0; i <= m_size; i++)
				temp[i] = m_items[i];

			delete[] m_items;

			m_items = temp;
		}

		m_items[m_size] = item;
		m_size++;

		return *this;
	}

	Bag& reserve(size_t new_capacity)
	{
		new_capacity *= 2;
		T* temp		  = new T[new_capacity];
		m_capacity    = new_capacity;

		for (int i = 0; i <= m_size; i++)
			temp[i] = m_items[i];

		delete[] m_items;
		m_items = temp;

		return *this;
	}

	bool has(T item) const
	{
		for (int i = 0; i <= m_size; i++)
		{
			if (m_items[i] == item)
				return true;
		}

		return false;
	}

	bool isEmpty() const { return m_size == 0; }

	size_t size() const { return m_size + 1; }

	T at(size_t index) const { return get_item(index); }

	T operator[](size_t index) const { return get_item(index); }
	
	Iterator<T> begin() const { return Iterator(m_items); }
	Iterator<T> end()   const { return Iterator(&m_items[m_size]); }

private:
	size_t m_size;
	size_t m_capacity;
	T*	   m_items;

	inline T get_item(size_t index) const
	{
		assert(index <= m_size && "Out of bounds index");
		return m_items[index];
	}
};

int bag_test()
{
	Bag<int> bag{ 20, 89, 50 };

	bag
		.reserve(10)
		.add(5)
		.add(25)
		.add(9)
		.add(35)
		.add(68);

	std::reverse(bag.begin(), bag.end());

	for (const auto item : bag)
	{
		std::cout << item << '\n';
	}

	return 0;
}