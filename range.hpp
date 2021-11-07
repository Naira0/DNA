#pragma once

template<typename T>
class Range
{
public:

    Range(T start, T end, T step = 1) : m_start(start), m_end(end), m_step(step) {}

    class Range_iterator
    {
    private:
        T m_current;
        T m_step;

    public:

        Range_iterator(T current, T step) : m_current(current), m_step(step) {}

        T operator*() { return m_current; }
        T operator++(int) { return m_current += m_step; }
        T& operator++() { return m_current += m_step; }

        friend bool operator!=(const Range_iterator &a, const Range_iterator &b) { return a.m_current < b.m_current+1; }
    };

    Range_iterator begin() { return Range_iterator(m_start, m_step); };
    Range_iterator end() { return Range_iterator(m_end, m_step); }

private:
    T m_start;
    T m_end;
    T m_step;
};

void range_test()
{
    for(auto i : Range(0, 525, 5))
        std::cout << i << '\n';
}