#include "bits/stdc++.h"
#include "atcoder/all"
using namespace std;
using namespace atcoder;
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
#define P pair<long long,long long>
template<typename A, typename B> inline bool chmax(A & a, const B & b) { if (a < b) { a = b; return true; } return false; }
template<typename A, typename B> inline bool chmin(A & a, const B & b) { if (a > b) { a = b; return true; } return false; }
template< typename structure_t, typename get_t, typename update_t >
struct SegmentTree2DCompressed {
	//{矩形取得, 一点更新}
	// クエリ先読みpreupdateが必要
	using merge_f = function< get_t(get_t, get_t) >;
	using range_get_f = function< get_t(structure_t &, int, int) >;
	using update_f = function< void(structure_t &, int, update_t) >;

	int sz;
	vector< structure_t > seg;
	const merge_f f;
	const range_get_f g;
	const update_f h;
	const get_t identity;
	vector< vector< int > > LL, RR;
	vector< vector< int > > beet;

	SegmentTree2DCompressed(int n, const merge_f &f, const range_get_f &g, const update_f &h, const get_t &identity)
		: f(f), g(g), h(h), identity(identity) {
		sz = 1;
		while (sz < n) sz <<= 1;
		beet.resize(2 * sz);
		LL.resize(2 * sz);
		RR.resize(2 * sz);
	}

	void update(int a, int x, update_t z, int k, int l, int r) {
		if (r <= a || a + 1 <= l) return;
		if (a <= l && r <= a + 1) return h(seg[k], x, z);
		update(a, LL[k][x], z, 2 * k + 0, l, (l + r) >> 1);
		update(a, RR[k][x], z, 2 * k + 1, (l + r) >> 1, r);
		return h(seg[k], x, z);
	}

	void update(int x, int y, update_t z) {
		y = lower_bound(begin(beet[1]), end(beet[1]), y) - begin(beet[1]);
		return update(x, y, z, 1, 0, sz);
	}

	get_t query(int a, int b, int x, int y, int k, int l, int r) {
		if (a >= r || b <= l) return identity;
		if (a <= l && r <= b) return g(seg[k], x, y);
		return f(query(a, b, LL[k][x], LL[k][y], 2 * k + 0, l, (l + r) >> 1),
			query(a, b, RR[k][x], RR[k][y], 2 * k + 1, (l + r) >> 1, r));
	}

	get_t query(int a, int b, int x, int y) {
		x = lower_bound(begin(beet[1]), end(beet[1]), x) - begin(beet[1]);
		y = lower_bound(begin(beet[1]), end(beet[1]), y) - begin(beet[1]);
		return query(a, b, x, y, 1, 0, sz);
	}

	void build() {
		for (int k = (int)beet.size() - 1; k >= sz; k--) {
			sort(begin(beet[k]), end(beet[k]));
			beet[k].erase(unique(begin(beet[k]), end(beet[k])), end(beet[k]));
		}
		for (int k = sz - 1; k > 0; k--) {
			beet[k].resize(beet[2 * k + 0].size() + beet[2 * k + 1].size());
			merge(begin(beet[2 * k + 0]), end(beet[2 * k + 0]), begin(beet[2 * k + 1]), end(beet[2 * k + 1]), begin(beet[k]));
			beet[k].erase(unique(begin(beet[k]), end(beet[k])), end(beet[k]));
			LL[k].resize(beet[k].size() + 1);
			RR[k].resize(beet[k].size() + 1);
			int tail1 = 0, tail2 = 0;
			for (int i = 0; i < beet[k].size(); i++) {
				while (tail1 < beet[2 * k + 0].size() && beet[2 * k + 0][tail1] < beet[k][i]) ++tail1;
				while (tail2 < beet[2 * k + 1].size() && beet[2 * k + 1][tail2] < beet[k][i]) ++tail2;
				LL[k][i] = tail1, RR[k][i] = tail2;
			}
			LL[k][beet[k].size()] = (int)beet[2 * k + 0].size();
			RR[k][beet[k].size()] = (int)beet[2 * k + 1].size();
		}
		for (int k = 0; k < beet.size(); k++) {
			seg.emplace_back(structure_t(beet[k].size()));
		}
	}

	void preupdate(int x, int y) {
		beet[x + sz].push_back(y);
	}
};

long long calcS(long long a, long long b, long long c, long long d, long long e, long long f) {
	c -= a; d -= b;
	e -= a; f -= b;
	return abs(c*f - d * e);
}
long long op(long long a, long long b) { return std::max(a, b); }
long long e() { return -1; }
using tree1d = atcoder::segtree<long long, op, e>;
using tree2d = SegmentTree2DCompressed<tree1d, long long, long long>;
int main() {
	long long N, Q;
	cin >> N >> Q;
	vector<long long> t(Q), l(Q), r(Q), s(Q), fl(N), fr(N), fs(N);
	rep(i, N) {
		int a, b, c, d, e, f;
		cin >> a >> b >> c >> d >> e >> f;
		fl[i] = min({ a,c,e });
		fr[i] = max({ a,c,e });
		fs[i] = calcS(a, b, c, d, e, f);
	}
	rep(i, Q) {
		cin >> t[i];
		if (t[i] == 1) {
			int a, b, c, d, e, f;
			cin >> a >> b >> c >> d >> e >> f;
			l[i] = min({ a,c,e });
			r[i] = max({ a,c,e });
			s[i] = calcS(a, b, c, d, e, f);
		}
		else cin >> l[i] >> r[i];
	}
	vector<long long> za;
	for (auto& x : l)za.emplace_back(x);
	for (auto& x : r)za.emplace_back(x);
	for (auto& x : fl)za.emplace_back(x);
	for (auto& x : fr)za.emplace_back(x);
	sort(all(za));
	za.erase(unique(all(za)), za.end());
	tree2d tree(za.size(), [](long long a, long long b) {return max(a, b); }, [](tree1d& x, int a, int b) {return x.prod(a, b); }, [](tree1d& x, int a, long long b) {x.set(a, b); }, -1);
	for (auto& x : l)x = lower_bound(all(za), x) - za.begin();
	for (auto& x : r)x = lower_bound(all(za), x) - za.begin();
	for (auto& x : fl)x = lower_bound(all(za), x) - za.begin();
	for (auto& x : fr)x = lower_bound(all(za), x) - za.begin();
	rep(i, N)tree.preupdate(fl[i], fr[i]);
	rep(i, Q)tree.preupdate(l[i], r[i]);
	tree.build();
	rep(i, N)tree.update(fl[i], fr[i], fs[i]);
	rep(i, Q) {
		if (t[i] == 1)tree.update(l[i], r[i], s[i]);
		else cout << tree.query(l[i], r[i] + 1, l[i], r[i] + 1) << endl;
	}
}