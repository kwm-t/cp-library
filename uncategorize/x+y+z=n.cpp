#include "bits/stdc++.h"
//#include "atcoder/all"
using namespace std;
//using namespace atcoder;
//using mint = modint1000000007;
//const int mod = 1000000007;
//using mint = modint998244353;
//const int mod = 998244353;
//const int INF = 1e9;
//const long long LINF = 1e18;
//const bool debug = false;
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
long long c3D(long long n, long long mx) {
	// how create n from[0,mx]^3
	long long ret = 0;
	if (n <= mx) {
		ret = (n + 2)*(n + 1) / 2;
	}
	else if (n < mx * 2) {
		ret += (n + 2)*(n + 1) / 2;
		ret -= (n - mx + 1)*(n - mx) / 2 * 3;
	}
	else if (n <= mx * 3) {
		ret += (mx * 3 - n + 2)*(mx * 3 - n + 1) / 2;
	}
	else {
		ret = 0;
	}
	return ret;
}
long long c2D(long long n, long long mx) {
	// how create n from[0,mx]^2
	long long ret = 0;
	if (n <= mx) {
		ret = n + 1;
	}
	else if (n <= mx * 2) {
		ret = 2 * mx - n + 1;
	}
	else {
		ret = 0;
	}
	return ret;
}
void c(long long n, long long m, long long mx) {
	// create n from [0,mx]^3 idx = m
	rep(i, mx + 1) {
		long long subn = n - i;
		long long x = c2D(subn, mx);
		if (x < m) {
			m -= x;
			continue;
		}
		else {
			m--;
			int a0 = i;
			int a1, a2;
			if (n - i <= mx) {
				a1 = m;
				a2 = n - a0 - a1;
			}
			else {
				a1 = n - a0 - mx + m;
				
			}
			a2 = n - a0 - a1;
			a0++; a1++; a2++;
			cout << a0 << " " << a1 << " " << a2 << endl;
			break;
		}
	}
}
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	long long n, k; cin >> n >> k;
	n--;
	long long sum = -1;
	while (k > 0) {
		sum++;
		long long x = c3D(sum, n);
		if (x < k) {
			k -= x;
			continue;
		}
		else {
			//合計がsum で k 番目のもの
			c(sum, k, n);
			break;
		}
	}
	return 0;
}