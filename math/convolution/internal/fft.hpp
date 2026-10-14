/**
 * @file fft.hpp
 * @brief FFT の内部実装
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/internal/fft.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_FFT_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_FFT_HPP

#include <vector>
#include <complex>
#include <cmath>

/**
 * @brief FFT（複素数）
 *
 * 計算量: O(n log n)
 */
namespace kwm_t::math::convolution::internal {

const double pi = acos(-1);

std::vector<std::complex<double>> fft(const std::vector<std::complex<double>> &a, double inv) {
	// inv = 1 ならば普通の FFT，inv = -1 ならば IFFT になるようにする．
	int n = (int)a.size();
	if (1 == n)return a;

	std::vector<std::complex<double>> ret;

	std::vector<std::complex<double>> even;
	std::vector<std::complex<double>> odd;
	for (int i = 0; i < n; ++i) {
		if (0 == i % 2) even.push_back(a[i]);
		else odd.push_back(a[i]);
	}

	auto d_even = fft(even, inv);
	auto d_odd = fft(odd, inv);

	const std::complex<double> zeta(cos(2 * pi * inv / (double)n), sin(2 * pi * inv / (double)n));

	std::complex<double> now = 1;
	for (int i = 0; i < n; i++) {
		ret.push_back(d_even[i % (n / 2)] + now * d_odd[i % (n / 2)]);
		now *= zeta;
	}
	return ret;
}
}

#endif