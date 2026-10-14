/**
 * @file linear_geometric_sum.hpp
 * @brief 線形・等比数列の和
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/linear_geometric_sum.hpp
 */
#ifndef KWM_T_MATH_LINEAR_GEOMETRIC_SUM_HPP
#define KWM_T_MATH_LINEAR_GEOMETRIC_SUM_HPP

#include <atcoder/modint>

/**
 * @brief 線形関数 × 幾何級数の和
 *
 * Σ_{i=0}^{n-1} (a*i + b) * c^i を計算する
 *
 * 分解:
 *   Σ (a*i + b)c^i = a Σ i c^i + b Σ c^i
 *
 * 典型用途:
 *   ・等差 × 等比 の和
 *   ・DPの高速化
 *   ・期待値計算
 *
 * 計算量:
 *   O(log n) （pow使用）
 *
 * @tparam mint atcoder::modint を想定
 *
 * @param n 項数 (n >= 0)
 * @param a 一次係数
 * @param b 定数項
 * @param c 公比
 *
 * @return Σ_{i=0}^{n-1} (a*i + b) * c^i
 *
 * 制約 / 注意:
 *   ・c == 1 のときは特別処理
 *   ・mint は四則演算 / pow が使えること
 *
 * 使用例:
 *   using mint = atcoder::modint998244353;
 *   mint res = linear_geometric_sum<mint>(n, a, b, c);
 *
 * verified:
 *   ・基本ケース
 *   ・c = 0, 1
 */
namespace kwm_t::math {
    template<typename mint>
    mint linear_geometric_sum(long long n, mint a, mint b, mint c) {
        if (n == 0) return mint(0);

        if (c == mint(1)) {
            // Σ(ai + b) = a * n(n-1)/2 + b * n
            mint N = n;
            return a * N * (N - 1) / 2 + b * N;
        }

        mint one = mint(1);
        mint cn = c.pow(n);

        // S = Σ c^i
        mint S = (one - cn) / (one - c);

        // T = Σ i c^i
        mint denom = (one - c);
        mint T = (c - mint(n) * cn + mint(n - 1) * cn * c) / (denom * denom);

        return a * T + b * S;
    }

} // namespace kwm_t::math

#endif // KWM_T_MATH_LINEAR_GEOMETRIC_SUM_HPP