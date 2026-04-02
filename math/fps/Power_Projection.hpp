#ifndef KWM_T_MATH_FPS_POWER_PROJECTION_HPP
#define KWM_T_MATH_FPS_POWER_PROJECTION_HPP

#include <vector>
#include <algorithm>
#include <atcoder/convolution>

//#include "fps.hpp"

namespace kwm_t::math::fps {

namespace detail {

// v の偶数/奇数項を抽出（NTT用のやつ）
// odd = 0 : even
// odd = 1 : odd
template <class T>
void pick_even_odd(std::vector<T>& v, bool odd) {
	int half_n = (int)v.size() / 2;
	T inv2 = T(1) / T(2);

	if (!odd) {
		// even part
		for (int i = 0; i < half_n; i++) {
			v[i] = (v[2 * i] + v[2 * i + 1]) * inv2;
		}
	}
	else {
		// odd part
		T root = T(atcoder::internal::primitive_root_constexpr(T::mod()))
			.pow(T::mod() / (2 * half_n));
		T inv_root = T(1) / root;

		std::vector<T> coef = { inv2 };
		while ((int)coef.size() < half_n) {
			int sz = coef.size();
			std::vector<T> nxt(sz * 2);
			for (int i = 0; i < sz; i++) {
				nxt[2 * i] = coef[i];
				nxt[2 * i + 1] = coef[i] * inv_root;
			}
			inv_root *= inv_root;
			coef.swap(nxt);
		}

		for (int i = 0; i < half_n; i++) {
			v[i] = (v[2 * i] - v[2 * i + 1]) * coef[i];
		}
	}

	v.resize(half_n);
}

} // namespace detail


/**
 * @brief Power Projection
 *
 * g(x), f(x) に対して
 *   [x^{n-1}] g(x) * f(x)^i を i = 0..m-1 で返す
 *
 * @tparam T modint
 * @param g
 * @param f
 * @param m
 * @return std::vector<T>
 *
 * 制約:
 *   |g| = n (n は 2 のべき)
 *
 * 計算量:
 *   O(n log n + m log m)
 * verified
 *   https://atcoder.jp/contests/fps-24/submissions/74606486
 *   https://atcoder.jp/contests/abc439/submissions/74606553
 */
template <class T>
std::vector<T> power_projection(std::vector<T> g,
	std::vector<T> f,
	int m) {
	int deg = (int)g.size() - 1;

	int n = 1;
	while (n < (int)g.size()) n <<= 1;

	g.resize(n, T(0));
	f.resize(n, T(0));

	std::vector<T> prev_g(n), prev_f(n);

	// g / (y - f)
	for (auto& x : f) x = -x;

	int block = n;
	T inv_norm = T(1) / T(2 * n);

	while (block > 1) {
		prev_g = g;
		prev_f = f;

		// expand
		g.resize(4 * n);
		f.resize(4 * n);

		for (int i = n / block - 1; i >= 0; i--) {
			for (int j = block - 1; j >= 0; j--) {
				g[i * block * 2 + j] = g[i * block + j];
				f[i * block * 2 + j] = f[i * block + j];

				if (i) {
					g[i * block + j] = T(0);
					f[i * block + j] = T(0);
				}
			}
		}

		atcoder::internal::butterfly(g);
		atcoder::internal::butterfly(f);

		for (int i = 0; i < 2 * n; i++) {
			T g0 = g[2 * i], g1 = g[2 * i + 1];
			T f0 = f[2 * i], f1 = f[2 * i + 1];

			g[2 * i] = g0 * f1;
			g[2 * i + 1] = g1 * f0;

			f[2 * i] = f0 * f1;
			f[2 * i + 1] = f[2 * i];
		}

		detail::pick_even_odd(g, deg & 1);
		detail::pick_even_odd(f, false);

		atcoder::internal::butterfly_inv(g);
		atcoder::internal::butterfly_inv(f);

		for (auto& x : g) x *= inv_norm;
		for (auto& x : f) x *= inv_norm;

		// y^block の補正
		for (int i = 0; i < n; i++) {
			if ((deg + i + 1) & 1) {
				int idx = n + (i / block) * block + ((i & (block - 1)) >> 1);
				g[idx] += prev_g[i];
			}
			if ((i & 1) == 0) {
				int idx = n + (i / block) * block + ((i & (block - 1)) >> 1);
				f[idx] += prev_f[i] * 2;
			}
		}

		block >>= 1;

		for (int i = 0; i < n; i++) {
			int src = (i / block) * block * 2 + (i & (block - 1));
			g[i] = g[src];
			f[i] = f[src];
		}

		g.resize(n);
		f.resize(n);
		deg >>= 1;
	}

	f.push_back(T(1));

	std::reverse(g.begin(), g.end());
	std::reverse(f.begin(), f.end());

	g.resize(m);

	auto res = atcoder::convolution(g,
		(FormalPowerSeries<T>(f)).inv(m));

	res.resize(m);
	return res;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_POWER_PROJECTION_HPP