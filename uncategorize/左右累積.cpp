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
//const bool debug = false;
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep2(i,l,r)for(int i=(l);i<(r);++i)
#define rrep(i, n) for (int i = (n-1); i >= 0; --i)
#define rrep2(i,l,r)for(int i=(r-1);i>=(l);--i)
#define all(x) (x).begin(),(x).end()
#define allR(x) (x).rbegin(),(x).rend()
#define endl "\n"
#define P pair<long long,long long>
template<typename A, typename B> inline bool chmax(A & a, const B & b) { if (a < b) { a = b; return true; } return false; }
template<typename A, typename B> inline bool chmin(A & a, const B & b) { if (a > b) { a = b; return true; } return false; }

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	vector<int>a(n);
	rep(i, n)cin >> a[i];
	vector<int>sumL(n + 1), sumR(n + 1);
	//sumL[i] =[0,i);
	//sumR[i] =[n-i,n);
	rep(i, n)sumL[i + 1] = sumL[i] + a[i];
	rep(i, n)sumR[i + 1] = sumR[i] + a[n - i - 1];
	rep(i, n) {
		//a[i]抜き <=>[0,i) + [i+1,n);
		int val = sumL[i] + sumR[n - (i + 1)];
		cout << val << endl;
	}
	return 0;
}