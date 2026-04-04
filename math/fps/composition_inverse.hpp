#ifndef KWM_T_MATH_FPS_COMPOSITION_INVERSE_HPP
#define KWM_T_MATH_FPS_COMPOSITION_INVERSE_HPP

#include <vector>
#include <cassert>

#include "fps.hpp"
#include "power_projection.hpp"

namespace kwm_t::math::fps {

/**
 * @brief 形式的冪級数の合成逆（ラグランジュ反転）
 *
 * f(g(x)) = x を満たす g(x) を求める
 *
 * @tparam T modint など（T::mod() が必要）
 *
 * @param f f(x) = a1 x + a2 x^2 + ... （定数項は 0）
 * @param len 求める長さ
 * @return g(x)
 *
 * @pre
 *  - f[0] == 0
 *  - f[1] != 0
 *
 * @note
 *  power_projection と FPS(log/exp) に依存
 *
 * @complexity
 *  O(n log n)
 * 
 * @verified
 *  https://judge.yosupo.jp/submission/363891
 */
template <class T>
std::vector<T> composition_inverse(std::vector<T> f, int len = -1) {
    if (len == -1) len = (int)f.size();
    if (len == 0) return {};
    if (len == 1) return { T(0) };

    assert((int)f.size() >= 2);
    assert(f[0] == T(0));
    assert(f[1] != T(0));

    // --- 正規化: f[1] = 1 にする ---
    T inv_a1 = T(1) / f[1];
    for (auto &x : f) x *= inv_a1;

    f.resize(len);

    // --- 逆数テーブル ---
    std::vector<T> inv(len + 1, T(1));
    for (int i = 2; i <= len; i++) {
        inv[i] = -inv[T::mod() % i] * (T::mod() / i);
    }

    // --- power projection ---
    std::vector<T> p(len);
    p[0] = 1;
    p = power_projection(p, f, len);

    // --- g の構築 ---
    std::vector<T> g(len - 1);
    for (int i = 1; i < len; i++) {
        g[len - 1 - i] = p[i] * T(len - 1) * inv[i];
    }

    g = FormalPowerSeries<T>(g).log();

    T coef = -inv[len - 1];
    for (auto &x : g) x *= coef;

    g = FormalPowerSeries<T>(g).exp();

    g.insert(g.begin(), T(0));

    // --- スケール戻し ---
    T scale = 1;
    for (auto &x : g) {
        x *= scale;
        scale *= inv_a1;
    }

    return g;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_COMPOSITION_INVERSE_HPP