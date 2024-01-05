#include <queue>
#include <vector>
template<typename W>
// ‹æŠÔ‚É•Ï‚ð“\‚éƒeƒN
struct range_edge_graph {
	int n;
	struct edge { int to; W weight; };
	std::vector<std::vector<edge>> g;

	range_edge_graph(int n) : n(n) {
		g.resize(4 * n);
		for (int i = 1; i < n; ++i) {
			int cl = i << 1 | 0, cr = i << 1 | 1;
			g[i].push_back({ cl, 0 });
			g[i].push_back({ cr, 0 });
			g[cl + 2 * n].push_back({ i + 2 * n, 0 });
			g[cr + 2 * n].push_back({ i + 2 * n, 0 });
		}
		for (int i = n; i < 2 * n; ++i) g[i].push_back({ i + 2 * n, 0 });
	}

	// from [l1, r1) to [l2, r2)
	void add_edge(int l1, int r1, int l2, int r2, W w) {
		int idx = g.size();
		for (l1 += n, r1 += n; l1 < r1; l1 >>= 1, r1 >>= 1) {
			if (l1 & 1) g[l1 + 2 * n].push_back({ idx, 0 }), l1++;
			if (r1 & 1) --r1, g[r1 + 2 * n].push_back({ idx, 0 });
		}
		std::vector<edge> node;
		for (l2 += n, r2 += n; l2 < r2; l2 >>= 1, r2 >>= 1) {
			if (l2 & 1) node.push_back({ l2++, w });
			if (r2 & 1) node.push_back({ --r2, w });
		}
		g.push_back(node);
	}

	std::vector<W> dijkstra(int s) {
		s += n;
		std::vector<W> dist(g.size(), numeric_limits<W>::max());
		dist[s] = 0;
		std::priority_queue<std::pair<W, int>, std::vector<std::pair<W, int>>, std::greater<std::pair<W, int>>> que;
		que.emplace(0, s);
		while (!que.empty()) {
			auto p = que.top();
			que.pop();
			int v = p.second;
			if (dist[v] < p.first) continue;
			for (edge& e : g[v]) {
				if (dist[e.to] > dist[v] + e.weight) {
					dist[e.to] = dist[v] + e.weight;
					que.emplace(dist[e.to], e.to);
				}
			}
		}
		std::vector<W> res(dist.begin() + n, dist.begin() + 2 * n);
		return res;
	}
};