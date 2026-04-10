#ifndef KWM_T_MATH_ENUMERATE_QUOTIENTS_HPP
#define KWM_T_MATH_ENUMERATE_QUOTIENTS_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

namespace kwm_t::math {

/**
 * @brief floor(n / i) の値ごとに区間を列挙
 *
 * n / i が同じ値になる i の区間 [l, r) をまとめて列挙する
 *
 * 戻り値:
 *   (q, l, r) の配列（l <= i < r の間、n / i = q）
 *
 * 性質:
 *   ・区間は重複なしで [1, n] を被覆
 *   ・q は昇順（小さい順）になる
 *
 * 計算量:
 *   O(√n)
 *
 * 典型用途:
 *   ・Σ floor(n / i) の高速計算
 *   ・約数系 / 調和級数系の最適化
 *   ・Bostan-Mori や Dirichlet 畳み込みの前処理
 *
 * @tparam T 整数型（int, long long など）
 * @param n 正整数
 *
 * @return vector of (q, l, r)
 *
 * 使用例:
 *   for (auto [q, l, r] : enumerate_quotients(n)) {
 *       // i in [l, r) は全て n/i = q
 *   }
 *
 * verified:
 *  https://judge.yosupo.jp/submission/365488
 */

template <class T>
std::vector<std::tuple<T, T, T>> enumerate_quotients(T n) {
    assert(n >= 1);

    std::vector<std::tuple<T, T, T>> res;
    T l = 1;

    while (l <= n) {
        T q = n / l;
        T r = n / q + 1;
        res.emplace_back(q, l, r);
        l = r;
    }

    std::reverse(res.begin(), res.end());
    return res;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_ENUMERATE_QUOTIENTS_HPP