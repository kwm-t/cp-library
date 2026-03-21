#ifndef KWM_T_MATH_FPS_BERNOULLI_HPP
#define KWM_T_MATH_FPS_BERNOULLI_HPP

#include <vector>
#include "factorial.hpp"          // Factorial

namespace kwm_t::math::fps {

// B_0 から B_n までのベルヌーイ数を返す
template <typename mint>
std::vector<mint> bernoulli(int n, kwm_t::math::fps::Factorial<mint> &fc) {
    n++; // B_0 から B_n まで
    FormalPowerSeries<mint> f(fc.finv.begin() + 1, fc.finv.end());
    f = f.inv();
    for (int i = 0; i < n; ++i) f[i] *= fc.fac[i];
    return f;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_BERNOULLI_HPP