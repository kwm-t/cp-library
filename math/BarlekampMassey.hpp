#ifndef BARLEKAMP_MASSEY_HPP
#define BARLEKAMP_MASSEY_HPP

// 線形漸化式の初めのn項の特性方程式を返す k次ならO(k^2)
// a_{i} = Σ(c_{j} * a_{i-j})で表されるような数列の先頭前k項(2*k程度で十分)を与えると
// {1,-c[1],-c[2],-c[3],-c[4]....}を返す
// ex	a_{i} = a[i-1] * 3 + a[i-2] * 1-> {1,-3,-1}
template<class T>
std::vector<mint> BarlekampMassey(const std::vector<T> &A) {
	int M = A.size();
	mint y = 1; // 1つ前のxの値.
	std::vector<T> B, C;// Cはret Bは1つ前のC.
	B.reserve(M + 1), C.reserve(M + 1);
	B.emplace_back(T(1)), C.emplace_back(T(1));

	for (int i = 1; i <= M; i++) {
		int lc = C.size(), lb = B.size();
		T x = 0;
		for (int k = 0; k < lc; k++) x += C.at(k)*A.at(i - lc + k);
		B.emplace_back(T(0)); lb++;
		if (x == 0) continue;
		T Multi = x / y;
		if (lc < lb) {
			auto memo = C;
			if (lb > lc)C.insert(C.begin(), (lb - lc), 0);
			for (int k = 0; k < lb; k++) C.at(k) -= Multi * B.at(k);
			B = memo; y = x;
		}
		else for (int k = 0; k < lb; k++) C.at(lc - 1 - k) -= Multi * B.at(lb - 1 - k);
	}
	std::reverse(C.begin(), C.end());
	return C;
}

#endif