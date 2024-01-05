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