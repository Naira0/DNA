#pragma once

#include "util.hpp"

constexpr auto descending = [](auto a, auto b) { return a < b; };
constexpr auto ascending  = [](auto a, auto b) { return a > b; };

template<is_container C, typename FN>
void selection_sort(C& container, FN fn)
{
    for(auto begin = container.begin(); begin != container.end(); begin++)
    {
        auto smallest = begin;

        for(auto current = begin + 1; current != container.end(); current++)
        {
            if(fn(*current, *smallest))
                smallest = current;
        }

        swap(*begin, *smallest);
    }
}

template<is_container C, typename FN>
void insertion_sort(C& container, FN fn)
{
    if(container.size() == 1)
        return;

    for(auto current = container.begin()+1; current != container.end(); current++)
    {
        auto key = *current;
        auto start = current-1;

        while(start+1 != container.begin() && fn(key, *start))
        {
            *(start+1) = std::move(*start);
            --start;
        }

        *(start+1) = std::move(key);
    }
}

