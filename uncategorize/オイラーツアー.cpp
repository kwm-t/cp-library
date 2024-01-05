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
vector<int>ans;
vector<int>to[200005];
int in[200005],out[200005];//[in,out)
int id;
int dfs(int v, int p = -1) {
	int ret = 1;
	in[v] = id;
	id++;
	for (int e : to[v]) {
		if (p == e) continue;
		ret += dfs(e, v);
	}
	out[v] = in[v] + ret;
	return ret;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, q; cin >> n >> q;
	x.resize(n);
	ans.resize(n);
	rep(i, n)cin >> x[i];
	rep(i, n - 1) {
		int a, b; cin >> a >> b; a--; b--;
		to[a].push_back(b);
		to[b].push_back(a);
	}
	id = 0;
	dfs(0);
	return 0;
}