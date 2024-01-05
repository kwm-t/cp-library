template<typename T>
struct bfs01 {
	struct Edge {
		int to;
		T cost;
		Edge(int _to, T _cost) :to(_to), cost(_cost) {}
	};
	int sz;
	std::vector<std::vector<Edge>>edge;
	std::vector<T>dp;
	bfs01(int sz) :sz(sz) {
		edge.resize(sz);
		dp.resize(sz, INF);
	}
	void addEdge(int v, int w, T c) {
		edge[v].emplace_back(w, c);
		edge[w].emplace_back(v, c);
	}
	void solve(int x) {
		std::deque<int> q;
		dp[0] = 0;
		q.push_back(0);
		while (!q.empty()) {
			int v = q.front();
			q.pop_front();
			for (auto e : edge[v]) {
				int nv = e.to;
				if (nv.cost <= x) {//0
					if (chmin(dp[nv], dp[v])) q.push_front(nv);
				}
				else {//1
					if (chmin(dp[nv], dp[v] + 1)) q.push_back(nv);
				}
			}
		}
	}
};