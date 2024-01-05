#include <vector>
#include <algorithm>
class Tree {
public:
	Tree(int n, int root) : n(n), root(root) {
		edge.resize(n);
		for (int i = 0; i < MAXLOGV; i++) parent[i].resize(n);
		depth.resize(n);
	}
	// uとvをつなぐ
	// lcaを求めることが主目的なので無向グラフとしている
	void unite(int u, int v) {
		edge[u].push_back(v);
		edge[v].push_back(u);
	}
	// initする
	// コンストラクタだけじゃなくてこれも呼ばないとlcaが求められないぞ
	void init() {
		dfs(root, -1, 0);
		for (int k = 0; k + 1 < MAXLOGV; k++) {
			for (int v = 0; v < n; v++) {
				if (parent[k][v] < 0) parent[k + 1][v] = -1;
				else parent[k + 1][v] = parent[k][parent[k][v]];
			}
		}
	}
	// uとvのlcaを求める
	int lca(int u, int v) const {
		if (depth[u] > depth[v]) std::swap(u, v);
		for (int k = 0; k < MAXLOGV; k++) {
			if ((depth[v] - depth[u]) >> k & 1) {
				v = parent[k][v];
			}
		}
		if (u == v) return u;
		for (int k = MAXLOGV - 1; k >= 0; k--) {
			if (parent[k][u] != parent[k][v]) {
				u = parent[k][u];
				v = parent[k][v];
			}
		}
		return parent[0][u];
	}
	// uのn個親を求める
	int pare(int v, int n) {
		if (depth[v] < n)return -1;
		n = std::min(n, depth[v]);
		int idx = MAXLOGV;
		while (n) {
			for (int i = idx - 1; i >= 0; --i) {
				if (n < (1 << i))continue;
				if (-1 == parent[i][v])continue;
				n -= (1 << i);
				v = parent[i][v];
				idx = i;
				break;
			}
		}
		return v;
	}
	// uからvに向かってd進んだ頂点を返す
	int JumpOnTree(int u, int v, int d) {
		if (0 == d)return u;
		int distuv = dist(u, v);
		if (distuv < d)return -1;
		int l = lca(u, v);
		if (l == u)return pare(v, distuv - d);
		if (l == v)return pare(u, d);
		int distlu = dist(l, u);
		if (distlu >= d)return pare(u, d);
		return pare(v, distuv - d);
	}
	// uとvの距離を求める
	// edgeを定義しないといけない時はこれじゃダメ
	int dist(int u, int v) const {
		int p = lca(u, v);
		return (depth[u] - depth[p]) + (depth[v] - depth[p]);
	}
	//頂点wが頂点u,vのパス上に存在するか
	bool on_path(int u, int v, int w) {
		return (dist(u, w) + dist(v, w) == dist(u, v));
	}
	int dfs(int v, int p, int d) {
		int ret = 1;
		parent[0][v] = p;
		depth[v] = d;
		for (int next : edge[v]) {
			if (next == p) continue;
			auto get = dfs(next, v, d + 1);
			ret += get;
		}
		return ret;
	}
	static const int MAXLOGV = 25;
	// グラフの隣接リスト表現
	std::vector<std::vector<int>>edge;
	// 頂点の数
	int n;
	// 根ノードの番号
	int root;
	// 親ノード
	std::vector<int> parent[MAXLOGV];
	// 根からの深さ
	std::vector<int> depth;
};