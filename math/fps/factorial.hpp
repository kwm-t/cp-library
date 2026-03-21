#ifndef KWM_T_MATH_FPS_FACTORIAL_HPP
#define KWM_T_MATH_FPS_FACTORIAL_HPP

#include <vector>

namespace kwm_t::math::fps {

/**
 * @brief 階乗・逆階乗テーブル
 *
 * fac[i]  = i!
 * finv[i] = (i!)^{-1}
 *
 * @tparam mint modint型
 */
template<typename mint>
struct Factorial {
    int n;
    std::vector<mint> fac, finv;

    explicit Factorial(int n = 0) : n(n), fac(n + 1, 1), finv(n + 1, 1) {
        if (n == 0) return;
        for (int i = 2; i <= n; ++i) fac[i] = fac[i - 1] * i;
        finv[n] = fac[n].inv();
        for (int i = n; i >= 1; --i) finv[i - 1] = finv[i] * i;
    }

    // nCk
    mint comb(int n, int k) const {
        if (k < 0 || n < k) return 0;
        return fac[n] * finv[k] * finv[n - k];
    }

    // nPk
    mint perm(int n, int k) const {
        if (k < 0 || n < k) return 0;
        return fac[n] * finv[n - k];
    }
};

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_FACTORIAL_HPP