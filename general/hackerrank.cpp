#include "pch.h"
#include "general.h"
#include "hackerrank.h"

namespace Hackerrank::BitArray
{
    constexpr unsigned int mod = 1 << 31;

    int BitArray(int n, int s, int p, int q)
    {
        unsigned int current = s % mod;
        unsigned int prev = current;
        int count = 1;
        for (int i = 1; i < n; ++i)
        {
            current = (1LL * current * p + q) % mod;
            count += (current != prev) ? 1 : 0;
        }
        std::cout << count;

        return 0;
    }
}
