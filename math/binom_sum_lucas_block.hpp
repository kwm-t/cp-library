/**
 * @file binom_sum_lucas_block.hpp
 * @brief Lucas の定理を利用した二項係数和
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/binom_sum_lucas_block.hpp
 */
#ifndef KWM_T_MATH_BINOM_SUM_LUCAS_BLOCK_HPP
#define KWM_T_MATH_BINOM_SUM_LUCAS_BLOCK_HPP

#include <vector>
#include <cstdint>

#include "math/lucas.hpp"

namespace kwm_t::math {

/**
 * @brief 二項係数の累積和（Lucas + p進分割）
 *
 * prefix(r) = Σ_{k=0}^{r-1} C(n,k) mod p
 *
 * mod は素数
 * n が大きくても扱える
 */
class BinomPrefixSumLucas {
public:
    BinomPrefixSumLucas(std::int64_t n, int mod)
        : n(n), mod(mod), lucas(mod) {
        build();
    }

    int prefix(int r) const {
        int ret = v0sum[r / d] * v1sum[d]
                + v0[r / d] * v1sum[r % d];
        return ret % mod;
    }

    int range(int l, int r) const {
        return (prefix(r) - prefix(l) + mod) % mod;
    }

private:
    std::int64_t n;
    int mod;
    int d;

    std::vector<int> v0, v1;
    std::vector<int> v0sum, v1sum;

    Lucas lucas;

    void build() {
        d = 1;
        while (d <= n / d) d *= mod;

        v0.resize(d);
        v1.resize(d);
        v0sum.assign(d + 1, 0);
        v1sum.assign(d + 1, 0);

        for (int i = 0; i < d; ++i) {
            v0[i] = lucas.comb(n / d, i);
            v1[i] = lucas.comb(n % d, i);

            v0sum[i + 1] = (v0sum[i] + v0[i]) % mod;
            v1sum[i + 1] = (v1sum[i] + v1[i]) % mod;
        }
    }
};

} // namespace kwm_t::math

#endif