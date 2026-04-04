#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_FFT_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_FFT_CONVOLUTION_HPP

#include <vector>
#include <complex>
#include "internal/fft.hpp"

/**
 * @brief FFT畳み込み（double）
 */
namespace kwm_t::math::convolution {

std::vector<double> convolution(std::vector<std::complex<double>> a, std::vector<std::complex<double>> b) {
	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n <= sizeC) n *= 2;

	a.resize(n);
	b.resize(n);

	auto da = internal::fft(a, 1);
	auto db = internal::fft(b, 1);

	std::vector<std::complex<double>> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = internal::fft(dc, -1);

	// IFFT の後は最後に n で割る必要がある
	std::vector<double> ret(n);
	for (int i = 0; i < n; ++i) ret[i] = c[i].real() / (double)n;
	return ret;
}
}
#endif