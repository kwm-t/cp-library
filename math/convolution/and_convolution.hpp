#ifndef KWM_T_MATH_CONVOLUTION_AND_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_AND_CONVOLUTION_HPP

#include <vector>
#include "internal/walsh_hadamard.hpp"

/**
 * @brief AND畳み込み
 * 
 * verified
 *  - https://judge.yosupo.jp/submission/365950
 */
namespace kwm_t::math::convolution {

template <typename T>
std::vector<T> and_convolution(std::vector<T> a, std::vector<T> b) {
	int n = 1;
	int sz = std::max(a.size(), b.size());
	while (n < sz) n <<= 1;

	a.resize(n);
	b.resize(n);

	internal::and_transform(a);
	internal::and_transform(b);

	for (int i = 0; i < n; ++i) a[i] *= b[i];

	internal::and_transform_inv(a);
	return a;
}

}

#endif