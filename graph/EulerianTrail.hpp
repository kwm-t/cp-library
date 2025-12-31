#include <vector>
// graph[from][i]={to, edge_index}
// 多重辺を重複なく持ちたければedge_indexではなく、edge_countにしても良い
// https://atcoder.jp/contests/codequeen2025-final-Public/submissions/68098334
// https://perogram.hateblo.jp/entry/2021/04/29/002042
std::vector<int> eulerianTrail(const std::vector<std::vector<pair<int, int>>> &graph, int edgeCount, int start = 0) {
	std::vector<int>result;
	std::vector<bool>used(edgeCount, true);
	auto dfs = [&](auto self, int v)->void {
		for (auto[nv, idx] : graph[v])if (used[idx]) {
			used[idx] = false;
			self(self, nv);
		}
		result.push_back(v);
	};
	dfs(dfs, start);
	std::reverse(result.begin(), result.end());
	return result;
}