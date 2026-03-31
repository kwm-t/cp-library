#ifndef KWM_T_MATH_EULER_PHI_HPP
#define KWM_T_MATH_EULER_PHI_HPP

#include <vector>

/**
 * @brief オイラーのφ関数テーブル
 *
 * 0 <= i <= n に対して φ(i) を前計算する
 *
 * 典型用途:
 *   - gcd系問題
 *   - 約数・倍数の数え上げ
 *   - 数論前計算
 *
 * 計算量:
 *   O(n log log n)
 *
 * @param n 上限
 * @return std::vector<int> phi
 *
 * 制約 / 注意:
 *   - phi[0] = 0
 *   - phi[1] = 1
 *   - φ(n) ≤ n のため int で十分
 *
 * 使用例:
 *   auto phi = kwm_t::math::euler_phi_table(10);
 *   // phi = {0,1,1,2,2,4,2,6,4,6,4}
 *
 * verified:
 *   - 典型数論問題
 */
namespace kwm_t::math {

std::vector<int> euler_phi_table(int n) {
    std::vector<int> phi(n + 1);
    for (int i = 0; i <= n; ++i) phi[i] = i;

    for (int i = 2; i <= n; ++i) {
        if (phi[i] == i) { // i が素数
            for (int j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
    return phi;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_EULER_PHI_HPP