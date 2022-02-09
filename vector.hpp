#pragma once

#include <initializer_list>
#include <stdexcept>
#include <utility>

template<typename T>
class Vector
{
public:

    Vector()
    {
        m_size     = 0;
        m_capacity = 10;
        m_data     = new T[m_capacity];
    }

    Vector(std::initializer_list<T> items)
    {
        m_size     = 0;
        m_capacity = items.size() * 2;
        m_data     = new T[m_capacity];

        for(auto &item : items)
            m_data[m_size++] = std::move(item);
    }

    Vector(Vector&& other) noexcept
    {
        m_data     = other.m_data;
        m_size     = other.m_size;
        m_capacity = other.m_capacity;
    }

    Vector(const Vector& other)
    {
        m_size     = other.m_size;
        m_capacity = other.m_capacity;

        m_data = new T[m_capacity];

        for(size_t i = 0; i < m_size; i++)
            m_data[i] = other.m_data[i];
    }

    ~Vector()
    {
        delete[] m_data;
    }

    void resize(size_t amount)
    {
        m_capacity = amount;

        T *temp = new T[m_capacity];

        for(size_t i = 0; i < m_size; i++)
            temp[i] = m_data[i];

        delete[] m_data;
        m_data = temp;
    }

    inline void reserve(size_t amount)
    {
        if(amount <= m_capacity)
            return;
        resize(amount);
    }

    inline void shrink_to_fit()
    {
        resize(m_size);
    }

    void clear()
    {
        m_size = 0;

        delete[] m_data;

        m_data = new T[m_capacity];
    }

    template<typename... A>
    void push_back(T item, A ...a)
    {
        size_t added_size = sizeof...(a) + 1;

        if(m_size + added_size >= m_capacity)
            resize((m_capacity * 2) + added_size);

        m_data[m_size++] = item;
        ((m_data[m_size++] = a), ...);
    }

    void pop_back()
    {
        if(empty())
            return;
        m_data[--m_size].~T();
    }

    template<typename... A>
    void emplace_back(A ...a)
    {
        if(m_size + 1 >= m_capacity)
            resize(m_capacity * 2);
        m_data[m_size++] = std::move(T(a...));
    }

    void insert(size_t index, T item)
    {
        if(index > m_size)
            return;
        if(m_size+1 >= m_capacity)
            resize(m_capacity * 2);

        size_t i;
        for(i = m_size; i > index; i--)
        {
            m_data[i] = m_data[i-1];
        }

        m_data[i] = item;

        m_size++;
    }

    void erase(size_t index)
    {
        if(index >= m_size)
            return;

        m_data[index].~T();

        for(size_t i = index; i < m_size; i++)
        {
            m_data[i] = m_data[i+1];
        }

        m_size--;
    }

    T& operator[](size_t index) const { return get_element(index); }

    T* begin() const { return m_data; }

    T* end() const { return &m_data[m_size]; }

    T& front() const { return get_element(0); }
    T& back() const { return get_element(m_size-1); }

    [[nodiscard]]
    inline size_t size() const { return m_size; }

    [[nodiscard]]
    inline size_t capacity() const { return m_capacity; }

    [[nodiscard]]
    inline bool empty() const { return m_size == 0; }

    [[nodiscard]]
    inline T *data() const { return m_data; }

private:
    T      *m_data;
    size_t  m_size;
    size_t  m_capacity;

    inline T& get_element(size_t index) const
    {
        if(index >= m_size)
            throw std::out_of_range("Index is out of range");
        return m_data[index];
    }
};