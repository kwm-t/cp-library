struct LCA {
	int n;
	std::vector<int> idx;
	struct S {
		int v, depth;
		S() {}
		constexpr S(int v, int depth) : v(v), depth(depth) {}
	};
	std::vector<std::vector<S>> spt;
	static constexpr S min(S x, S y) { return x.depth < y.depth ? x : y; }
	LCA(const std::vector<std::vector<int>>& g, int root) : n(g.size()), idx(n, -1) {
		assert(0 <= root && root < n);
		std::vector<S> tour;
		auto dfs = [&](auto&& self, int u, int d) -> void {
			idx[u] = tour.size();
			tour.emplace_back(u, d);
			for (int v : g[u]) if (idx[v] == -1) {
				self(self, v, d + 1);
				tour.emplace_back(u, d);
			}
		};
		dfs(dfs, root, 0);
		int m = tour.size();
		int w = _bit_width(m + 0U);
		spt.resize(w);
		spt[0] = move(tour);
		for (int k = 0; k + 1 < w; k++) {
			int k2 = 1 << k;
			int sz = m - 2 * k2 + 1;
			spt[k + 1].resize(sz);
			for (int i = 0; i < sz; i++) spt[k + 1][i] = min(spt[k][i], spt[k][i + k2]);
		}
	}
	int lca(int u, int v) {
		int l = idx[u], r = idx[v];
		if (l > r) std::swap(l, r);
		int k = _bit_width(r - l + 1U) - 1;
		return min(spt[k][l], spt[k][r + 1 - (1 << k)]).v;
	}
	static int _bit_width(unsigned int x) noexcept {
#ifdef _MSC_VER
		if (x == 0) return 0;
		unsigned long index;
		_BitScanReverse(&index, x);
		return static_cast<int>(index) + 1;
#else
		return std::bit_width(x); // C++20～
#endif
	}
};