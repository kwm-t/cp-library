/**
 * @file linear_matroid_parity.hpp
 * @brief 線形マトロイドパリティ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/matroid/linear_matroid_parity.hpp
 */
#ifndef KWM_T_MATH_MATROID_LINEAR_MATROID_PARITY_HPP
#define KWM_T_MATH_MATROID_LINEAR_MATROID_PARITY_HPP

#include <algorithm>
#include <utility>
#include <vector>

#include "../matrix/gauss_jordan.hpp"
#include "../../utility/xor_shift.hpp"

namespace kwm_t::math::matroid {

namespace internal {

/**
 * @brief 0 でないランダムな Mint を生成する
 */
template <class Mint>
Mint random_nonzero(kwm_t::utility::Xorshift& rnd) {
	Mint x = Mint(rnd());
	while (x == Mint(0)) {
		x = Mint(rnd());
	}
	return x;
}

/**
 * @brief 行列のランクを求める
 */
template <class Mint>
int rank(std::vector<std::vector<Mint>> mat) {
	int n = static_cast<int>(mat.size());
	if (n == 0) return 0;

	std::vector<Mint> b(n, Mint(0));
	kwm_t::math::matrix::GaussJordan<Mint> gj(
		std::move(mat), std::move(b)
	);
	return gj.sweep();
}

/**
 * @brief 2 本のベクトルの外積による歪対称行列への加算
 */
template <class Mint>
void add_pair(
	std::vector<std::vector<Mint>>& mat,
	const std::vector<std::pair<int, Mint>>& p,
	const std::vector<std::pair<int, Mint>>& q
) {
	for (const auto& [i, x] : p) {
		for (const auto& [j, y] : q) {
			mat[i][j] += x * y;
			mat[j][i] -= x * y;
		}
	}
}

}  // namespace internal

/**
 * @brief 線形マトロイドパリティ（dense）
 *
 * 各ペアは 2 本のベクトル (p, q) からなる。
 * 各ベクトルの非ゼロ座標に独立なランダム値を割り当て、
 * 線形独立なペアの最大数を確率的に求める。
 *
 * 入力の T は座標が 0 か非 0 かを判定するためだけに使う。
 * 実際のベクトルの値としては使用しない。
 *
 * @tparam Mint 有限体の型
 * @tparam T 入力ベクトルの要素型
 * @param pairs ベクトルのペア
 * @param rnd 乱数生成器
 * @return 最大ペア数の確率的な推定値
 *
 * 各ベクトルの長さはすべて等しいこと。
 * 
 * verified
 * https://atcoder.jp/contests/abc482/submissions/80547562
 */
template <class Mint, class T>
int dense(
	const std::vector<std::pair<std::vector<T>, std::vector<T>>>& pairs,
	kwm_t::utility::Xorshift& rnd,
	int repeat = 2
) {
	int n = 0;
	if (!pairs.empty()) {
		n = static_cast<int>(pairs[0].first.size());
	}

	int ans = 0;
	for (int rep = 0; rep < repeat; ++rep) {
		std::vector<std::vector<Mint>> mat(n, std::vector<Mint>(n));

		for (const auto& [p, q] : pairs) {
			std::vector<std::pair<int, Mint>> x, y;

			for (int i = 0; i < n; ++i) {
				if (p[i] != T(0)) {
					x.emplace_back(i, Mint(val) * internal::random_nonzero<Mint>(rnd));
				}
				if (q[i] != T(0)) {
					y.emplace_back(i, Mint(val) * internal::random_nonzero<Mint>(rnd));
				}
			}

			internal::add_pair(mat, x, y);
		}

		ans = std::max(ans, internal::rank(std::move(mat)) / 2);
	}

	return ans;
}

/**
 * @brief 線形マトロイドパリティ（sparse）
 *
 * 各ベクトルは (座標, 値) の組の列で表す。
 * 値が T(0) でない座標に独立なランダム値を割り当てる。
 *
 * 入力の T は座標が 0 か非 0 かを判定するためだけに使う。
 * 実際のベクトルの値としては使用しない。
 *
 * @tparam Mint 有限体の型
 * @tparam T 入力ベクトルの要素型
 * @param n ベクトルの次元
 * @param pairs ベクトルのペア
 * @param rnd 乱数生成器
 * @return 最大ペア数の確率的な推定値
 *
 * 各ベクトル内で座標は重複せず、0 <= 座標 < n を満たすこと。
 * 
 * verified
 * https://atcoder.jp/contests/abc482/submissions/80547619
 */
template <class Mint, class T>
int sparse(
	int n,
	const std::vector<
	std::pair<
	std::vector<std::pair<int, T>>,
	std::vector<std::pair<int, T>>
	>
	>& pairs,
	kwm_t::utility::Xorshift& rnd,
	int repeat = 2
) {
	int ans = 0;
	for (int rep = 0; rep < repeat; ++rep) {
		std::vector<std::vector<Mint>> mat(n, std::vector<Mint>(n));

		for (const auto& [p, q] : pairs) {
			std::vector<std::pair<int, Mint>> x, y;
			x.reserve(p.size());
			y.reserve(q.size());

			for (const auto& [i, val] : p) {
				if (val != T(0)) {
					x.emplace_back(i, Mint(val) * internal::random_nonzero<Mint>(rnd));
				}
			}
			for (const auto& [i, val] : q) {
				if (val != T(0)) {
					y.emplace_back(i, Mint(val) * internal::random_nonzero<Mint>(rnd));
				}
			}

			internal::add_pair(mat, x, y);
		}

		ans = std::max(ans, internal::rank(std::move(mat)) / 2);
	}

	return ans;
}

}  // namespace kwm_t::math::matroid

#endif