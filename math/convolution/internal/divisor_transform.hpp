/**
 * @file divisor_transform.hpp
 * @brief 約数変換
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/internal/divisor_transform.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_DIVISOR_TRANSFORM_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_DIVISOR_TRANSFORM_HPP

#include <vector>

/**
 * @brief 約数ゼータ / メビウス変換
 *
 * f[i] に対して約数に関する変換を行う
 *
 * 典型用途:
 *   - 約数畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * @tparam T
 *
 * 制約 / 注意:
 *   - f[0] は未使用 or 0
 *
 * verified:
 */
namespace kwm_t::math::convolution::internal {

template <typename T>
void divisor_zeta_transform(std::vector<T>& f, const std::vector<bool>& is_prime) {
	int n = (int)f.size() - 1;

	for (int p = 2; p <= n; ++p) {
		if (!is_prime[p]) continue;
		for (int i = p; i <= (n / p) * p; i += p) {
			f[i] += f[i / p];
		}
	}
}

template <typename T>
void divisor_mobius_transform(std::vector<T>& f, const std::vector<bool>& is_prime) {
	int n = (int)f.size() - 1;

	for (int p = 2; p <= n; ++p) {
		if (!is_prime[p]) continue;
		for (int i = (n / p) * p; i >= p; i -= p) {
			f[i] -= f[i / p];
		}
	}
}

}

#endif // KWM_T_MATH_CONVOLUTION_INTERNAL_DIVISOR_TRANSFORM_HPP