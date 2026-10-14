/**
 * @file faulhaber.hpp
 * @brief Faulhaber の公式
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/fps/faulhaber.hpp
 */
#ifndef KWM_T_MATH_FPS_FAULHABER_HPP
#define KWM_T_MATH_FPS_FAULHABER_HPP

#include <vector>
#include "bernoulli.hpp"      // kwm_t::math::fps::bernoulli
#include "fps.hpp"   // Factorial
#include "factorial.hpp"   // Factorial

namespace kwm_t::math::fps {

// n乗和の公式（Faulhaber公式）の母関数を返す
// k: 指数, shift: x をシフトしたい場合, fc: Factorial
template <typename mint>
std::vector<mint> faulhaber(int k, int shift, kwm_t::math::fps::Factorial<mint> &fc) {
    // ベルヌーイ数を取得 B_0 ... B_{k+1}
    auto b = bernoulli<mint>(k + 1, fc);

    std::vector<mint> f(k + 2);
    mint inv = mint(k + 1).inv();

    // 係数計算
    for (int i = 0; i <= k; ++i) {
        f[k + 1 - i] = fc.binom(k + 1, i) * b[i] * inv * ((i % 2) ? -1 : 1);
    }

    // x のシフトが必要な場合
    if (shift) {
        FormalPowerSeries<mint> fps_f(f);
        fps_f.shift_inplace(shift);
        f = fps_f;
    }

    return f;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_FAULHABER_HPP