#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
struct PerfectMatchingTree {
public:
	PerfectMatchingTree(int n):n(n) {
		edge.resize(n);
		col.resize(n);
	}
	void addEdge(int u, int v) {
		edge[u].push_back(v);
		edge[v].push_back(u);
	}
	bool solve(int n) { return ((n + 1) / 2) == dfs(0); }
private:
	int n;
	std::vector<int>col;
	std::vector<std::vector<int>>edge;
	int dfs(int v, int d = 0, int p = -1) {
		col[v] = -1;
		int ret = 0;
		for (int e : edge[v]) {
			if (p == e) continue;
			ret += dfs(e, d + 1, v);
			if ((-1 == col[e]) && (-1 == col[v])) {
				col[e] = v; col[v] = v;
				ret++;
			}
		}
		return ret;
	}
};