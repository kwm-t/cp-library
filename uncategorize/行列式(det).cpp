#include "bits/stdc++.h"
//#include "atcoder/all"
using namespace std;
//using namespace atcoder;
//using mint = modint1000000007;
//onst int mod = 1000000007;
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
long long mod_pow(long long num, long long p, long long mod) {
	//num^p(mod)
	long long ret = 1;
	while (p > 0) {
		if (p % 2) {
			ret *= num;
			ret %= mod;
		}
		num *= num;
		num %= mod;
		p /= 2;
	}
	return ret;
}
long long mod_inv(long long num, long long mod) {
	return mod_pow(num, mod - 2, mod);
}
//行列の行列式(mod p上での)を求める(modは素数じゃないとだめ、それが無理ならdoubleでやって補正しろ)
long long det(vector<vector<long long>>&x, long long mod) {
	auto A = x;
	rep(i, A.size()) rep(j, A.size()) A[i][j] %= mod;
	long long ret = 1;
	//基本変形を繰り返し、上三角行列にしたい。
	rep(i, A.size()) {
		int tmp = -1;
		rep2(j, i, A.size()) {
			if (0 != A[j][i]) {
				tmp = j;
				break;
			}
		}
		if (-1 == tmp) {
			ret *= 0;
			continue;
		}
		if (i != tmp) {//入れ替え
			swap(A[i], A[tmp]);
			ret = (mod - ret) % mod;
		}
		long long inv = mod_inv(A[i][i], mod);
		rep2(j, i + 1, A.size()) {
			long long c = (A[j][i] * inv) % mod;
			rep2(k, i, A.size()) {
				A[j][k] -= (c * A[i][k]) % mod;
				A[j][k] += mod;
				A[j][k] %= mod;
			}
		}
		ret *= A[i][i];
		ret %= mod;
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int N;
	cin >> N;
	vector<vector<long long>> S(N, vector<long long>(N));
	rep(i, N) {
		rep(j, N) {
			cin >> S[i][j];
		}
	}
	cout << det(S, 2) << endl;
	cout << det(S, 3) << endl;
	cout << det(S, 5) << endl;
	cout << det(S, 7) << endl;
	cout << det(S, 11) << endl;
	cout << det(S, 13) << endl;
	cout << det(S, 19) << endl;
	return 0;
}