// 第二種スターリング数
mint StirlingNumbersOfTheSecondKind(int n, int k) {
	if (n < k)return 0;
	mint ret = 0;
	for (int i = 1; i <= k; ++i) {
		mint sub = c(k, i) * pow_mod(i, n, mod);
		if (i % 2 == k % 2)ret += sub;
		else ret -= sub;
	}
	return ret * c.ifact[k];
}
std::vector<std::vector<mint>>stirlingNumbersOfTheSecondKindTable(int n) {
	std::vector s(n + 1, std::vector<mint>(n + 1));
	for (int i = 1; i <= n; ++i) {
		s[i][1] = s[i][i] = 1;
	}
	for (int i = 2; i <= n; ++i) {
		for (int j = 2; j < i; ++j) {
			s[i][j] = s[i - 1][j - 1] + j * s[i - 1][j];
		}
	}
	return s;
}