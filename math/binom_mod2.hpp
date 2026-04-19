#ifndef KWM_T_MATH_BINOM_MOD2_HPP
#define KWM_T_MATH_BINOM_MOD2_HPP

#include <cstdint>

namespace kwm_t::math {

/**
 * @brief 二項係数 nCk mod 2
 *
 * Lucas の定理 (mod 2) により、
 * nCk ≡ 1 (mod 2) ⇔ (n & k) == k
 *
 * 典型用途:
 *   - パリティ（偶奇）のみ必要な場合
 *   - bit DP / XOR 系問題
 *
 * 計算量:
 *   - O(1)
 *
 * 制約 / 注意:
 *   - n, k は非負整数
 *   - k > n の場合は 0
 *
 * 使用例:
 *   int x = kwm_t::math::binom_mod2(n, k);
 *
 * verified:
 *   - Lucas theorem (mod 2)
 */
inline int binom_mod2(std::int64_t n, std::int64_t k) {
    if (k < 0 || k > n) return 0;
    return ((n & k) == k) ? 1 : 0;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_BINOM_MOD2_HPP