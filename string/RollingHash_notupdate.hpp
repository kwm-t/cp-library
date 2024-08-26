// startのindexから右にsize分のHashを計算する
// 更新がある場合はSegment Treeを使う
// pow_modがボトルネックになるなら必要に応じて前計算をしたほうがいい
mint calcHash(int n, int start, long long size, const std::vector<mint> &hashArray, const int base) {
	long long h = std::min(size, (long long)n - start);
	long long b = ((size - h) / n) * n;
	long long t = size - h - b;
	mint hash = 0;
	if (h) {
		mint sub = (hashArray[start + h] - hashArray[start]) / atcoder::pow_mod(base, start, mod);
		hash += sub;
		//cout << sub.val() << endl;
	}
	if (b) {
		mint sub = hashArray[n] * (atcoder::pow_mod(base, b, mod) - 1) / (atcoder::pow_mod(base, n, mod) - 1) * atcoder::pow_mod(base, h, mod);
		hash += sub;
	}
	if (t) {
		mint sub = atcoder::pow_mod(base, h + b, mod)*hashArray[t];
		hash += sub;
	}
	return hash;
}