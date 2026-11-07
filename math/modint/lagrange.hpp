/**
 * @file lagrange.hpp
 * @brief Lagrange 補間
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/modint/lagrange.hpp
 */
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
  * verified
  *   https://atcoder.jp/contests/arc230/submissions/79418709
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

/**
 * @brief Lagrange 補間による多項式の係数復元
 *
 * y[i] = f(i) (0 <= i <= N) から
 * f(x) = c[0] + c[1]x + ... + c[N]x^N
 * の係数を復元する
 *
 * 計算量:
 *   O(N log^2 N)
 *
 * @tparam Mint modint型
 *
 * 制約 / 注意:
 *   - mod は素数
 *   - N < mod
 *   - y.size() >= 1
 *
 * @return c[i] = x^i の係数
 *
 * verified
 * https://yukicoder.me/submissions/1197581
 */
template <class Mint>
std::vector<Mint> lagrange_coefficients(const std::vector<Mint>& y) {
	int N = (int)y.size();
	if (N == 0) return {};

	std::vector<std::vector<Mint>> p(4 * N);

	auto build = [&](auto&& self, int node, int l, int r) -> void {
		if (r - l == 1) {
			p[node] = { -Mint(l), Mint(1) };
			return;
		}
		int mid = (l + r) / 2;
		self(self, node * 2, l, mid);
		self(self, node * 2 + 1, mid, r);
		p[node] = atcoder::convolution(p[node * 2], p[node * 2 + 1]);
	};
	build(build, 1, 0, N);

	std::vector<Mint> invfac(N, 1);
	Mint fac = 1;
	for (int i = 1; i < N; i++) fac *= i;
	invfac[N - 1] = fac.inv();
	for (int i = N - 1; i > 0; i--) {
		invfac[i - 1] = invfac[i] * i;
	}

	std::vector<Mint> res;

	auto combine = [&](auto&& self, int node, int l, int r) -> std::vector<Mint> {
		if (r - l == 1) {
			Mint c = y[l] * invfac[l] * invfac[N - 1 - l];
			if ((N - 1 - l) & 1) c = -c;
			return { c };
		}

		int mid = (l + r) / 2;
		std::vector<Mint> a = self(self, node * 2, l, mid);
		std::vector<Mint> b = self(self, node * 2 + 1, mid, r);

		std::vector<Mint> c1 = atcoder::convolution(a, p[node * 2 + 1]);
		std::vector<Mint> c2 = atcoder::convolution(b, p[node * 2]);

		std::vector<Mint> res(std::max(c1.size(), c2.size()), 0);
		for (int i = 0; i < (int)c1.size(); i++) res[i] += c1[i];
		for (int i = 0; i < (int)c2.size(); i++) res[i] += c2[i];
		return res;
	};

	res = combine(combine, 1, 0, N);
	res.resize(N);
	return res;
}
} // namespace kmw_t::math::modint

#endif // KMW_T_MATH_MODINT_LAGRANGE_HPP