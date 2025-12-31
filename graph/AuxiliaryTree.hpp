class AuxiliaryTree {
public:
	AuxiliaryTree(std::vector<std::vector<int>> g) :n(g.size()), g(g) {
	}
	void init() {
		int cur = 0;
		fs.resize(n);
		ls.resize(n);
		st.assign(NL, std::vector<int>(n * 3));
		depth.resize(n);

		dfs(0, cur);
		logInit(cur);
		sparseTableInit(cur);
	}
	// rootと圧縮木を返す(ただし圧縮木はrootからの有向木)
	int createCompressionTree(std::vector<int>&vs, std::vector<std::vector<int>> &graph) {
		std::sort(vs.begin(), vs.end(), [&](int l, int r) {return fs[l] < fs[r]; });
		int sz = vs.size();
		for (int i = 0; i < sz - 1; ++i) {
			vs.push_back(lca(vs[i], vs[i + 1]));
		}
		std::sort(vs.begin(), vs.end(), [&](int l, int r) {return fs[l] < fs[r]; });
		int prv = -1;
		int cur = 0;
		std::vector<int>stk(vs.size());
		for (int i = 0; i < vs.size(); ++i) {
			int v = vs[i];
			if (prv == v)continue;
			while (cur > 0 && ls[stk[cur - 1]] < fs[v]) --cur;
			if (cur > 0)graph[stk[cur - 1]].push_back(v);
			graph[v].clear();
			stk[cur++] = v;
			prv = v;
		}
		return stk[0];
	}
	int dist(int u, int v) {
		int l = lca(u, v);
		return depth[u] + depth[v] - 2 * depth[l];
	}
private:
	const int NL = 19;
	int n;
	std::vector<std::vector<int>> g;
	// fs[v]: Euler tour上の頂点vの最初の出現位置
	// lS[v]: Euler tour上の頂点vの最後の出現位置
	std::vector<int>fs, ls;
	std::vector<std::vector<int>> st;// st[lv][v]: Sparse Table
	std::vector<int>depth;
	std::vector<int>log;

	// Euler Tour Dfs
	void dfs(int v, int &cur, int p = -1, int d = 0) {
		st[0][fs[v] = cur++] = v;
		depth[v] = d;
		for (auto nv : g[v]) {
			if (p == nv)continue;
			dfs(nv, cur, v, d + 1);
			st[0][cur++] = v;
		}
		ls[v] = cur - 1;
	}
	void logInit(int size) {
		log.resize(size + 1);
		log[0] = log[1] = 0;
		for (int i = 2; i <= size; ++i)log[i] = log[i >> 1] + 1;
	}
	void sparseTableInit(int size) {
		for (int i = 0, b = 1; i < log[size]; ++i, b <<= 1) {
			for (int j = 0; j < (size - (b << 1) + 1); ++j) {
				st[i + 1][j] = (depth[st[i][j]] <= depth[st[i][j + b]] ? st[i][j] : st[i][j + b]);
			}
		}
	}
	int lca(int u, int v) {
		int x = fs[u], y = fs[v];
		if (x > y) std::swap(x, y);
		int l = log[y - x + 1];
		return (depth[st[l][x]] <= depth[st[l][y - (1 << l) + 1]] ? st[l][x] : st[l][y - (1 << l) + 1]);
	}
};