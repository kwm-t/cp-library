#include <vector>
const long long LINF = 1e18;

template<typename T>
class Bellman_ford {
public:
	Bellman_ford(int n, bool longestRoot = false) :n(n), longestRoot(longestRoot) {}
	void addEdge(int v, int w, T c) {
		g.push_back({ v, w, c *(longestRoot ? -1 : 1) });
	}
	std::vector<T> solve() {
		std::vector<T>d(n, LINF);
		d[0] = 0;
		// 通常更新パート
		// n-1回最短経路問題を解くと負閉路がない場合の更新が終わる
		for (int i = 0; i < n - 1; ++i) {
			for (int j = 0; j < (int)g.size(); ++j) {
				if (LINF == d[g[j].from]) continue;
				if (d[g[j].from] + g[j].cost < d[g[j].to]) d[g[j].to] = d[g[j].from] + g[j].cost;
			}
		}
		// 閉路の伝達パート
		std::vector<bool>update(n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < g.size(); ++j) {
				if (LINF == d[g[j].from]) continue;
				if (d[g[j].from] + g[j].cost < d[g[j].to]) update[g[j].to] = true;
				if (update[g[j].from]) update[g[j].to] = true;
			}
		}
		// 負の閉路が存在するなら更新
		for (int i = 0; i < n; ++i) {
			if (update[i])d[i] = -LINF;
			if (longestRoot) d[i] *= -1;
		}
		return d;
	}
private:
	struct Edge {
		int from, to;
		T cost;
		Edge(int _from, int _to, T _cost) :from(_from), to(_to), cost(_cost) {}
	};
	int n;
	bool longestRoot;
	std::vector<Edge>g;
};