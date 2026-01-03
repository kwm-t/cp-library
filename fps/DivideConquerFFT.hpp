#include <vector>
#include <atcoder/all>
using mint = atcoder::modint998244353;
const int mod = 998244353;

// Π(1+A_i x)のFFT
std::vector<mint> divideConquerFFT(const std::vector<int> &a, int l, int r) {
	if (r == l)return { 1 };
	if (r - l == 1) return { 1, a[l] };
	int m = (l + r) / 2;
	return atcoder::convolution(divideConquerFFT(a, l, m), divideConquerFFT(a, m, r));
}

// O(N log(N)^2)
// p(x)/q(x)の形で返す p,qはともに高々|A|次式
std::pair<std::vector<mint>, std::vector<mint>> divideConquerFFT(int l, int r,
	const std::function<std::pair<std::vector<mint>, std::vector<mint>>(int)> &e) {
	if (r == l)return e(-1);
	if (r - l == 1) return e(l);
	int m = (l + r) / 2;
	auto[pl, ql] = divideConquerFFT(l, m, e);
	auto[pr, qr] = divideConquerFFT(m, r, e);
	auto p0 = atcoder::convolution(pl, qr);
	auto p1 = atcoder::convolution(pr, ql);
	auto q = atcoder::convolution(ql, qr);
	if (p0.size() < p1.size())swap(p0, p1);
	for (int i = 0; i < p1.size(); ++i)p0[i] += p1[i];
	return { p0,q };
}
// how to use
// https://atcoder.jp/contests/fps-24/submissions/71555028