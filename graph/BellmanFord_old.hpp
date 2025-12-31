#include <vector>
const int INF = 1e9;

template<typename T>
struct Bellman_ford {
	struct Edge {
		int from, to;
		T cost;
		Edge(int _from, int _to, T _cost) :from(_from), to(_to), cost(_cost) {}
	};
	Bellman_ford(int n) :n(n), m(m) {
	}
	int n, m;
	std::vector<Edge>g;
	void addEdge(int v, int w, T c) {
		g.push_back({ v, w, -c });
	}
	T solve() {
		std::vector<T>d(n, INF);
		d[0] = 0;
		for (int i = 0; i < 2 * n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (d[g[j].from] != INF && d[g[j].to] > d[g[j].from] + g[j].cost) {
					d[g[j].to] = d[g[j].from] + g[j].cost;
					if ((i >= n - 1) && (g[j].to == n - 1)) {
						//閉路の検出
						return -1;
					}
				}
			}
		}
		return  max(0, -d[n - 1]);
	}
};