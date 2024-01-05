#include <vector>
#include "MintBinom.hpp"
// 二項係数の区間和
struct ChooseLangeSum {
public:
	ChooseLangeSum(int n) :n(n) { Init(); }
private:
	//下準備
	void Init() {
		sqrtn = sqrt(n);
		int sz = n / sqrtn + 1;
		v.resize(sz);
		for (int i = 0; i < sz; ++i)v[i].resize(n + 1);
		for (int i = 0; i < sz; ++i)for (int j = 0; i < n + 1; ++j) {
			int x = sqrtn * i;
			if (0 != j)v[i][j] = v[i][j - 1];
			v[i][j] += c(x, j);
		}
	}
public:
	// f(n,m) = Binom(n,0) +...+Binom(n,m)
	mint f(int n, int m) {
		mint ret = v[n / sqrtn][m];
		int x = (n / sqrtn)*sqrtn;
		while (n != x) {
			ret *= 2;
			ret -= c(x, m);
			x++;
		}
		return ret;
		//if (0 == n % sqrtn) return v[n / sqrtn][m];
		//return f(n - 1, m) * 2 - c(n - 1, m);
	}
	mint f(int n, int l, int r) {
		// Binom(n,l)+...+Binom(n,r)
		return f(n, r) - f(n, l - 1);
	}
private:
	int n;
	int sqrtn;
	std::vector<std::vector<mint>>v;
};