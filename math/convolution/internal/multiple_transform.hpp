#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_MULTIPLE_TRANSFORM_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_MULTIPLE_TRANSFORM_HPP

#include <vector>

/**
 * @brief 倍数ゼータ / メビウス変換
 *
 * f[i] = Σ_{j: i|j} f[j]
 *
 * 典型用途:
 *   - GCD畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * verified:
 */
namespace kwm_t::math::convolution::internal {

template <typename T>
void multiple_zeta_transform(std::vector<T>& f, const std::vector<bool>& is_prime) {
	int n = (int)f.size();

	for (int p = 2; p < n; ++p) {
		if (!is_prime[p]) continue;
		for (int i = (n - 1) / p; i >= 1; --i) {
			f[i] += f[i * p];
		}
	}
}

template <typename T>
void multiple_mobius_transform(std::vector<T>& f, const std::vector<bool>& is_prime) {
	int n = (int)f.size();

	for (int p = 2; p < n; ++p) {
		if (!is_prime[p]) continue;
		for (int i = 1; i * p < n; ++i) {
			f[i] -= f[i * p];
		}
	}
}

}

#endif // KWM_T_MATH_CONVOLUTION_INTERNAL_MULTIPLE_TRANSFORM_HPP