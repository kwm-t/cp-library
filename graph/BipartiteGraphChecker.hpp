#include <vector>
/*
	“ñ•”ƒOƒ‰ƒt”»’è
*/
struct BipartiteGraphChecker {

	BipartiteGraphChecker(int n) :n(n){
		col.assign(n, -1);
		graph.resize(n);
	}

	void add_edge(int u, int v) {
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	int n;
	std::vector<std::vector< int >> graph;
	std::vector<int>col;

	bool solve(int  v, int p = -1) {
		if (-1 == p)col[v] = 0;
		bool ret = true;
		for (auto e : graph[v]) {
			if (-1 != col[e]) {
				if (1 != col[e] + col[v])ret = false;
			}
			else {
				col[e] = 1 - col[v];
				ret &= solve(e, v);
			}
		}
		return ret;
	}
	bool solverAll() {
		bool chk = true;
		for (int i = 0; i < n; ++i) {
			if (-1 != col[i])continue;
			auto get = solve(i);
			if (!get)chk = false;
		}
		return chk;
	}
};