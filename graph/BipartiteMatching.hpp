#include<iostream>
#include <vector>
/*
	二部グラフの最大マッチング = 最小頂点被覆
	・最小頂点被覆:=すべての辺が選択した頂点から伸びているように頂点を選ぶ際の最小個数
*/
struct BipartiteMatching {
	std::vector< std::vector< int > > graph;
	std::vector< int > match, alive, used;
	int timestamp;

	BipartiteMatching(int n) : graph(n), alive(n, 1), used(n, 0), match(n, -1), timestamp(0) {}

	void add_edge(int u, int v) {
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	bool dfs(int idx) {
		used[idx] = timestamp;
		for (auto &to : graph[idx]) {
			int to_match = match[to];
			if (alive[to] == 0) continue;
			if (to_match == -1 || (used[to_match] != timestamp && dfs(to_match))) {
				match[idx] = to;
				match[to] = idx;
				return true;
			}
		}
		return false;
	}

	int bipartite_matching() {
		int ret = 0;
		for (int i = 0; i < graph.size(); i++) {
			if (alive[i] == 0) continue;
			if (match[i] == -1) {
				++timestamp;
				ret += dfs(i);
			}
		}
		return ret;
	}

	void output() {
		for (int i = 0; i < graph.size(); i++) {
			if (i < match[i]) {
				std::cout << i << "-" << match[i] << std::endl;
			}
		}
	}
};