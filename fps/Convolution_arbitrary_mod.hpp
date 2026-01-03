// 任意mod畳み込み
#include <atcoder/all>
const int mod = 1000000007;
std::vector<long long> fast_mod_convolution(std::vector<long long>&a, std::vector<long long>&b) {
	const long long m1 = 167772161, m2 = 469762049, m3 = 1224736769;
	const long long m1_inv_m2 = atcoder::pow_mod(m1, m2 - 2, m2);
	const long long m12_inv_m3 = atcoder::pow_mod(m1*m2, m3 - 2, m3);
	const long long m12_mod = m1 * m2%mod;
	auto x = atcoder::convolution<m1>(a, b);
	auto y = atcoder::convolution<m2>(a, b);
	auto z = atcoder::convolution<m3>(a, b);
	std::vector<long long> ret((int)a.size() + (int)b.size() - 1);
	for (int i = 0; i < (int)ret.size(); ++i) {
		long long v1 = (y[i] - x[i])*m1_inv_m2%m2; if (v1 < 0)v1 += m2;
		long long v2 = (z[i] - (x[i] + m1 * v1) % m3)*m12_inv_m3%m3; if (v2 < 0)v2 += m3;
		ret[i] = (x[i] + m1 * v1 + m12_mod * v2) % mod;
	}
	return ret;
}