#pragma once

#include "util.hpp"
#include "common.hpp"

class range
{
public:

    range(size_t start, size_t end, size_t step = 1) : m_start(start), m_end(end), m_step(step) {}

    class Range_iterator
    {
    private:
        size_t m_current;
        size_t m_step;

    public:

        Range_iterator(size_t current, size_t step) : m_current(current), m_step(step) {}

        size_t operator*() { return m_current; }
        size_t operator++(int) { return m_current += m_step; }
        size_t& operator++() { return m_current += m_step; }

        friend bool operator!=(const Range_iterator &a, const Range_iterator &b) { return a.m_current < b.m_current+1; }
    };

    Range_iterator begin() { return Range_iterator(m_start, m_step); };
    Range_iterator end() { return Range_iterator(m_end, m_step); }

private:
    size_t m_start;
    size_t m_end;
    size_t m_step;
};

template<is_container T>
class Range
{
public:

    Range(T container) : container(container)
    {}

    template<typename FN>
    inline Range& map(FN fn)
    {
        container::map(container, fn);
        return *this;
    }

    template<typename FN>
    inline Range& each(FN fn)
    {
        container::each(container, fn);
        return *this;
    }

    template<typename FN>
    inline Range& reduce(FN fn)
    {
        container::reduce(container, fn);
        return *this;
    }

    inline auto sum() const
    {
       return container::sum(container);
    }

private:
    T container;
};

