/**
 * @file diophantine.hpp
 * @brief 一次不定方程式
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/diophantine.hpp
 */
#ifndef KWM_T_MATH_DIOPHANTINE_HPP
#define KWM_T_MATH_DIOPHANTINE_HPP

#include <utility>
#include <cassert>

namespace kwm_t::math {

/**
 * @brief 拡張ユークリッドの互除法
 *
 * ax + by = gcd(a,b) を満たす (x,y) を返す
 */

long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

/**
 * @brief 線形ディオファントス方程式 ax + by = c
 *
 * return:
 *   {x0, y0} 1つの解（存在しない場合 {-1,-1}）
 *
 * 注意:
 *   ・一般解は x = x0 + t*(b/g), y = y0 - t*(a/g)
 *   ・x >= 0 最小化などは別処理
 */

std::pair<long long, long long>
linear_diophantine(long long a, long long b, long long c) {
    long long x, y;
    long long g = ext_gcd(a, b, x, y);

    if (c % g != 0) return {-1, -1};

    long long mul = c / g;
    x *= mul;
    y *= mul;

    return {x, y};
}

/**
 * @brief ax ≡ c (mod b) の解（存在すれば1つ）
 *
 * return:
 *   x ≡ solution (mod b/g)
 *   なければ -1
 * verified
 *   https://atcoder.jp/contests/abc423/submissions/74801048
 */

long long mod_linear_equation(long long a, long long b, long long c) {
    long long x, y;
    long long g = ext_gcd(a, b, x, y);

    if (c % g != 0) return -1;

    long long mod = b / g;
    long long res = (x * (c / g)) % mod;

    if (res < 0) res += mod;
    return res;
}

} // namespace kwm_t::math

#endif