#ifndef KWM_T_MATH_CAYLEY_AND_MATCHING_HPP
#define KWM_T_MATH_CAYLEY_AND_MATCHING_HPP

#include <vector>

namespace kwm_t::math {

/**
 * @brief Cayley の定理：ラベル付き木の個数
 *
 * 頂点数 n のラベル付き木の個数は n^(n-2)
 *
 * 典型用途:
 *   - 木の個数カウント
 *   - Prüfer 列との対応
 *
 * 計算量:
 *   - O(log n)（pow）
 *
 * 制約 / 注意:
 *   - n <= 1 のとき 1
 *
 * verified:
 *   - Cayley theorem
 */
template <typename mint>
mint cayley_theorem(int n) {
    if (n <= 1) return mint(1);
    return mint(n).pow(n - 2);
}

/**
 * @brief 完全マッチングを含むラベル付き木の個数
 *
 * 頂点数 n のラベル付き木のうち、
 * 完全マッチングを持つものの個数を数える。
 *
 * 導出:
 *   1. 完全マッチングを選ぶ → (n-1)!!
 *   2. 各ペアを縮約 → 頂点数 n/2 の木
 *      → Cayley により (n/2)^(n/2-2)
 *   3. 縮約を戻す自由度
 *      → 各辺で 2×2 の選択 → (2^2)^(n/2-1)
 *
 *   よって
 *     (n-1)!! * (n/2)^(n/2-2) * 4^(n/2-1)
 *   = n^(n/2-2) * n! / (n/2)!
 *
 * 典型用途:
 *   - 特殊制約付き木の数え上げ
 *   - matching + tree の合成問題
 *
 * 計算量:
 *   - O(log n)
 *
 * 制約 / 注意:
 *   - n は偶数
 *   - fact, ifact が必要
 *
 * verified:
 *   - combinatorics
 */
template <typename mint>
mint perfect_matching_tree(
    int n,
    const std::vector<mint>& fact,
    const std::vector<mint>& ifact
) {
    if (n % 2 != 0) return mint(0);
    if (n <= 2) return mint(1);

    // n^(n/2 - 2) * n! / (n/2)!
    return mint(n).pow(n / 2 - 2) * fact[n] * ifact[n / 2];
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_CAYLEY_AND_MATCHING_HPP