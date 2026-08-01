#ifndef KWM_T_MATH_MODINT_MINT_UTIL_HPP
#define KWM_T_MATH_MODINT_MINT_UTIL_HPP

#include <vector>

namespace kwm_t::math::modint {

/**
 * @brief C(n, i) (0 <= i <= k) の配列を返す。
 *
 * n が大きく、k が小さい場合に適している。
 * k! の逆元を一度だけ計算して、二項係数を求める。
 *
 * 計算量:
 *   O(k + log mod)
 *
 * @tparam Mint 使用する modint 型。
 * @tparam T    n の整数型。
 *
 * @param n 二項係数の上側。
 * @param k   計算する最大の下側。
 *
 * @return
 *   res[i] = C(n, i) (0 <= i <= k)
 *
 * 制約 / 注意:
 *   - 0 <= k <= n を想定する。
 *   - Mint は inv() を持つ型を想定する。
 *
 * verified
 *   - https://atcoder.jp/contests/codequeen2026-final-Public/submissions/77990610
 */
template <class Mint, class T>
std::vector<Mint> binomial_row(T n, int k) {
	std::vector<Mint> res(k + 1);
	res[0] = 1;

	for (int i = 1; i <= k; ++i) {
		res[i] = res[i - 1] * (n - i + 1);
	}

	Mint fact = 1;
	for (int i = 1; i <= k; ++i) {
		fact *= i;
	}

	Mint ifact = fact.inv();
	for (int i = k; i >= 1; --i) {
		res[i] *= ifact;
		ifact *= i;
	}

	return res;
}

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_MINT_UTIL_HPP