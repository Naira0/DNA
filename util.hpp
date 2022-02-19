#pragma once

#include <string>
#include <sstream>
#include <chrono>

template<typename T>
concept is_container = requires(T t)
{
    t.begin();
    t.end();
    t.empty();
    t.size();
};

template<is_container T>
std::string to_string(const T& container)
{
    if(container.empty())
        return "[]";

    std::stringstream result;

    if(container.size() == 1)
    {
        result << "[ " << *container.begin() << " ]";
        return result.str();
    }

    result << "[ " << *container.begin() << ", ";

    // written in this way to be compatible with all iterators

    auto current = ++container.begin();
    auto end     = --container.end();

    for(; current != end; current++)
        result << *current << ", ";

    result << *current << " ]";

    return result.str();
}

std::string rand_str(size_t max_len)
{
    auto rand_range = [](size_t min, size_t max) -> size_t
    {
        return rand() % (max - min + 1) + min;
    };

    int len = rand_range(1, max_len);

    std::string output;

    output.reserve(len);

    for(size_t i = 0; i < len; i++)
        output += (char)rand_range(33, 127);

    return output;
}

template<class FN, typename... Args>
void benchmark(FN fn, Args&... a)
{
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    fn(a...);

    auto end = high_resolution_clock::now();

    std::cout << "function time: " << end-start << '\n';
}