/**
 * @file multipoint_evaluation.hpp
 * @brief 多点評価
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/fps/multipoint_evaluation.hpp
 */
#ifndef KWM_T_MATH_FPS_MULTIPOINT_EVALUATION_HPP
#define KWM_T_MATH_FPS_MULTIPOINT_EVALUATION_HPP

#include <vector>
#include <algorithm>
#include <atcoder/convolution>

#include "base/builtin.hpp"

namespace kwm_t::math::fps {

namespace detail {

// 複数FPSの積（divide & conquer）
template <class T>
std::vector<T> product_sequence(const std::vector<std::vector<T>>& polys) {
	if (polys.empty()) return { T(1) };

	auto dfs = [&](auto&& self, int l, int r) -> std::vector<T> {
		if (l + 1 == r) return polys[l];
		int m = (l + r) >> 1;
		return atcoder::convolution(
			self(self, l, m),
			self(self, m, r)
		);
	};

	return dfs(dfs, 0, (int)polys.size());
}


// 区間畳み込み（必要な範囲だけ取る）
template <class T>
std::vector<T> convolution_range(std::vector<T> a,
	std::vector<T> b,
	int l, int r) {
	if (l >= r) return {};

	if (l < 0) l = 0;

	while ((int)a.size() > r) a.pop_back();
	while ((int)b.size() > r) b.pop_back();

	int need = (int)a.size() + (int)b.size() - 1;
	need -= std::min(need - r, l);

	int z = 1;
	while (z < need) z <<= 1;

	a.resize(z, T(0));
	b.resize(z, T(0));

	atcoder::internal::butterfly(a);
	atcoder::internal::butterfly(b);

	for (int i = 0; i < z; i++) a[i] *= b[i];

	atcoder::internal::butterfly_inv(a);

	static std::vector<T> inv = { T(1), T(1) / T(2) };
	while ((int)inv.size() <= __builtin_ctz(z)) {
		inv.push_back(inv.back() * inv[1]);
	}

	std::vector<T> res(r - l);
	T coef = inv[__builtin_ctz(z)];
	for (int i = l; i < r; i++) {
		res[i - l] = a[i] * coef;
	}

	return res;
}

} // namespace detail


/**
 * @brief 多点評価
 *
 * f(x) を p[i] で評価する
 *
 * @tparam T modint
 * @param f 多項式
 * @param p 評価点
 * @return std::vector<T> f(p[i])
 *
 * 計算量:
 *   O((n + m) log^2 n)
 *
 * verified
 *   https://atcoder.jp/contests/fps-24/submissions/74606689
 */
template <class T>
std::vector<T> multipoint_evaluation(std::vector<T> f,
	const std::vector<T>& p) {
	int N = (int)p.size();
	int M = (int)f.size();

	// reverse
	std::reverse(f.begin(), f.end());

	// g[i] = (x - p[i])
	std::vector<std::vector<T>> leaf(N);
	for (int i = 0; i < N; i++) {
		leaf[i] = { T(1), -p[i] };
	}

	// 全体積
	auto prod = detail::product_sequence(leaf);

	// 逆元
	auto inv = (FormalPowerSeries<T>(prod)).inv(M);

	// f / prod
	auto tmp = atcoder::convolution(inv, f);
	tmp.resize(M);

	// セグ木構築
	int size = 1;
	while (size < N) size <<= 1;

	std::vector<std::vector<T>> seg(2 * size, { T(1) });
	for (int i = 0; i < N; i++) {
		seg[size + i] = leaf[i];
	}
	for (int i = size - 1; i > 0; i--) {
		seg[i] = atcoder::convolution(seg[i << 1], seg[i << 1 | 1]);
	}

	std::vector<T> res(N);

	// remainder tree
	auto dfs = [&](auto&& self, int node, std::vector<T> cur) -> void {
		if ((int)seg[node].size() == 1) return;

		if (node >= size) {
			res[node - size] = cur.back();
			return;
		}

		const auto& L = seg[node << 1];
		const auto& R = seg[node << 1 | 1];

		self(self, node << 1,
			detail::convolution_range(
				cur, R,
				(int)cur.size() - (int)L.size() + 1,
				(int)cur.size()
			));

		self(self, node << 1 | 1,
			detail::convolution_range(
				cur, L,
				(int)cur.size() - (int)R.size() + 1,
				(int)cur.size()
			));
	};

	dfs(dfs, 1, tmp);

	return res;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_MULTIPOINT_EVALUATION_HPP