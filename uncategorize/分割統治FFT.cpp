#include "bits/stdc++.h"
#include "atcoder/all"
using namespace std;
using namespace atcoder;
//using mint = modint1000000007;
//const int mod = 1000000007;
using mint = modint998244353;
const int mod = 998244353;
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
vector<P> es;
vector<vector<mint>> ps;
vector<vector<mint>> dp;
void dfs(int l, int r) {
	if (r - l <= 1)return;
	int c = (l + r) / 2;
	dfs(l, c);
	rep(i, es.size()) {
		auto[a, b] = es[i];
		rep(j, 2) {
			vector<mint>x(dp[a].begin() + l, dp[a].begin() + c);
			vector<mint>y(ps[i].begin(), ps[i].begin() + (r - l));
			auto z = convolution(x, y);
			rep2(k, c, r) dp[b][k] += z[k - l];
			swap(a, b);
		}
	}
	dfs(c, r);
}
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, m, t; cin >> n >> m >> t;
	rep(i, m) {
		int a, b; cin >> a >> b; a--; b--;
		es.emplace_back(a, b);
		vector<mint>p(t + 1);
		rep(j, t) {
			int x; cin >> x;
			p[j + 1] = x;
		}
		ps.push_back(p);
	}
	dp.resize(n, vector<mint>(t + 1));
	dp[0][0] = 1;
	dfs(0, t + 1);
	cout << dp[0][t].val() << endl;
	return 0;
}