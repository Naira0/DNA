#pragma once

#include "forward_list.hpp"

// a lazy queue implementation
template<class T>
class Queue
{
public:

	Queue() = default;

	Queue(std::initializer_list<T> init_list) : m_list(init_list), m_size(init_list.size()) {}

	void enqueue(T item)
	{
	    m_list.push_back(item);
	    m_size++;
	}

	void dequeue()
	{
        m_list.pop();
        m_size--;
	}

	bool empty() const
    {
	    return m_size == 0;
    }

    size_t size() const
    {
	    return m_size;
    }

    T peek()
    {
	    return m_list.back();
    }

    T view(size_t index)
    {
	    return m_list[index];
    }

private:
	size_t m_size;
	Forward_List<T> m_list;
};