#include <vector>
#include <queue>
template<typename W>
// 区間に変を貼るテク
// xは単調性が必要
class RangeEdgeGraph {
private:
	int n, sz;
	struct edge { int to; W weight; };
	std::vector<std::vector<edge>> g;
	std::vector<std::pair<W, W>>lr;
	std::vector<W>wv;
public:
	RangeEdgeGraph(int n, std::vector<W>w = {}) : n(n), wv(w) {
		sz = 1;
		while (sz < n)sz *= 2;
		while (wv.size() < sz)wv.push_back(std::numeric_limits<W>::max());
		g.resize(5 * sz);
		lr.resize(2 * sz);
		for (int i = 0; i < sz; ++i) {
			lr[i + sz].first = wv[i];
			lr[i + sz].second = wv[i];
		}
		for (int i = sz - 1; 0 < i; --i) {
			lr[i].first = lr[i * 2 + 0].first;
			lr[i].second = lr[i * 2 + 1].second;
		}
		// gのindexは
		// 頂点,from-R、to-R,from-L,to-Lの順
		for (int v = 1; v < sz; ++v) {
			for (int w : {2 * v, 2 * v + 1}) {
				g[get_index(0, w)].push_back({ get_index(0, v), lr[v].second - lr[w].second });
				g[get_index(1, v)].push_back({ get_index(1, w), lr[w].first - lr[v].first });
				g[get_index(2, w)].push_back({ get_index(2, v), lr[w].first - lr[v].first });
				g[get_index(3, v)].push_back({ get_index(3, w), lr[v].second - lr[w].second });
			}
		}
	}

	// from [l1, r1) to [l2, r2)
	void add_edge(int index, int l1, int r1, int l2, int r2, W w) {
		g.push_back({});
		if (r1 <= l2) {
			for (int from : divide(l1, r1)) {
				g[get_index(0, from)].push_back({ get_index(4, index), wv[l2] - lr[from].second + w });
			}
			for (int to : divide(l2, r2)) {
				g[get_index(4, index)].push_back({ get_index(1, to), lr[to].first - wv[l2] });
			}
		}
		else {
			for (int from : divide(l1, r1)) {
				g[get_index(2, from)].push_back({ get_index(4, index), lr[from].first - wv[l1] + w });
			}
			for (int to : divide(l2, r2)) {
				g[get_index(4, index)].push_back({ get_index(3, to), wv[l1] - lr[to].second });
			}
		}
	}

	std::vector<W> dijkstra(int s = 0) {
		std::vector<W> dist(g.size(), std::numeric_limits<W>::max());
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
		std::vector<W> res(dist.begin(), dist.begin() + n);
		for (auto &e : res)if (e == std::numeric_limits<W>::max())e = -1;
		return res;
	}
private:
	int get_index(int type, int v) {
		if (type == 4) return 5 * sz + v;// 超頂点
		if (v >= sz) return v - sz;
		return v + (type + 1) * sz;
	}
	std::vector<int> divide(int l, int r) {
		std::vector<int> res;
		l += sz;
		r += sz;
		while (l < r) {
			if (l & 1) res.push_back(l++);
			if (r & 1) res.push_back(--r);
			l >>= 1;
			r >>= 1;
		}
		return res;
	}
};