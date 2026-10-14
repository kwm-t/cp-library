/**
 * @file gcd_convolution.hpp
 * @brief GCD 畳み込み
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/gcd_convolution.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_GCD_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_GCD_CONVOLUTION_HPP

#include <vector>
#include "../sieve.hpp"
#include "internal/multiple_transform.hpp"

/**
 * @brief GCD畳み込み
 *
 * c[k] = Σ gcd(i,j)=k a[i]*b[j]
 *
 * 典型用途:
 *   - 数論畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * @tparam T
 *
 * verified:
 */
namespace kwm_t::math::convolution {

template <typename T>
std::vector<T> gcd_convolution(const std::vector<T>& a, const std::vector<T>& b) {
	int n = std::max(a.size(), b.size());

	auto is_prime = kwm_t::math::sieve(n);

	std::vector<T> A(n), B(n);
	for (int i = 0; i < (int)a.size(); ++i) A[i] = a[i];
	for (int i = 0; i < (int)b.size(); ++i) B[i] = b[i];

	internal::multiple_zeta_transform(A, is_prime);
	internal::multiple_zeta_transform(B, is_prime);

	std::vector<T> C(n);
	for (int i = 1; i < n; ++i) C[i] = A[i] * B[i];

	internal::multiple_mobius_transform(C, is_prime);
	return C;
}

}

#endif // KWM_T_MATH_CONVOLUTION_GCD_CONVOLUTION_HPP