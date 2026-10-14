/**
 * @file mobius_table.hpp
 * @brief メビウス関数の表
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/mobius_table.hpp
 */
#ifndef KWM_T_MATH_MOBIUS_HPP
#define KWM_T_MATH_MOBIUS_HPP

#include <vector>

namespace kwm_t::math {

/**
 * @brief 1 以上 n 以下の Möbius 関数の値を求める
 *
 * μ(1) = 1
 * μ(n) = 0              : n が平方因子を持つ
 * μ(n) = (-1)^k         : 相異なる素因数を k 個持つ
 *
 * 計算量:
 *   O(n log log n)
 *
 * 空間計算量:
 *   O(n)
 *
 * @param n
 *   Möbius 関数を求める最大値
 *
 * 制約 / 注意:
 *   - 戻り値の i 番目が μ(i)
 *   - μ(0) は 0
 *
 * 使用例:
 *   auto mu = kwm_t::math::mobius_table(n);
 *
 * verified:
 *   https://atcoder.jp/contests/awc0150/submissions/78988183
 */
std::vector<int> mobius_table(int n) {
	std::vector<int> mu(n + 1, 1);
	std::vector<bool> composite(n + 1);

	mu[0] = 0;

	for (int p = 2; p <= n; ++p) {
		if (composite[p]) continue;

		for (int i = p; i <= n; i += p) {
			composite[i] = true;
			mu[i] *= -1;
		}

		for (long long i = 1LL * p * p; i <= n; i += 1LL * p * p) {
			mu[i] = 0;
		}
	}

	return mu;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_MOBIUS_HPP