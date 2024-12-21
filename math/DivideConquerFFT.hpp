#include <vector>
#include <atcoder/all>
// Π(1+A_i x)のFFT
std::vector<mint> divideConquerFFT(const std::vector<int> &a, int l, int r) {
	if (r == l)return { 1 };
	if (r - l == 1) return { 1, a[l] };
	int m = (l + r) / 2;
	return atcoder::convolution(divideConquerFFT(a, l, m), divideConquerFFT(a, m, r));
}