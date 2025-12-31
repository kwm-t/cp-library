#include<vector>
// 約数の高速ゼータ変換
void zeta(std::vector<mint> &v) {
	int n = v.size() - 1;
	std::vector<bool> isprime(n + 1, true);
	for (int i = 2; i * i <= n; ++i) {
		if (!isprime[i]) continue;
		for (int j = i * 2; j <= n; j += i) isprime[j] = false;
	}
	for (int p = 2; p <= n; ++p) {
		if (!isprime[p]) continue;
		for (long long i = p; i <= (n / p) * p; i += p) {
			v[i] += v[i / p];
		}
	}
}
// 約数の高速メビウス変換
void mebius(std::vector<mint> &v) {
	int n = v.size() - 1;
	std::vector<bool> isprime(n + 1, true);
	for (int i = 2; i * i <= n; ++i) {
		if (!isprime[i]) continue;
		for (int j = i * 2; j <= n; j += i) isprime[j] = false;
	}
	for (int p = 2; p <= n; ++p) {
		if (!isprime[p]) continue;
		for (long long i = (n / p) * p; i >= p; i -= p) {
			v[i] -= v[i / p];
		}
	}
}