/** @file
 *****************************************************************************
 Declaration of misc math and serialization utility functions
 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace libsnark {

typedef std::vector<bool> bit_vector;

/// returns ceil(log2(n)), so 1ul<<log2(n) is the smallest power of 2, that is not less than n
#ifdef _WIN32
uint64_t log2(uint64_t n);
#else
size_t log2(size_t n);
#endif

#ifdef _WIN32
inline uint64_t exp2(uint64_t k) { return 1ull << k; }
#else
inline size_t exp2(size_t k) { return 1ul << k; }
#endif

#ifdef _WIN32
uint64_t bitreverse(uint64_t n, const uint64_t l);
bit_vector int_list_to_bits(const std::initializer_list<uint64_t> &l, const uint64_t wordsize);
int64_t div_ceil(int64_t x, int64_t y);
#else
size_t bitreverse(size_t n, const size_t l);
bit_vector int_list_to_bits(const std::initializer_list<unsigned long> &l, const size_t wordsize);
long long div_ceil(long long x, long long y);
#endif

bool is_little_endian();

std::string FORMAT(const std::string &prefix, const char* format, ...);

/* A variadic template to suppress unused argument warnings */
template<typename ... Types>
void UNUSED(Types&&...) {}

#ifdef DEBUG
#define FMT FORMAT
#else
#define FMT(...) (UNUSED(__VA_ARGS__), "")
#endif

void serialize_bit_vector(std::ostream &out, const bit_vector &v);
void deserialize_bit_vector(std::istream &in, bit_vector &v);

template<typename T>
#ifdef _WIN32
uint64_t size_in_bits(const std::vector<T> &v);
#else
size_t size_in_bits(const std::vector<T> &v);
#endif

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

} // libsnark

#include "common/utils.tcc" /* note that utils has a templatized part (utils.tcc) and non-templatized part (utils.cpp) */
#endif // UTILS_HPP_
