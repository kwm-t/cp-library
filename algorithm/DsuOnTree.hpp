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

	template <typename UPDATE, typename QUERY, typename CLEAR>
	void run(UPDATE &update, QUERY &query, CLEAR &clear) {
		auto dsu = [&](auto &&self, int v, int p, bool keep)->void {
			// un heavy path
			for (int i = 1; i < (int)g[v].size(); ++i) {
				if (g[v][i] == p) continue;
				self(self, g[v][i], v, false);
			}
			// not leaf
			if (sub_size[v] != 1) {
				self(self, g[v][0], v, true);
				for (int i = up[g[v][0]]; i < up[v]; i++) {
					update(v, euler[i]);
				}
			}
			update(v, v);
			query(v);
			if (!keep) {
				for (int i = down[v]; i < up[v]; i++) clear(euler[i]);
			}
		};
		dsu(dsu, root, -1, true);
	}

	using F = function<void(int)>;
	using F2 = function<void(int, int)>;
	void run_every_pair(F update, F query_root, F2 query_light, F clear) {
		auto dfs = [&](auto &&self, int v, int p, bool keep) -> void {
			// un heavy path
			for (int i = 1; i < (int)g[v].size(); i++) {
				if (g[v][i] == p) continue;
				self(self, g[v][i], v, false);
			}

			// not leaf
			if (sub_size[v] != 1) {
				self(self, g[v][0], v, true);
			}

			// update Small Child
			if (sub_size[v] != 1) {
				for (int i = 1; i < g[v].size(); i++) {
					if (g[v][i] == p) continue;

					int ch = g[v][i];
					// Big Child とすでに処理済みの Small Child に関するクエリを処理
					for (int u = down[ch]; u < up[ch]; u++) query_light(v, euler[u]);
					for (int u = down[ch]; u < up[ch]; u++) update(euler[u]);
				}
			}

			query_root(v);
			update(v);

			if (!keep) {
				for (int i = down[v]; i < up[v]; i++) clear(euler[i]);
			}
			return;
		};

		dfs(dfs, root, -1, true);
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
// sumple
// https://yukicoder.me/submissions/1040215
// run_every_pairのquery_rootは別処理がいる？