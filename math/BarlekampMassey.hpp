// https://yukicoder.me/submissions/1098946
// 線形漸化式の初めのn項の特性方程式を返す d次ならO(d^2)
std::vector<mint> BarlekampMassey(const std::vector<mint> &A) { 
	// d次->2*d項があれば十分 ΣCix^i=0のCiを返す C0=1.
	int M = A.size();
	mint y = 1; // 1つ前のxの値.
	std::vector<mint> C, B;// Cはret Bは1つ前のC.
	C.reserve(M + 1), B.reserve(M + 1);
	C.emplace_back(mint(1)), B.emplace_back(mint(1));

	for (int i = 1; i <= M; i++) {
		int lc = C.size(), lb = B.size();
		mint x = 0;
		for (int k = 0; k < lc; k++) x += C.at(k)*A.at(i - lc + k); //revしている?.
		B.emplace_back(mint(0)); lb++;
		if (x == 0) continue;
		mint Multi = x / y;
		if (lc < lb) { // C<-C-x/y*x^m*B B.emplace_back(0)でx^mを担当?.
			auto memo = C;
			if (lb > lc)C.insert(C.begin(), (lb - lc), 0);
			for (int k = 0; k < lb; k++) C.at(k) -= Multi * B.at(k); // O(d^2)の「掛け算」が重い モンゴメれば2倍速.
			B = memo; y = x;
		}
		else for (int k = 0; k < lb; k++) C.at(lc - 1 - k) -= Multi * B.at(lb - 1 - k);
	}
	reverse(C.begin(), C.end());// revで考えていたので元に戻す.
	return C; // 何やってるか分からないシリーズ.
}
// 線形漸化式の初めのa.size()項からn項目を求める 
// d次漸化式ならO(d^2+dlogdlogN).
mint BM_BM(long long n, const std::vector<mint> &a) {
	if (n < 0) return 0;

	auto b = BarlekampMassey(a);// O(d^2)
	auto c = a;
	c.resize(b.size() - 1);
	auto d = atcoder::convolution(c, b);
	d.resize(b.size() - 1);

	Bostan_mori<mint> bm(d, b);
	return bm[n];
}