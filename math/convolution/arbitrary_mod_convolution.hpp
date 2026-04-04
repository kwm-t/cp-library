#ifndef KWM_T_MATH_CONVOLUTION_ARBITRARY_MOD_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_ARBITRARY_MOD_CONVOLUTION_HPP

#include <vector>
#include <atcoder/convolution.hpp>
#include <atcoder/math.hpp>

/**
 * @brief 任意 mod 畳み込み（Garner + NTT）
 *
 * 3つのNTT-friendly素数で畳み込みを行い、
 * CRTで任意 mod に復元する
 *
 * 典型用途:
 *   - mod 1e9+7 の高速畳み込み
 *   - 任意 mod の多項式計算
 *
 * 計算量:
 *   O(n log n)
 *
 * @param a, b 入力列（long long）
 * @param mod 出力の mod
 *
 * 制約 / 注意:
 *   - mod < m1*m2*m3 (~1e27) ならOK
 *   - a,b は非負でなくてもOK（mod取るので）
 *
 * 使用例:
 *   auto c = arbitrary_mod_convolution(a, b, 1000000007);
 *
 * verified:
 */
namespace kwm_t::math::convolution {

    inline std::vector<long long> arbitrary_mod_convolution(
        const std::vector<long long>& a,
        const std::vector<long long>& b,
        long long mod = 1000000007
    ) {
        if (a.empty() || b.empty()) return {};

        // NTT-friendly primes
        constexpr long long m1 = 167772161;
        constexpr long long m2 = 469762049;
        constexpr long long m3 = 1224736769;

        // 前計算
        static const long long m1_inv_m2 = atcoder::pow_mod(m1, m2 - 2, m2);
        static const long long m12_inv_m3 = atcoder::pow_mod(m1 * m2 % m3, m3 - 2, m3);
        static const long long m12_mod = (m1 * m2) % mod;

        // 畳み込み
        auto x = atcoder::convolution<m1>(a, b);
        auto y = atcoder::convolution<m2>(a, b);
        auto z = atcoder::convolution<m3>(a, b);

        int n = (int)x.size();
        std::vector<long long> res(n);

        for (int i = 0; i < n; ++i) {
            long long v1 = (y[i] - x[i]) % m2;
            if (v1 < 0) v1 += m2;
            v1 = v1 * m1_inv_m2 % m2;

            long long t = (x[i] + m1 * v1) % m3;
            long long v2 = (z[i] - t) % m3;
            if (v2 < 0) v2 += m3;
            v2 = v2 * m12_inv_m3 % m3;

            res[i] = (x[i] + m1 * v1 + m12_mod * v2) % mod;
        }

        return res;
    }

}

#endif // KWM_T_MATH_CONVOLUTION_ARBITRARY_MOD_CONVOLUTION_HPP