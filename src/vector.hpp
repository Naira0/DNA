#pragma once

#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "util.hpp"
#include "common.hpp"

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

    Vector(Vector<T>&& other) noexcept
    {
        move_from(std::move(other));
    }

    Vector(const Vector<T>& other)
    {
        copy_from(other);
    }

    ~Vector()
    {
        delete[] m_data;
    }

    template<is_container C>
    static Vector<T>&& from(C& container)
    {
        Vector<T> output;

        output.reserve(container.size()+1);

        for(size_t i = 0; i < container.size(); i++)
            output.push_back(std::move(container[i]));

        return std::move(output);
    }

    void resize(size_t amount)
    {
        m_capacity = amount;

        T *temp = new T[m_capacity];

        for(size_t i = 0; i < m_size; i++)
            temp[i] = std::move(m_data[i]);

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
        constexpr size_t added_size = sizeof...(a) + 1;

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

    // swaps the index of a and b
    inline void swap(size_t a, size_t b)
    {
        if(a >= m_size || b >= m_size)
            return;

        auto temp = m_data[a];
        m_data[a] = m_data[b];
        m_data[b] = temp;
    }

    // swaps the desired index to the last index and pops it
    void swap_pop(size_t index)
    {
        if(index >= m_size)
            return;

        swap(index, m_size-1);
        pop_back();
    }

    template<typename... A>
    void emplace_back(A ...a)
    {
        if(m_size + 1 >= m_capacity)
            resize(m_capacity * 2);
        m_data[m_size++] = std::move(T(a...));
    }

    void insert(size_t index, const T& item)
    {
        if(index > m_size)
            return;
        if(m_size+1 >= m_capacity)
            resize(m_capacity * 2);

        size_t i;
        for(i = m_size; i > index; i--)
            m_data[i] = std::move(m_data[i-1]);

        m_data[i] = std::move(item);

        m_size++;
    }

    void erase(size_t index)
    {
        if(index >= m_size)
            return;

        m_data[index].~T();

        for(size_t i = index; i < m_size; i++)
        {
            m_data[i] = std::move(m_data[i+1]);
        }

        m_size--;
    }

    // takes a filter function for every item if it returns false that item will be filtered
    template<typename FN>
    void filter(FN fn)
    {
        Vector<T> store;
        store.resize(m_capacity);

        for(size_t i = 0; i < m_size; i++)
        {
            if(fn(m_data[i]))
                store.push_back(std::move(m_data[i]));
        }

        move_from(std::move(store));
    }

    Vector<T> slice(size_t start, size_t end)
    {
        Vector<T> output;

        output.reserve(end-start+1);

        for(; start < end; start++)
            output.push_back(m_data[start]);

        return std::move(output);
    }

    // returns true if every item in the vector satisfies the provided function
    template<typename FN>
    inline bool every(FN fn) const
    {
        return container::every(*this, fn);
    }

    inline bool includes(const T& item) const
    {
        return container::includes(*this, item);
    }

    inline void fill(size_t start, size_t end, const T& item)
    {
        container::fill(*this, start, end, item);
    }

    // returns Vector::npos if item does not exit
    size_t index_of(const T& item, size_t offset = 0) const
    {
        return container::index_of(*this, item, offset);
    }

    template<typename FN>
    void map(FN fn)
    {
        container::map(*this, fn);
    }

    template<typename FN>
    void each(FN fn) const
    {
        container::each(*this, fn);
    }

    template<typename FN>
    void reduce(FN fn)
    {
        container::reduce(*this, fn);
    }

    T sum() const
    {
        return container::sum(*this);
    }

    friend Vector<T> operator+(Vector<T>& a, Vector<T>& b)
    {
        a.resize(a.size()+b.size()+1);

        for(size_t i = 0; i < b.size(); i++)
            a.push_back(std::move(b[i]));

        return std::move(a);
    }

    Vector<T>& operator=(const Vector<T>& other)
    {
        copy_from(other);
        return *this;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept
    {
        move_from(std::move(other));
        return *this;
    }

    Vector& operator+=(Vector<T>& other)
    {
        reserve(m_size+other.size()+1);

        for(size_t i = 0; i < other.size(); i++)
            push_back(std::move(other[i]));

        return *this;
    }

    friend bool operator==(const Vector<T>& a, const Vector<T>& b)
    {
        if(a.size() != b.size())
            return false;

        return is_equal(a, b);
    }

    friend bool operator!=(const Vector<T>& a, const Vector<T>& b)
    {
        if(a.size() != b.size())
            return false;

        return !is_equal(a, b);
    }

    T& operator[](size_t index) { return get_element(index); }

    const T& operator[](size_t index) const { return get_element(index); }

    friend std::ostream& operator<<(std::ostream& os, const Vector& vec)
    {
        return os << to_string(vec);
    }

    [[nodiscard]]
    T* begin() const { return m_data; }

    [[nodiscard]]
    T* end() const { return &m_data[m_size]; }

    [[nodiscard]]
    T& front() const { return get_element(0); }

    [[nodiscard]]
    T& back() const { return get_element(m_size-1); }

    [[nodiscard]]
    inline size_t size() const { return m_size; }

    [[nodiscard]]
    inline size_t capacity() const { return m_capacity; }

    [[nodiscard]]
    inline bool empty() const { return m_size == 0; }

    [[nodiscard]]
    inline T *data() const { return m_data; }

    static size_t npos;

private:
    T      *m_data;
    size_t  m_size;
    size_t  m_capacity;

    inline T& get_element(size_t index) const
    {
        if(index >= m_size && index != npos)
            throw std::out_of_range("Index is out of range");
        return index == npos ? m_data[m_size-1] : m_data[index];
    }

    static inline bool is_equal(const Vector<T>& a, const Vector<T>& b)
    {
        for(size_t i = 0; i < a.size(); i++)
        {
            if(a[i] != b[i])
                return false;
        }

        return true;
    }

    inline void copy_from(const Vector<T>& vec)
    {
        m_size     = vec.m_size;
        m_capacity = vec.m_capacity;

        m_data = new T[m_capacity];

        for(size_t i = 0; i < m_size; i++)
            m_data[i] = vec.m_data[i];
    }

    inline void move_from(Vector<T>&& vec)
    {
        m_data     = vec.m_data;
        m_size     = vec.m_size;
        m_capacity = vec.m_capacity;

        vec.m_size = 0;
        vec.m_capacity = 0;
        vec.m_data = nullptr;
    }
};

template<typename T>
size_t Vector<T>::npos = -1;

void vector_bench(const size_t target)
{
    using namespace std::chrono;

    Vector<size_t> vec;

    vec.reserve(target+1);

    auto start = system_clock::now();

    for(size_t i = 0; i < target; i++)
        vec.push_back(i);

    auto end = system_clock::now();

    std::cout
            << "Time: "     << duration_cast<milliseconds>(end-start) << '\n'
            << "Capacity: " << vec.capacity() << '\n';
}