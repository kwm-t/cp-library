template<typename G>
class DsuOnTree {
public:
	DsuOnTree(G &g, int root = 0)
		:g(g),
		n(g.size()),
		sub_size(g.size()),
		euler(g.size()),
		down(g.size()),
		up(g.size()),
		idx_(0),
		root(root) {
		dfs1(root);
		dfs2(root);
	}

	template <typename UPDATE, typename QUERY, typename CLEAR, typename RESET>
	void run(UPDATE &update, QUERY &query, CLEAR &clear, RESET &reset) {
		auto dsu = [&](auto self, int v, int p = -1, bool keep = false)->void {
			// un heavy path
			for (int i = 1; i < (int)g[v].size(); ++i) {
				if (p == g[v][i])continue;
				self(self, g[v][i], v, false);
			}
			// not leaf
			if (sub_size[v] != 1) {
				self(self, g[v][0], v, true);
				for (int i = up[g[v][0]]; i < up[v]; i++) {
					update(euler[i]);
				}
			}
			update(v);
			query(v);
			if (!keep) {
				for (int i = down[v]; i < up[v]; i++) clear(euler[i]);
				reset(v);
			}
		};
		dsu(dsu, root);
	}
private:
	G &g;
	int n;
	std::vector<int>sub_size, euler, down, up;
	int idx_;
	int root;

	int dfs1(int v, int p = -1) {
		sub_size[v] = 1;
		// 先頭にheavy pathを
		if ((int)g[v].size() >= 2 && g[v][0] == p) {
			std::swap(g[v][0], g[v][1]);
		}
		for (auto &e : g[v]) {
			if (e == p)continue;
			sub_size[v] += dfs1(e, v);
			if (sub_size[e] > sub_size[g[v][0]]) {
				std::swap(g[v][0], e);
			}
		}
		return sub_size[v];
	}
	void dfs2(int v, int p = -1) {
		euler[idx_] = v;
		down[v] = idx_++;
		for (auto &e : g[v]) {
			if (e == p)continue;
			dfs2(e, v);
		}
		up[v] = idx_;
	}
};