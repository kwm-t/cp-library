/**
 * @file ntt.hpp
 * @brief NTT の内部実装
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/internal/ntt.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_NTT_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_NTT_HPP

#include <vector>
#include <algorithm>
#include <atcoder/all>

/**
 * @brief NTT（自前実装・再帰）
 *
 * 典型用途:
 *   - mod 998244353 の畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * 制約 / 注意:
 *   - サイズは 2^k
 *
 * verified:
 */
namespace kwm_t::math::convolution::internal {

using mint = atcoder::modint998244353;
constexpr int mod = 998244353;

// ====================
// root生成
// ====================

inline std::vector<mint> make_root() {
	std::vector<mint> root;
	long long r = atcoder::pow_mod(3, 119, mod); // primitive root

	for (int i = 0; i < 23; i++) {
		root.push_back(r);
		r = (r * r) % mod;
	}
	std::reverse(root.begin(), root.end());
	return root;
}

inline std::vector<mint> make_invroot(const std::vector<mint>& root) {
	std::vector<mint> invroot(root.size());
	for (int i = 0; i < (int)root.size(); i++) {
		invroot[i] = root[i].inv();
	}
	return invroot;
}

// ====================
// NTT本体（再帰）
// ====================

inline std::vector<mint> ntt(const std::vector<mint>& a, int depth, const std::vector<mint>& root) {
	int n = (int)a.size();
	if (n == 1) return a;

	std::vector<mint> even, odd;
	even.reserve(n / 2);
	odd.reserve(n / 2);

	for (int i = 0; i < n; ++i) {
		(i & 1 ? odd : even).push_back(a[i]);
	}

	auto d_even = ntt(even, depth - 1, root);
	auto d_odd = ntt(odd, depth - 1, root);

	std::vector<mint> ret(n);
	const mint r = root[depth];

	mint now = 1;
	for (int i = 0; i < n; i++) {
		ret[i] = d_even[i % (n / 2)] + now * d_odd[i % (n / 2)];
		now *= r;
	}
	return ret;
}

} // namespace

#endif // KWM_T_MATH_CONVOLUTION_INTERNAL_NTT_HPP