/**
 * @file multipoint_binomial_sum.hpp
 * @brief 多点での二項係数和
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/multipoint_binomial_sum.hpp
 */
#ifndef KWM_T_MATH_MULTIPOINT_BINOMIAL_SUM_HPP
#define KWM_T_MATH_MULTIPOINT_BINOMIAL_SUM_HPP

#include <vector>
#include <utility>

#include "modint/binom.hpp"
#include "../algorithm/mo.hpp"

namespace kwm_t::math {

/**
 * @brief 複数クエリに対して
 * \f$\sum_{k=0}^{j}\binom{i}{k}\f$
 * をまとめて計算する。
 *
 * 各クエリは (i, j) を与え、
 * \f[
 *   \sum_{k=0}^{j}\binom{i}{k}
 * \f]
 * を返す。
 *
 * Mo's Algorithm を用いてクエリ順を最適化し、
 * 各遷移を O(1) で処理する。
 *
 * 計算量:
 *   - O((N + Q)\sqrt{N})
 *
 * @tparam Mint modint 型
 * @param n i の最大値
 * @param queries (i, j) の列
 * @param binom 二項係数ライブラリ
 * @return 各クエリの答え
 *
 * 制約:
 *   - 0 <= i <= n
 *   - 0 <= j <= i
 *
 * verified:
 *   https://atcoder.jp/contests/arc224/submissions/77449219
 */
template <class Mint>
std::vector<Mint> multipoint_binomial_sum(
	int n,
	const std::vector<std::pair<int, int>>& queries,
	kwm_t::math::modint::Binom<Mint>& binom) {

	int p = 0, q = 0;

	Mint val = 1;
	const Mint inv2 = Mint(2).inv();

	std::vector<Mint> res(queries.size());

	auto add_right = [&](int) {val += binom(p, ++q); };
	auto remove_right = [&](int) {val -= binom(p, q--); };
	auto add_left = [&](int) {val = (val + binom(--p, q)) * inv2; };
	auto remove_left = [&](int) {val = val * 2 - binom(p++, q); };
	auto output = [&](int idx) {res[idx] = val; };

	kwm_t::algorithm::Mo mo(0, n + 1, queries.size());
	mo.query(queries);
	mo.run(add_left, add_right, remove_left, remove_right, output, 0, 0);
	return res;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_MULTIPOINT_BINOMIAL_SUM_HPP