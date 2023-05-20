#pragma once

#include <cstdint>

#include "util.hpp"

namespace dna
{
    // implementation of Euclid's GCD algorithm
    uint32_t gcd(size_t a, size_t b)
    {
        if(a < b)
        {
            size_t temp = a;
            a = b;
            b = temp;
        }

        size_t r = a % b;

        if(r == 0)
            return b;

        while(r > 0)
        {
            a = b;
            b = r;
            r = a % b;
        }

        return b;
    }
}

// Class for generating prime numbers
class Prime
{
public:

    enum class Sieve
    {
        Eratosthenes,
        Sundaram,
    };

    Prime(size_t start, size_t end, Sieve sieve)
    {
        using enum Sieve;

        switch(sieve)
        {
            case Eratosthenes: m_primes = std::move(eratosthenes(start, end)); break;
            case Sundaram: m_primes = std::move(sundaram(start, end)); break;
        }
    }

    inline size_t next()
    {
        if(m_offset >= m_primes.size())
            return npos;
        return m_primes[m_offset++];
    }

    inline bool good() const
    {
        return m_offset < m_primes.size();
    }

    inline size_t offset() const
    {
        return m_offset;
    }

    inline const std::vector<size_t>& primes() const
    {
        return m_primes;
    }

    static std::vector<size_t> sundaram(size_t start, size_t end)
    {
        size_t k = (end-1)/2;

        std::vector<bool> a;

        a.reserve(k+1);

        fill_vec(a, true, k+1);

        a[0] = false;

        auto m = [](size_t i, size_t j) { return i + j + 2 * i * j; };

        size_t target = std::sqrt(k);

        for(size_t i = 1; i <= target; i++)
        {
            size_t j = i;

            while(m(i, j) <= k)
            {
                a[m(i, j)] = false;
                j += 1;
            }
        }

        std::vector<size_t> output;

        output.reserve(a.size());

        for(size_t i = start; i < a.size(); i++)
        {
            if(a[i])
                output.push_back(i);
        }

        for(auto &item : output)
            item = 2 * item + 1;

        return output;
    }

    static std::vector<size_t> eratosthenes(size_t start, size_t end)
    {
        std::vector<bool> a;

        a.reserve(end);

        fill_vec(a, true, end);

        a[1] = false;

        size_t target = std::sqrt(end);

        for(size_t i = 2; i <= target; i++)
        {
            if(a[i])
            {
                size_t j = std::pow(i, 2);

                while(j < end)
                {
                    a[j] = false;
                    j = j+i;
                }
            }
        }

        std::vector<size_t> output;

        output.reserve(a.size());

        for(size_t i = start; i < end; i++)
        {
            if(a[i])
                output.push_back(i);
        }

        return output;
    }

private:
    std::vector<size_t> m_primes;
    std::vector<bool> m_buff;
    size_t m_offset = 0;
};