#include <vector>
#include <atcoder/all>
// Π(1+A_i x)のFFT
std::vector<mint> divideConquerFFT(const std::vector<int> &a, int l, int r) {
	if (r == l)return { 1 };
	if (r - l == 1) return { 1, a[l] };
	int m = (l + r) / 2;
	return atcoder::convolution(divideConquerFFT(a, l, m), divideConquerFFT(a, m, r));
}


// Σ1/(1-A_{i}x)
// O(N log(N)^2)
// p(x)/q(x)の形で返す p,qはともに高々|A|次式
// https://atcoder.jp/contests/fps-24/submissions/71456093
std::pair<std::vector<mint>, std::vector<mint>> divideConquerFFT2(const std::vector<int> &a, int l, int r) {
	if (r == l)return { {1}, {1} };
	if (r - l == 1) return { {1}, {1,-a[l]} };
	int m = (l + r) / 2;
	auto[pl, ql] = divideConquerFFT2(a, l, m);
	auto[pr, qr] = divideConquerFFT2(a, m, r);
	auto p0 = atcoder::convolution(pl, qr);
	auto p1 = atcoder::convolution(pr, ql);
	auto q = atcoder::convolution(ql, qr);
	if (p0.size() < p1.size())swap(p0, p1);
	for (int i = 0; i < p1.size(); ++i)p0[i] += p1[i];
	return { p0,q };
}