/**
 * @file xor_linear_basis.hpp
 * @brief XOR 線形基底
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/xor_linear_basis.hpp
 */
#ifndef KWM_T_MATH_XOR_LINEAR_BASIS_HPP
#define KWM_T_MATH_XOR_LINEAR_BASIS_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::math {

// XOR linear basis (minimum form, noshi91 style)
// verified
// https://atcoder.jp/contests/awc0125/submissions/77975682
template <class T>
std::vector<T> xor_linear_basis(std::vector<T> a) {
	std::vector<T> basis;

	for (size_t i = 0; i < a.size(); ++i) {
		// reduce a[i]
		for (const auto& b : basis) {
			a[i] = std::min(a[i], a[i] ^ b);
		}
		if (a[i] == 0) continue;

		// reduce existing basis
		for (auto& b : basis) {
			b = std::min(b, b ^ a[i]);
		}

		basis.push_back(a[i]);
	}

	std::sort(basis.begin(), basis.end());
	return basis;
}

// XOR linear basis with original indices
template <class T>
std::vector<std::pair<T, int>> xor_linear_basis_with_index(const std::vector<T>& a) {
	std::vector<std::pair<T, int>> basis;
	basis.reserve(a.size());

	for (int i = 0; i < (int)a.size(); ++i) {
		T x = a[i];

		// reduce x
		for (const auto& [b, _] : basis) {
			x = std::min(x, x ^ b);
		}
		if (x == 0) continue;

		// reduce existing basis
		for (auto& [b, _] : basis) {
			b = std::min(b, b ^ x);
		}

		basis.emplace_back(x, i);
	}

	std::sort(basis.begin(), basis.end());
	return basis;
}

/**
 * @brief 各 XOR 値を作る部分集合の個数
 *
 * @tparam T 整数型
 * @param a 元の配列
 * @param bit XOR 値の最大ビット数
 * @return ret[x]: XOR が x となる部分集合の個数
 *
 * 計算量: O(2^bit * bit + N * bit)
 * 
 * verified
 * https://atcoder.jp/contests/awc0196/submissions/80730347
 */
template <class T>
std::vector<long long> xor_subset_count(const std::vector<T>& a, int bit) {
	auto basis = xor_linear_basis(a);

	int zero = (int)a.size() - (int)basis.size();
	long long cnt = 1LL << zero;

	int n = 1 << bit;
	std::vector<long long> ret(n);

	for (int x = 0; x < n; ++x) {
		T v = x;

		for (const auto& b : basis) {
			if ((v ^ b) < v) v ^= b;
		}

		if (v == 0) ret[x] = cnt;
	}

	return ret;
}

} // namespace kwm_t::math

#endif