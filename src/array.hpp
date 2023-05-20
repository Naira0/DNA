#pragma once

#include <initializer_list>
#include <cassert>
#include <stdexcept>

template<class T, size_t N>
class Array
{
public:

	Array(std::initializer_list<T> items)
	{
        size_t i = 0;

        for(auto &item : items)
        {
            m_data[i++] = std::move(item);
        }
	}

    Array()
    {
        for (int i = 0; i < N; i++)
        {
            m_data[i++] = T();
        }
    }

    [[nodiscard]]
    inline constexpr
	T& at(size_t index)
    {
        return get_element(index);
    }

    [[nodiscard]]
    inline constexpr
	T& operator[](size_t index)
    {
        return get_element(index);
    }

    [[nodiscard]]
    inline constexpr
	T& front()
    {
        return m_data[0];
    }

    [[nodiscard]]
    inline constexpr
	T& back()
    {
        return m_data[N - 1];
    }

    [[nodiscard]]
    inline constexpr
    T* begin()
    {
        return m_data;
    }

    [[nodiscard]]
    inline constexpr
    T* end()
    {
        return &m_data[N];
    }

    [[nodiscard]]
    inline constexpr
	T* data()
    {
        return m_data;
    }

    [[nodiscard]]
    inline constexpr
	bool empty() const
    {
        return N == 0;
    }

    [[nodiscard]]
    inline constexpr
    size_t size() const
    {
        return N;
    };

private:
	T m_data[N];

    inline constexpr
    T& get_element(size_t index)
	{
		if (index >= N)
        {
			throw std::out_of_range("Provided index is out of bounds");
        }
		return m_data[index];
	}
};