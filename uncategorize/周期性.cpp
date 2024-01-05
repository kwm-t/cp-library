#include <bits/stdc++.h>
//#include <atcoder/all>
using namespace std;
//using namespace atcoder;
//using mint = modint1000000007;
//const int mod = 1000000007;
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

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	long long k; cin >> k;
	vector<long long>a(n);
	rep(i, n)cin >> a[i];
	vector<long long>ans;
	ans.push_back(0);
	map<int, int>mp;
	mp[0] = 0;
	long long st, ed;
	while (true) {
		long long now = ans.back();
		now += a[now %n];
		ans.push_back(now);
		if (mp.count(now %n)) {
			st = mp[now%n];
			ed =  v.size();
			break;
		}
		mp[now % n] =  v.size();
	}
	long long cycle = ans[ed] - ans[st];
	if (k <= ed) {
		cout << ans[k] << endl;
	}
	else {
		long long fir = st;
		fir += (k - st) % (ed - st);
		long long val = ans[fir];
		val += cycle * ((k - fir) / (ed - st));
		cout << val << endl;
	}
	return 0;
}