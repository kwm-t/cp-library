#include <bits/stdc++.h>
//#include <atcoder/all>
using namespace std;
//using namespace atcoder;
//using mint = modint1000000007;
//const int mod = 1000000007;
//using mint = modint998244353;
//const int mod = 998244353;
const int INF = 1e9;
const long long LINF = 1e18;
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
#define def 0 //todo
using V = long long;
V comp(V& l, V& r) { return max(l, r); };//todo
struct SegTree { //[l,r)
	int NV;
	vector<V> val;
	void init(int n) {
		NV = 1;
		while (NV < n) NV *= 2;
		val = vector<V>(NV * 2, def);
	}
	V get(int x, int y, int l, int r, int k) {
		if (r <= x || y <= l) return def; if (x <= l && r <= y)return val[k];
		auto a = get(x, y, l, (l + r) / 2, k * 2); auto b = get(x, y, (l + r) / 2, r, k * 2 + 1); return comp(a, b);
	}
	V get(int x, int y) { return get(x, y, 0, NV, 1); }
	void update(int i, V v) { i += NV; val[i] = v; while (i > 1)i >>= 1, val[i] = comp(val[i * 2], val[i * 2 + 1]); }
	void add(int i, V v) { update(i, max(val[i + NV], v)); }//todo
	V operator[](int x) { return get(x, x + 1); }
};
struct Healthy2DSegTree {
	int NV;
	vector<SegTree> st;
	vector<vector<int>> index;

	void init(vector<vector<int>> &v) {
		int n = v.size();
		NV = 1; while (NV < n) NV *= 2;
		index.resize(2 * NV);
		rep(i, n) for (auto &j : v[i]) index[i + NV].push_back(j);
		rrep2(i, 1, NV * 2) {
			sort(index[i].begin(), index[i].end());
			index[i].erase(unique(index[i].begin(), index[i].end()), index[i].end());
			for (auto &j : index[i]) index[i / 2].push_back(j);
		}
		st.resize(2 * NV);
		rep2(i, 1, NV * 2) st[i].init(index[i].size());
	}
	void update(int x, int y, V v) {
		assert(x < NV);
		x += NV;
		while (x) {
			int yy = lower_bound(index[x].begin(), index[x].end(), y) - index[x].begin();
			assert(yy != index[x].size());
			assert(y == index[x][yy]);
			st[x].update(yy, v);
			x >>= 1;
		}
	}
	void add(int x, int y, V v) {
		assert(x < NV);
		x += NV;
		while (x) {
			int yy = lower_bound(index[x].begin(), index[x].end(), y) - index[x].begin();
			assert(yy != index[x].size());
			assert(y == index[x][yy]);
			st[x].add(yy, v);
			x >>= 1;
		}
	}
	V get(int sx, int tx, int sy, int ty, int k, int l, int r) {
		assert(k < NV * 2);
		assert(l < r);
		if (r <= sx or tx <= l) return def;
		if (sx <= l and r <= tx) {
			int syy = lower_bound(index[k].begin(), index[k].end(), sy) - index[k].begin();
			int tyy = lower_bound(index[k].begin(), index[k].end(), ty) - index[k].begin();
			return st[k].get(syy, tyy);
		}
		int md = (l + r) / 2;
		V le = get(sx, tx, sy, ty, k * 2, l, md);
		V ri = get(sx, tx, sy, ty, k * 2 + 1, md, r);
		return comp(le, ri);
	}
	V get(int sx, int tx, int sy, int ty) {
		return get(sx, tx, sy, ty, 1, 0, NV);
	}
};
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	vector<array<int, 4>>data;
	vector<int>p = { 0 }, q = { 0 };
	rep(i, n) {
		int t, x, y, a;
		cin >> t >> x >> y >> a;
		if ((t >= x + y) && (t >= y - x)) {
			data.push_back({ y,t - x - y,t + x - y,a });
			p.push_back(t - x - y);
			q.push_back(t + x - y);
		}
	}
	sort(all(data));
	sort(all(p));
	p.erase(unique(all(p)), p.end());
	sort(all(q));
	q.erase(unique(all(q)), q.end());

	vector<vector<int>>v(p.size());
	rep(i, data.size()) {
		data[i][1] = lower_bound(all(p), data[i][1]) - p.begin();
		data[i][2] = lower_bound(all(q), data[i][2]) - q.begin();
		v[data[i][1]].push_back({ data[i][2] });
	}
	Healthy2DSegTree st;
	st.init(v);
	long long ans = 0;
	for (auto &d : data) {
		auto[y, p, q, a] = d;
		long long get = st.get(0, p + 1, 0, q + 1);
		chmax(ans, get + a);
		st.add(p, q, get + a);
	}
	cout << ans << endl;
	return 0;
}