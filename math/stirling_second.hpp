/**
 * @file stirling_second.hpp
 * @brief 第二種 Stirling 数
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/stirling_second.hpp
 */
#ifndef KWM_T_MATH_STIRLING_SECOND_HPP
#define KWM_T_MATH_STIRLING_SECOND_HPP

#include <vector>

/**
 * @brief 第二種スターリング数 S(n, k)
 *
 * S(n, k) = k個の非空集合への分割数
 *
 * 典型用途:
 *   - 集合分割
 *   - ベル数計算
 *
 * verified:
 */
namespace kwm_t::math {

/**
 * @brief 単体計算（包除原理）
 *
 * S(n, k) = 1/k! * Σ (-1)^{k-i} C(k,i) i^n
 *
 * 計算量:
 *   O(k log n)
 *
 * 前提:
 *   - Binom<Mint> が渡される
 */
template <class Mint, class Binom>
Mint stirling_second(int n, int k, const Binom& C) {
    if (n < k) return Mint(0);

    Mint res = 0;
    for (int i = 0; i <= k; ++i) {
        Mint term = C.com(k, i) * Mint(i).pow(n);
        if ((k - i) & 1) res -= term;
        else res += term;
    }
    return res * C.ifact[k];
}

/**
 * @brief テーブル計算（DP）
 *
 * S(n, k) = S(n-1, k-1) + k * S(n-1, k)
 *
 * 計算量:
 *   O(n^2)
 */
template <class Mint>
std::vector<std::vector<Mint>> stirling_second_table(int n) {
    std::vector<std::vector<Mint>> s(n + 1, std::vector<Mint>(n + 1));

    s[0][0] = 1;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            s[i][j] = s[i - 1][j - 1] + Mint(j) * s[i - 1][j];
        }
    }

    return s;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_STIRLING_SECOND_HPP