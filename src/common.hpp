#pragma once

#include "util.hpp"

namespace container
{
    template<is_container C, typename FN>
    void map(C& container, FN fn)
    {
        for(auto& item : container)
            item = fn(item);
    }

    template<is_container C, typename FN>
    void each(C& container, FN fn)
    {
        for(auto& item : container)
            fn(item);
    }

    template<is_container C, typename FN>
    void reduce(C& container, FN fn)
    {
        auto previous = *container.begin();

        for(auto begin = container.begin()+1; begin != container.end(); begin++)
            *begin = fn(previous, *begin);
    }

    template<is_container C>
    auto sum(const C& container)
    {
        auto total = *container.begin();

        for(auto begin = container.begin()+1; begin != container.end(); begin++)
            total += *begin;

        return total;
    }

    template<is_container C, typename FN>
    bool every(C& container, FN fn)
    {
        for(auto &item : container)
        {
            if(!fn(item))
                return false;
        }

        return true;
    }

    template<is_container C, typename T>
    bool includes(C& container, const T& item)
    {
        for(auto &i : container)
        {
            if(i == item)
                return true;
        }

        return false;
    }

    template<is_container C, typename T>
    void fill(C& container, size_t start, size_t end, const T& item)
    {
        if(start >= container.size() || end >= container.size())
            return;

        for(; start <= end; start++)
        {
            container[start] = item;
        }
    }

    template<is_container C, typename T>
    size_t index_of(C& container, const T& item, size_t offset = 0)
    {
        for(size_t i = offset; i < container.size(); i++)
        {
            if(container[i] == item)
                return i;
        }

        return -1;
    }
}

