#ifndef KWM_T_MATH_CONVOLUTION_NTT_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_NTT_CONVOLUTION_HPP

#include <vector>
#include "internal/ntt.hpp"

/**
 * @brief NTT畳み込み（自前）
 *
 * 典型用途:
 *   - mod 998244353 の多項式畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * verified:
 */
namespace kwm_t::math::convolution {

using mint = internal::mint;

inline std::vector<mint> ntt_convolution(std::vector<mint> a, std::vector<mint> b) {
	auto root = internal::make_root();
	auto invroot = internal::make_invroot(root);

	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n < sizeC) n <<= 1;

	a.resize(n);
	b.resize(n);

	int log_n = 0;
	while ((1 << log_n) < n) log_n++;

	auto da = internal::ntt(a, log_n - 1, root);
	auto db = internal::ntt(b, log_n - 1, root);

	std::vector<mint> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = internal::ntt(dc, log_n - 1, invroot);

	std::vector<mint> res(sizeC);
	mint invN = mint(n).inv();
	for (int i = 0; i < sizeC; ++i) {
		res[i] = c[i] * invN;
	}
	return res;
}

}

#endif // KWM_T_MATH_CONVOLUTION_NTT_CONVOLUTION_HPP