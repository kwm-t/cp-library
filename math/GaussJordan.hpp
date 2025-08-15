#include <vector>
#include <string>
#include <iostream> 

template<typename ModInt>
struct GaussJordan {
	GaussJordan(std::vector<std::vector<ModInt>>_a, std::vector<ModInt>_b) :a(_a), b(_b) {}
	// デバッグ用
	void dbg() {
		int n = a.size();
		int m = a[0].size();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)std::cout << a[i][j].val() << " ";
			std::cout << std::endl;
		}
		for (int i = 0; i < n; ++i) std::cout << b[i].val() << " ";
		std::cout << std::endl;
	}
	// 掃き出し法
	int sweep() {
		int n = a.size();
		int m = a[0].size();
		rank = 0;
		for (int i = 0; i < m; ++i) {
			int pivot = -1;
			for (int j = rank; j < n; ++j) {
				if (0 != a[j][i]) {
					pivot = j;
					break;
				}
			}
			if (-1 == pivot)continue;
			ModInt inv = inv_mod(a[pivot][i].val(), ModInt::mod());
			std::swap(a[rank], a[pivot]);
			std::swap(b[rank], b[pivot]);
			for (int j = i; j < m; ++j) a[rank][j] *= inv;
			b[rank] *= inv;
			for (int j = 0; j < n; ++j) {
				if (rank == j)continue;
				if (0 == a[j][i])continue;
				ModInt x = a[j][i];
				for (int k = 0; k < m; ++k) a[j][k] -= x * a[rank][k];
				b[j] -= x * b[rank];
			}
			rank++;
		}
		return rank;
	}
	// Ax= Bなxを１つ求める
	bool solve() {
		int n = a.size();
		int m = a[0].size();
		x.assign(m, 0);
		sweep();
		for (int i = rank; i < n; ++i)if (0 != b[i].val()) return false;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (0 != a[i][j]) {
					x[j] = b[i];
					break;
				}
			}
		}
		return true;
	}
	// 核
	std::vector<std::vector<ModInt>> kernel() {
		int n = a.size();
		int m = a[0].size();
		sweep();
		std::vector<int>cols(m, 0), piv;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (1 == a[i][j]) {
					cols[j] = 1;
					piv.push_back(j);
					break;
				}
			}
		}
		ker.clear();
		for (int i = 0; i < m; ++i) {
			if (cols[i])continue;
			std::vector<ModInt>v(m);
			v[i] = -1;
			for (int j = 0; j < piv.size(); ++j) v[piv[j]] = a[j][i];
			ker.push_back(v);
		}
		return ker;
	}
	int rank;
	std::vector<std::vector<ModInt>>a;
	std::vector<ModInt>x;
	std::vector<ModInt>b;
	std::vector<std::vector<ModInt>>ker;
};
// how to use
// using modint2 = static_modint<2>;
// vector a(n, vector<modint2>(n));
// vector<modint2>b(n);


// Aの線形結合でBを表現できるかの判定と一例を示す
std::pair<bool, std::vector<int>> gaussJordan_ll(const std::vector<long long>&A, const long long B) {
	int n = A.size();
	const int SIZE = 60;
	using modint2 = atcoder::static_modint<2>;
	std::vector a(SIZE, std::vector<modint2>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			a[j][i] = (1 & (A[i] >> j));
		}
	}
	std::vector<modint2>b(SIZE);
	for (int i = 0; i < SIZE; ++i)b[i] = 1 & (B >> i);
	GaussJordan gj(a, b);
	gj.sweep();
	auto res = gj.solve();
	if (!res) return{ false,{} };
	auto x = gj.x;
	vector<int>X((int)x.size());
	for (int i = 0; i < (int)x.size(); ++i)X[i] = x[i].val();
	return { true,X };
}