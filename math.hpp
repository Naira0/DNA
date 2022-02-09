#pragma once

#include <cstdint>

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
