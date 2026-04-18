// 独立集合の数え上げ O(n * 2^n)
std::vector<mint> countIndependentSet(const std::vector<std::vector<int>>&g) {
	int n = g.size();
	std::vector<mint> cnt(1 << n);
	for (int i = 0; i < (1 << n); ++i) {
		if (0 == i) {
			cnt[i] = 1;
			continue;
		}
		int s = __builtin_clz(i);
		int sub = i - (1 << s);
		cnt[i] = cnt[sub];
		for (auto j : g[s]) {
			sub &= ~(1 << j);
		}
		cnt[i] += cnt[sub];
	}
	return cnt;
}
// 彩色数 O(n * 2^n)
int chromaticNumber(const std::vector<std::vector<int>>&g) {
	int n = g.size();
	if (0 == n)return 0;
	auto ind = countIndependentSet(g);
	std::vector<mint>pow(1 << n);
	for (int i = 0; i < (1 << n); ++i) {
		pow[i] = (n - __builtin_popcount(i)) % 2 ? -1 : 1;
	}
	for (int i = 1; i < n; ++i) {
		mint sum = 0;
		for (int j = 0; j < (1 << n); ++j) {
			pow[j] *= ind[j];
			sum += pow[j];
		}
		if (0 != sum)return i;
	}
	return n;
}