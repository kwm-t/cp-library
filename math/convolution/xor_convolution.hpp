/**
 * @file xor_convolution.hpp
 * @brief XOR 畳み込み
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/xor_convolution.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_XOR_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_XOR_CONVOLUTION_HPP

#include <vector>
#include "internal/walsh_hadamard.hpp"

/**
 * @brief XOR畳み込み
 *
 * 計算量: O(n log n)
 */
namespace kwm_t::math::convolution {

template <typename T>
std::vector<T> xor_convolution(std::vector<T> a, std::vector<T> b) {
	int n = 1;
	int sz = std::max(a.size(), b.size());
	while (n < sz) n <<= 1;

	a.resize(n);
	b.resize(n);

	internal::xor_transform(a);
	internal::xor_transform(b);

	for (int i = 0; i < n; ++i) a[i] *= b[i];

	internal::xor_transform_inv(a);
	return a;
}

}

#endif