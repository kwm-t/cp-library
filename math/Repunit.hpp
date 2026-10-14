/**
 * @file repunit.hpp
 * @brief レピュニット
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/repunit.hpp
 */
#ifndef KWM_T_MATH_REPUNIT_HPP
#define KWM_T_MATH_REPUNIT_HPP

namespace kwm_t::math {

// n 桁のレピュニット数を返す
// 例: n=1 → 1, n=2 → 11, n=3 → 111
//
// mod P 上で「9 の逆元が存在しない場合」に有効
// 通常は (10^n - 1) / 9 を使うが、それが使えないケース用
//
// T は +, * が定義されている型（modint などを想定）
template <typename T>
inline T repunit(long long n) {
    T result = 0;     // 累積結果
    T base = 1;       // 現在の 10^k
    T block = 1;      // 長さ 2^k の repunit
    T pow10 = 10;     // 10^(2^k)

    while (n > 0) {
        if (n & 1) {
            result += base * block;
            base *= pow10;
        }
        block += block * pow10;  // repunit doubling
        pow10 *= pow10;          // 10^(2^(k+1))
        n >>= 1;
    }

    return result;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_REPUNIT_HPP