/** @file
 *****************************************************************************

 Implementation of interfaces for wNAF ("weighted Non-Adjacent Form") exponentiation routines.

 See wnaf.hpp .

 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef WNAF_TCC_
#define WNAF_TCC_

namespace libsnark {

template<mp_size_t n>
#ifdef _WIN32
std::vector<int64_t> find_wnaf(const size_t window_size, const bigint<n> &scalar)
#else
std::vector<long> find_wnaf(const size_t window_size, const bigint<n> &scalar)
#endif
{
    const size_t length = scalar.max_bits(); // upper bound
#ifdef _WIN32
    std::vector<int64_t> res(length+1);
#else
    std::vector<long> res(length+1);
#endif
    bigint<n> c = scalar;
#ifdef _WIN32
    int64_t j = 0;
#else
    long j = 0;
#endif
    while (!c.is_zero())
    {
#ifdef _WIN32
        int64_t u;
#else
        long u;
#endif
        if ((c.data[0] & 1) == 1)
        {
            u = c.data[0] % (1u << (window_size+1));
            if (u > (1 << window_size))
            {
                u = u - (1 << (window_size+1));
            }

            if (u > 0)
            {
                mpn_sub_1(c.data, c.data, n, u);
            }
            else
            {
                mpn_add_1(c.data, c.data, n, -u);
            }
        }
        else
        {
            u = 0;
        }
        res[j] = u;
        ++j;

        mpn_rshift(c.data, c.data, n, 1); // c = c/2
    }

    return res;
}

template<typename T, mp_size_t n>
T fixed_window_wnaf_exp(const size_t window_size, const T &base, const bigint<n> &scalar)
{
#ifdef _WIN32
    std::vector<int64_t> naf = find_wnaf(window_size, scalar);
    std::vector<T> table(UINT64_C(1)<<(window_size-1));
#else
    std::vector<long> naf = find_wnaf(window_size, scalar);
    std::vector<T> table(1ul<<(window_size-1));
#endif
    T tmp = base;
    T dbl = base.dbl();
#ifdef _WIN32
    for (size_t i = 0; i < UINT64_C(1)<<(window_size-1); ++i)
#else
    for (size_t i = 0; i < 1ul<<(window_size-1); ++i)
#endif
    {
        table[i] = tmp;
        tmp = tmp + dbl;
    }

    T res = T::zero();
    bool found_nonzero = false;
#ifdef _WIN32
    for (int64_t i = naf.size()-1; i >= 0; --i)
#else
    for (long i = naf.size()-1; i >= 0; --i)
#endif
    {
        if (found_nonzero)
        {
            res = res.dbl();
        }

        if (naf[i] != 0)
        {
            found_nonzero = true;
            if (naf[i] > 0)
            {
                res = res + table[naf[i]/2];
            }
            else
            {
                res = res - table[(-naf[i])/2];
            }
        }
    }

    return res;
}

template<typename T, mp_size_t n>
T opt_window_wnaf_exp(const T &base, const bigint<n> &scalar, const size_t scalar_bits)
{
    size_t best = 0;
#ifdef _WIN32
    for (int64_t i = T::wnaf_window_table.size() - 1; i >= 0; --i)
#else
    for (long i = T::wnaf_window_table.size() - 1; i >= 0; --i)
#endif
    {
        if (scalar_bits >= T::wnaf_window_table[i])
        {
            best = i+1;
            break;
        }
    }

    if (best > 0)
    {
        return fixed_window_wnaf_exp(best, base, scalar);
    }
    else
    {
        return scalar * base;
    }
}

} // libsnark

#endif // WNAF_TCC_