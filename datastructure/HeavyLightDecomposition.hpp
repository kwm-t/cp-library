#include <vector>
template<class S, class F, template<class, class> class DataStruct>
struct HeavyLightDecomposition {
	HeavyLightDecomposition(std::vector<std::vector<int>> &_g, std::vector<S>&_val) :g(_g), val(_val) {
		n = g.size();
		sz.resize(n);
		par.resize(n);
		in.resize(n);
		out.resize(n);
		rev.resize(n);
		head.resize(n);
		deep.resize(n);
	}
	int n;
	std::vector<std::vector<int>>g;
	std::vector<int>sz, par, in, out, rev, head, deep;
	std::vector<S>val;
	DataStruct<S, F> dataStruct;
	void build() {
		dfs_sz(0);
		int time = 0;
		dfs_hld(0, time);
		initDataStruct();
	}
	void dfs_sz(int v, int d = 0, int p = -1) {
		deep[v] = d;
		par[v] = p;
		sz[v] = 1;
		if (g[v].size() && (p == g[v][0]))std::swap(g[v][0], g[v].back());
		for (auto &e : g[v]) {
			if (p == e)continue;
			dfs_sz(e, d + 1, v);
			sz[v] += sz[e];
			if (sz[g[v][0]] < sz[e])std::swap(g[v][0], e);
		}
	}
	void dfs_hld(int v, int &time, int p = -1) {
		in[v] = time;
		time++;
		rev[in[v]] = v;
		for (auto &e : g[v]) {
			if (p == e)continue;
			if (e == g[v][0])head[e] = head[v];
			else head[e] = e;
			dfs_hld(e, time, v);
		}
		out[v] = time;
	}
	void initDataStruct() {
		std::vector<S>_val(n);
		for (int i = 0; i < n; ++i)_val[i] = val[rev[i]];
		dataStruct = DataStruct<S,F>(_val);
	}
	S query(int v) {
		return dataStruct.prod(in[v], out[v]);
	}
	S query(int u, int v) {
		S ret = dataStruct.e();
		for (;; v = par[head[v]]) {
			if (in[u] > in[v])std::swap(u, v);
			if (head[u] == head[v])break;
			auto get = dataStruct.prod(in[head[v]], in[v] + 1);
			ret = op(ret, get);
		}
		auto get = dataStruct.prod(in[u], in[v] + 1);
		ret = op(ret, get);
		return ret;
	}
	int la(int v, int k) {
		while (1) {
			int u = head[v];
			if (in[v] - k >= in[u])return rev[in[v] - k];
			k -= in[v] - in[u] + 1;
			v = par[u];
		}
	}
	int lca(int u, int v) {
		for (;; v = par[head[v]]) {
			if (in[u] > in[v])std::swap(u, v);
			if (head[u] == head[v])return u;
		}
	}
	int dist(int u, int v) {
		return deep[u] + deep[v] - 2 * deep[lca(u, v)];
	}
	void set_point(int u, S x) {
		dataStruct.set(in[u], x);
	}
	void apply_point(int u, F x) {
		dataStruct.apply(in[u], x);
	}
	void apply_path(int u, int v, F x) {
		for (;; v = par[head[v]]) {
			if (in[u] > in[v])std::swap(u, v);
			if (head[u] == head[v])break;
			dataStruct.apply(in[head[v]], in[v] + 1, x);
		}
		dataStruct.apply(in[u], in[v] + 1, x);
	}
	void apply_subtree(int v, F x) {
		dataStruct.apply(in[v], out[v], x);
	}
	void debug() {
		for (int i = 0; i < n; ++i) std::cout << dataStruct.prod(i, i + 1) << " ";
		std::cout << std::endl;
	}
};
// example 
// https://atcoder.jp/contests/abc406/submissions/65942076
/*
template<class S, class F>
struct SegTree {
	SegTree() {}
	SegTree(const std::vector<S> data) {
		seg = segtree<S, op, e>(data);
	}
	S prod(int l, int r) {
		return seg.prod(l, r);
	}
	void set(int p, S x) {
		seg.set(p, x);
	}
	void apply(int p, F f) {
		seg.set(p, op(seg.get(p), f));
	}
	void apply(int l, int r, F f) {
		//seg.apply(l, r, f));
	}
	static S op(S a, S b) { return a + b; }
	static S e() { return 0; }
	atcoder::segtree<S, SegTree::op, SegTree::e>seg;
};*/