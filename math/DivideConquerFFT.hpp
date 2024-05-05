#include <vector>
#include <atcoder/all>
// ƒ®(1+A_i x)‚ÌFFT
std::vector<mint> divideConquerFFT(const std::vector<int> &a, int l, int r) {
	if (r - l == 1) return { 1, a[l] };
	int m = (l + r) / 2;
	return atcoder::convolution(divideConquerFFT(a, l, m), divideConquerFFT(a, m, r));
}