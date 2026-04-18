mint cayleyTheorem(int n) {
	// F(x) = x exp(F(x))
	// F(x)/exp(F(x)) = x
	// ラグランジュ反転
	if (n <= 1) return 1;
	return atcoder::pow_mod(n, n - 2, mod);
}
mint perfectMatching(int n) {
	// 完全マッチングを含む木の個数
	// 個数 = 完全マッチングの選び方(頂点数をn->n/2)
	//      * 縮約後の木(cayley)
	//      * 復元の自由度(縮約を復元するときにどれを選択するか)
	//      = (n-1)!!
	//      * cayleyTheorem(n/2)
	//		* (2*2)^(n/2-1)
	//		= n^(n/2-2)*n!/(n/2)!
	if (n % 2)return 0;
	if (n <= 2)return 1;
	return mint(n).pow(n / 2 - 2) * c.fact[n] * c.ifact[n / 2];
}