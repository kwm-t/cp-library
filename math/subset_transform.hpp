/**
 * @file subset_transform.hpp
 * @brief 部分集合変換
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/subset_transform.hpp
 */
#ifndef KWM_T_MATH_SUBSET_TRANSFORM_HPP
#define KWM_T_MATH_SUBSET_TRANSFORM_HPP

#include <utility>
#include <vector>

/**
 * @brief Subset 上の変換
 *
 * 典型用途:
 *   - 高速ゼータ変換
 *   - 高速メビウス変換
 *
 * 各 j について、j ビットを持つ集合 i に対して
 * op(z[i], z[i \ {j}) を適用する。
 *
 * 計算量:
 *   O(n 2^n)
 * verified
 * https://atcoder.jp/contests/awc0168/submissions/79785831
 */
namespace kwm_t::math {

template <typename T, typename Op>
std::vector<T> transform(int n, std::vector<T> z, Op op) {
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < (1 << n); ++i) {
			if (i >> j & 1) {
				op(z[i], z[i & ~(1 << j)]);
			}
		}
	}
	return z;
}

/**
 * @brief 高速ゼータ変換
 *
 * z[S] <- sum_{T subseteq S} z[T]
 */
template <typename T>
std::vector<T> zeta(int n, std::vector<T> z) {
	return transform(n, std::move(z), [](T& x, const T& y) {
		x += y;
	});
}

/**
 * @brief 高速メビウス変換
 *
 * z[S] <- sum_{T subseteq S} (-1)^{|S \ T|} z[T]
 */
template <typename T>
std::vector<T> mobius(int n, std::vector<T> z) {
	return transform(n, std::move(z), [](T& x, const T& y) {
		x -= y;
	});
}

}

#endif // KWM_T_MATH_SUBSET_TRANSFORM_HPP