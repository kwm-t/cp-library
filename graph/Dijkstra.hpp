#include <queue>
#include <vector>
template<typename T>
struct Dijkstra {
private:
	int n;
	struct Edge {
		int to;
		T cost;
	};
	std::vector<std::vector<Edge>> g;
public:
	const T INF = numeric_limits<T>::max();
	std::vector<T> dp;
	Dijkstra() {}
	Dijkstra(int n) : n(n) {
		g.resize(n);
	}
	Dijkstra<T>& operator=(const Dijkstra<T>& obj) {
		this->v = obj.v;
		this->g = obj.g;
		this->dp = obj.dp;
		return *this;
	}
	void add_edge(int from, int to, T weight, bool twoWay = false) {
		g[from].push_back({ to,weight });
		if (twoWay) g[to].push_back({ from,weight });
	}
	int add_vertex() {
		g.push_back(std::vector<Edge>());
		return n++;
	}
	void build(int s) {
		dp.assign(n, INF);
		std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> pq;
		dp[s] = 0;
		pq.push({ dp[s], s });
		while (!pq.empty()) {
			auto p = pq.top();
			pq.pop();
			T cost = p.first;
			int v = p.second;
			if (dp[v] < cost) continue;
			for (const auto &e : g[v]) {
				if (dp[e.to] > dp[v] + e.cost) {
					dp[e.to] = dp[v] + e.cost;
					pq.push({ dp[e.to], e.to });
				}
			}
		}
	}
	std::vector<int> restore(int s, int t) {
		// restore‚Ìs‚Íbuild‚Ìs‚Æˆê’v‚µ‚Ä‚¢‚é•K—v‚ª‚ ‚é
		std::vector<int>route = { t };
		while (s != t) {
			for (const auto & e : gR[t]) {
				if (dp[e.to] + e.cost  == dp[t]) {
					t = e.to;
					route.push_back(t);
					break;
				}
			}
		}
		std::reverse(route.begin(), route.end());
		return route;
	}
	T get(int t) {
		if (INF == dp[t])return -1;
		else return dp[t];
	}
};