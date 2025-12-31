#include <vector>
// 重心分解
class CentroidDecomposition {
private:
	int V;
	std::vector<std::vector<int> > G;
	std::vector<bool> used;
	//sz:重心分解後の最大部分木に含まれる頂点の数(自分を含める)
	//par:重心分解後の親の頂点
	std::vector<int> sz, par;
	//部分木のサイズを計算
	void calcSize(int u, int p) {
		sz[u] = 1;
		for (int v : G[u]) {
			if (!used[v] && v != p) {
				calcSize(v, u);
				sz[u] += sz[v];
			}
		}
	}
	int cdBuild(int u, int p) {
		calcSize(u, -1);
		int tot = sz[u];
		bool ok = false;
		int pp = -1;
		//いま見ている部分木での重心を見つける
		while (!ok) {
			ok = true;
			for (int v : G[u]) {
				if (!used[v] && v != pp && 2 * sz[v] > tot) {
					pp = u, u = v, ok = false;
					break;
				}
			}
		}
		par[u] = p;
		//何らかの操作
		used[u] = true;
		//深さ優先でたどる
		for (int v : G[u]) {
			if (!used[v]) {
				cdBuild(v, u);
			}
		}
		return u;
	}
public:
	CentroidDecomposition(int node_size) : V(node_size), G(V), used(V, false)
		, sz(V, 0), par(V, -1) {}
	void add_edge(int u, int v) {
		G[u].push_back(v), G[v].push_back(u);
	}
	int build() {
		return cdBuild(0, -1);
	}
};