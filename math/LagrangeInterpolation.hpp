// ラグランジュ補間
mint lagrange_interpolation(const std::vector<mint>& y, long long x) {
	int N = (int)y.size() - 1;
	if (x <= N) return y[x];

	std::vector<mint> invfac(N + 1, 1);
	mint fac = 1;
	for (int i = 1; i <= N; i++) fac *= i;
	invfac[N] = fac.inv();
	for (int i = N; --i > 0;) invfac[i] = invfac[i + 1] * (i + 1);

	std::vector<mint> dp(N + 1, 1), pd(N + 1, 1);
	for (int i = 0; i < N; i++) dp[i + 1] = dp[i] * (x - i);
	for (int i = N; i > 0; i--) pd[i - 1] = pd[i] * (x - i);

	mint ret = 0;
	for (int i = 0; i <= N; i++) {
		mint tmp = y[i] * dp[i] * pd[i] * invfac[i] * invfac[N - i];
		ret += ((N - i) & 1) ? -tmp : tmp;
	}
	return ret;
}