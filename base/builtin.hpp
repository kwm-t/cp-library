/**
 * @file builtin.hpp
 * @brief コンパイラ組み込み機能の補助
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/base/builtin.hpp
 */
#ifndef KWM_T_BASE_BUILTIN_HPP
#define KWM_T_BASE_BUILTIN_HPP

/**
 * @brief ローカル環境用(MSC)builtin
 */
#ifdef _MSC_VER
#include <intrin.h>
#define __builtin_popcount (int)__popcnt
#define __builtin_popcountll (int)__popcnt64
inline int __builtin_ctz(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; }
inline int __builtin_ctzll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int __builtin_clz(unsigned int n) { unsigned long i; if (_BitScanReverse(&i, n)) { return 31 - i; }return 32; }
inline int __builtin_clzll(unsigned long long n) { unsigned long i; if (_BitScanReverse64(&i, n)) { return 63 - i; }return 64; }
#endif

#endif // KWM_T_BASE_BUILTIN_HPP