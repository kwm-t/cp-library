#ifndef KWM_T_MATH_FPS_MOD_CONVOLUTION_HPP
#define KWM_T_MATH_FPS_MOD_CONVOLUTION_HPP

#include <vector>
#include <atcoder/convolution.hpp>
#include <atcoder/modint.hpp>
#include <atcoder/math.hpp>

namespace kwm_t::math::fps {

// 任意 mod での高速畳み込み
// a, b の要素は long long
// NTT可能な3つの素数 m1,m2,m3 を使い、CRTで mod に戻す
inline std::vector<long long> fast_mod_convolution(
    const std::vector<long long> &a,
    const std::vector<long long> &b,
    const long long mod = 1000000007
) {
    const long long m1 = 167772161;
    const long long m2 = 469762049;
    const long long m3 = 1224736769;

    const long long m1_inv_m2 = atcoder::pow_mod(m1, m2 - 2, m2);
    const long long m12_inv_m3 = atcoder::pow_mod(m1 * m2, m3 - 2, m3);
    const long long m12_mod = m1 * m2 % mod;

    auto x = atcoder::convolution<m1>(a, b);
    auto y = atcoder::convolution<m2>(a, b);
    auto z = atcoder::convolution<m3>(a, b);

    int n = int(a.size() + b.size() - 1);
    std::vector<long long> ret(n);

    for (int i = 0; i < n; ++i) {
        long long v1 = (y[i] - x[i]) * m1_inv_m2 % m2;
        if (v1 < 0) v1 += m2;

        long long v2 = (z[i] - (x[i] + m1 * v1) % m3) * m12_inv_m3 % m3;
        if (v2 < 0) v2 += m3;

        ret[i] = (x[i] + m1 * v1 + m12_mod * v2) % mod;
    }

    return ret;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_MOD_CONVOLUTION_HPP