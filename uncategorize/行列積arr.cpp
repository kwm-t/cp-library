#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint1000000007;
const int mod = 1000000007;
//using mint = modint998244353;
//const int mod = 998244353;
//const int INF = 1e9;
//const long long LINF = 1e18;
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep2(i,l,r)for(int i=(l);i<(r);++i)
#define rrep(i, n) for (int i = (n-1); i >= 0; --i)
#define rrep2(i,l,r)for(int i=(r-1);i>=(l);--i)
#define all(x) (x).begin(),(x).end()
#define allR(x) (x).rbegin(),(x).rend()
#define endl "\n"
#define P pair<int,int>
template<typename A, typename B> inline bool chmax(A & a, const B & b) { if (a < b) { a = b; return true; } return false; }
template<typename A, typename B> inline bool chmin(A & a, const B & b) { if (a > b) { a = b; return true; } return false; }
const int X = 12;
struct matrix {
	array<array<mint, X>, X> v;
	matrix(int i = 0) {
		if (1 == i) { for (int i = 0; i < X; ++i) { v[i][i] = 1; } }
	}
	matrix(const array<array<mint, X>, X> &s) { v = s; }
	matrix operator*=(const matrix & m) {
		matrix s(0);
		for (int i = 0; i < X; ++i) {
			for (int j = 0; i < X; ++j) {
				for (int k = 0; i < X; ++k) {
					s.v[i][j] += m.v[i][k] * v[k][j];
				}
			}
		}
		swap(s, *this);
		return *this;
	}
};
struct solver {
	solver() {}
	vector<matrix> pw;
	void build(matrix base, int n = 60) {
		pw.resize(n);
		pw[0] = base;
		for (int i = 1; i < n; ++i) {
			pw[i] = pw[i - 1];
			pw[i] *= pw[i];
		}
	}
	void calc(long long n, matrix &ans) {
		for (int i = 0; i < (int)pw.size(); ++i) {
			if (1 == n % 2) ans *= pw[i];
			n /= 2;
		}
	}
	vector<mint>solve(vector<long long>input, vector<mint>one) {
		int sz = input.size();
		vector<mint>ret(sz);
		for (int i = 0; i < sz; ++i) {
			matrix mat(1);
			calc(input[i] - 1, mat);
			mint val = 0;
			for (int j = 0; j < (int)one.size(); ++j) val += one[j] * mat.v[6][j];// 問題固有
			ret[i] = val;
		}
		return ret;
	}
};
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int t; cin >> t;
	vector<long long>a(t);
	rep(i, t) cin >> a[i];
	solver solver;

	matrix base(0);
	rep(i, X / 2) {
		rep(j, 10) {
			int nj = (i * 10 + j) % 6;
			base.v[nj][i] += 1;
			base.v[nj + X / 2][i] += j;
			base.v[nj + X / 2][i + X / 2] += 1;
		}
	}
	solver.build(base);
	vector<mint>v = { 1,2,2,2,1,1,6,8,10,12,4,5 };
	auto get = solver.solve(a, v);
	rep(i, get.size())cout << get[i].val() << endl;
	return 0;
}