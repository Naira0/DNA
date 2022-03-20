#pragma once

#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <cstdarg>

template<typename T>
concept is_container = requires(T t)
{
    t.begin();
    t.end();
    t.empty();
    t.size();
};

static constexpr size_t npos = -1;

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

    auto current = container.begin()+1;

    for(; current != container.end()-1; current++)
        result << *current << ", ";

    result << *current << " ]";

    return result.str();
}

template<typename... A>
std::string format(std::string_view fmt, const A&... a)
{
    if(sizeof...(a) == 0)
        return fmt.data();

    std::string output;

    output.reserve(fmt.size() * 2);

    std::stringstream values;

    ((values << a << '\0'), ...);

    std::string buffer = values.rdbuf()->str();
    size_t offset = 0;

    for(size_t i = 0; i < fmt.size(); i++)
    {
        if(fmt[i] == '{')
        {
            if(fmt[++i] == '{')
                goto concat;

            for(char c = buffer[offset++]; c; c = buffer[offset++])
            {
                output += c;
            }

            while(i < fmt.size() && fmt[i] != '}')
                i++;
            continue;
        }

        concat:
        output += fmt[i];
    }

    return output;
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

template<typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<class FN, typename... Args>
size_t benchmark(FN fn, Args&... a)
{
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    fn(a...);

    auto end = high_resolution_clock::now();

    return (end-start).count();
}

void fill_vec_random(std::vector<size_t>& vec, size_t target)
{
    vec.reserve(target);

    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<size_t> dist(1, target);

    for(size_t i = 0; i < target; i++)
        vec.push_back(dist(gen));
}

template<typename T>
void fill_vec(std::vector<T> &vec, T value, size_t count)
{
    for(size_t i = 0; i < count; i++)
        vec.push_back(value);
}

