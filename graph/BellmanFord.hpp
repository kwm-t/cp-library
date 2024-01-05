#include <vector>
const long long LINF = 1e18;

template<typename T>
struct Bellman_ford {
	struct Edge {
		int from, to;
		T cost;
		Edge(int _from, int _to, T _cost) :from(_from), to(_to), cost(_cost) {}
	};
	Bellman_ford(int n) :n(n) {
	}
	int n;
	std::vector<Edge>g;
	void addEdge(int v, int w, T c) {
		g.push_back({ v, w, c });
	}
	T solve() {
		std::vector<T>d(n, -LINF);
		d[0] = 0;
		//通常更新パート
		for (int i = 0; i < n - 1; ++i) {
			for (int j = 0; j < (int)g.size(); ++j) {
				if (-LINF == d[g[j].from]) continue;
				if (d[g[j].to] < (d[g[j].from] + g[j].cost)) d[g[j].to] = d[g[j].from] + g[j].cost;
			}
		}
		//閉路の伝達パート
		std::vector<bool>update(n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < g.size(); ++j) {
				if (-LINF == d[g[j].from]) continue;
				if (d[g[j].to] < (d[g[j].from] + g[j].cost)) update[g[j].to] = true;
				if (update[g[j].from]) update[g[j].to] = true;
			}
		}
		//正の閉路が存在する
		T ans = -LINF;
		for (int i = 1; i < n; ++i) {
			if (!update[i]) chmax(ans, d[i]);
			else chmax(ans, LINF);
		}
		return ans;
	}
};