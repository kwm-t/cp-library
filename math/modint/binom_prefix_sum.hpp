#ifndef KWM_T_MATH_MODINT_BINOM_PREFIX_SUM_HPP
#define KWM_T_MATH_MODINT_BINOM_PREFIX_SUM_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

#include "../../algorithm/mo.hpp"
#include "binom.hpp"

namespace kwm_t::math::modint {

/**
 * @brief 二項係数の prefix sum を Mo's Algorithm でまとめて求める
 *
 * 各クエリ (x, y) に対して
 *
 *   sum_{i=0}^{y-1} C(x, i)
 *
 * を求める。
 *
 * 計算量:
 *   O((N + Q) sqrt(N))
 *
 * @tparam Mint modint 型
 * @tparam Binom 二項係数を計算できる型
 *
 * @param x
 *   各クエリの x
 *
 * @param y
 *   各クエリの y
 *
 * @param binom
 *   nCk を計算する Binom
 *
 * @return
 *   各クエリに対する
 *   sum_{i=0}^{y[j]-1} C(x[j], i)
 */
template <class Mint>
std::vector<Mint> binom_prefix_sum(
	const std::vector<int>& x,
	const std::vector<int>& y,
	const kwm_t::math::modint::Binom<Mint>& binom
) {
	const int q = (int)x.size();

	std::vector<Mint> res(q);

	int max_coord = 0;
	for (int v : x) {
		max_coord = std::max(max_coord, v);
	}
	for (int v : y) {
		max_coord = std::max(max_coord, v);
	}

	kwm_t::algorithm::Mo mo(max_coord + 1, q);
	mo.query(x, y);

	const Mint inv2 = Mint(2).inv();

	int current_x = 0;
	int current_y = 0;
	Mint value = 0;

	// x -> x + 1
	auto expand_x = [&](int) {
		value *= 2;
		value -= binom(current_x, current_y - 1);
		++current_x;
	};

	// x -> x - 1
	auto shrink_x = [&](int) {
		--current_x;
		value += binom(current_x, current_y - 1);
		value *= inv2;
	};

	// y -> y + 1
	auto expand_y = [&](int) {
		value += binom(current_x, current_y);
		++current_y;
	};

	// y -> y - 1
	auto shrink_y = [&](int) {
		--current_y;
		value -= binom(current_x, current_y);
	};

	auto output = [&](int idx) {
		res[idx] = value;
	};

	mo.run(
		shrink_x,
		expand_y,
		expand_x,
		shrink_y,
		output,
		0,
		0
	);

	return res;
}

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_BINOM_PREFIX_SUM_HPP