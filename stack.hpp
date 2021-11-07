#pragma once

#include "forward_list.hpp"

template<typename T>
class Stack
{
public:

    Stack() = default;

    Stack(std::initializer_list<T> init_list) : m_list(init_list), m_size(init_list.size()) {}

    void push(T item)
    {
        m_list.push_back(item);
        m_size++;
    }

    void pop()
    {
        m_list.pop_back();
        m_size--;
    }

    bool empty() const
    {
        return m_size == 0;
    }

    T peek()
    {
        return m_list.back();
    }

    T view(size_t index)
    {
        return m_list[index];
    }

    size_t size() const
    {
        return m_size;
    }

private:
    size_t m_size;
    Forward_List<T> m_list;
};