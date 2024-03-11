#include <vector>
#include <deque>
class bfs01 {
public:
	bfs01(int sz) :sz(sz) {
		edge.resize(sz);
		dp.resize(sz, INF);
	}
	void addEdge(const int v, const  int w, const  int c) {
		edge[v].emplace_back(w, c);
		edge[w].emplace_back(v, c);
	}
	int solve(const int st, const  int ed) {
		std::deque<int> q;
		dp[st] = 0;
		q.push_back(st);
		while (!q.empty()) {
			int v = q.front();
			q.pop_front();
			for (auto e : edge[v]) {
				int nv = e.to;
				if (0 == e.cost) {//0
					if (dp[v] < dp[nv]) {
						dp[nv] = dp[v];
						q.push_front(nv);
					}
				}
				else {//1
					if (dp[v] + 1 < dp[nv]) {
						dp[nv] = dp[v] + 1;
						q.push_back(nv);
					}
				}
			}
		}
		return dp[ed];
	}
private:
	struct Edge {
		int to;
		int cost;
		Edge(int _to, int _cost) :to(_to), cost(_cost) {}
	};
	int sz;
	std::vector<std::vector<Edge>>edge;
	std::vector<int>dp;
};