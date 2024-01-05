#include <vector>
using  S = long long;
S op(S a, S b) { return a + b; }
S e() { return 0; }
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
	segtree<S, op, e>seg;
	void build() {
		dfs_sz(0);
		int time = 0;
		dfs_hld(0, time);
		initSegmentTree();
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
	void initSegmentTree() {
		std::vector<S>_val(n);
		for (int i = 0; i < n; ++i)_val[i] = val[rev[i]];
		seg = segtree<S, op, e>(_val);
	}
	S query(int u, int v) {
		S ret = e();
		for (;; v = par[head[v]]) {
			if (in[u] > in[v])std::swap(u, v);
			if (head[u] == head[v])break;
			auto get = seg.prod(in[head[v]], in[v] + 1);
			ret = op(ret, get);
		}
		auto get = seg.prod(in[u], in[v] + 1);
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
	// point update
	void update_point(int u, S x) {
		auto get = seg.get(in[u]);
		seg.set(in[u], get + 1);
	}
	void debug() {
		for (int i = 0; i < n; ++i) std::cout << seg.prod(i, i + 1) << " ";
		std::cout << std::endl;
	}
};