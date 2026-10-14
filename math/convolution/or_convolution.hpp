/**
 * @file or_convolution.hpp
 * @brief OR 畳み込み
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/or_convolution.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_OR_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_OR_CONVOLUTION_HPP

#include <vector>
#include "internal/walsh_hadamard.hpp"

/**
 * @brief OR畳み込み
 */
namespace kwm_t::math::convolution {

template <typename T>
std::vector<T> or_convolution(std::vector<T> a, std::vector<T> b) {
	int n = 1;
	int sz = std::max(a.size(), b.size());
	while (n < sz) n <<= 1;

	a.resize(n);
	b.resize(n);

	internal::or_transform(a);
	internal::or_transform(b);

	for (int i = 0; i < n; ++i) a[i] *= b[i];

	internal::or_transform_inv(a);
	return a;
}

}

#endif