#ifndef KMW_T_MATH_MODINT_LAGRANGE_HPP
#define KMW_T_MATH_MODINT_LAGRANGE_HPP

#include <vector>

/**
 * @brief ラグランジュ補間（0,1,...,Nでの値から f(x) を求める）
 *
 * 典型用途:
 *   f(0), f(1), ..., f(N) が分かっているときに f(x) を求める
 *
 * 計算量:
 *   O(N)
 *
 * @tparam Mint modint型
 *
 * 制約 / 注意:
 *   - mod は素数
 *   - x は long long
 *   - y.size() >= 1
 */

namespace kmw_t::math::modint {

	template <class Mint>
	Mint lagrange_interpolation(const std::vector<Mint>& y, long long x) {
		int N = (int)y.size() - 1;
		if (x <= N) return y[x];

		std::vector<Mint> invfac(N + 1, 1);
		Mint fac = 1;
		for (int i = 1; i <= N; i++) fac *= i;

		invfac[N] = fac.inv();
		for (int i = N; i > 0; i--) {
			invfac[i - 1] = invfac[i] * i;
		}

		std::vector<Mint> dp(N + 1, 1), pd(N + 1, 1);
		for (int i = 0; i < N; i++) {
			dp[i + 1] = dp[i] * (x - i);
		}
		for (int i = N; i > 0; i--) {
			pd[i - 1] = pd[i] * (x - i);
		}

		Mint res = 0;
		for (int i = 0; i <= N; i++) {
			Mint term = y[i] * dp[i] * pd[i] * invfac[i] * invfac[N - i];
			if ((N - i) & 1) res -= term;
			else res += term;
		}

		return res;
	}

} // namespace kmw_t::math::modint

#endif // KMW_T_MATH_MODINT_LAGRANGE_HPP