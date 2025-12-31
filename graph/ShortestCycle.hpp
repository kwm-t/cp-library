#include <algorithm>
#include <vector>
const int INF = 1e9;
// 最小サイクル
// https://atcoder.jp/contests/abc308/editorial/6709
// グラフと根を渡す
// overFlowに注意
std::pair<long long, std::vector<int>> shortest_cycle(const std::vector<std::vector<long long>> &G, int root) {
	int n = G.size();
	std::vector<long long>dist(n, INF * 2), p(n, -1), g(n);
	std::vector<bool>seen(n);
	dist[root] = 0;
	g[root] = root;
	// rootに近いとこから確定させていく
	for (int t = 0; t < n; t++) {
		long long mn = INF * 2;
		int pos = -1;
		for (int i = 0; i < n; ++i) {
			if (!seen[i] && dist[i] < mn) {
				mn = dist[i];
				pos = i;
			}
		}
		seen[pos] = true;
		for (int i = 0; i < n; ++i) {
			if (dist[i] > dist[pos] + G[pos][i]) {
				dist[i] = dist[pos] + G[pos][i];
				p[i] = pos;
				g[i] = (pos == root ? i : g[pos]);
			}
		}
	}
	long long mn = (long long)INF * 5;
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (p[i] == j || p[j] == i)continue;
			if (g[i] == g[j])continue;
			mn = std::min(mn, dist[i] + dist[j] + G[i][j]);
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (p[i] == j || p[j] == i)continue;
			if (g[i] == g[j])continue;
			if (mn != dist[i] + dist[j] + G[i][j])continue;
			std::vector<int>res;
			int a = i, b = j;
			while (a != root) {
				res.push_back(a);
				a = p[a];
			}
			res.push_back(a);
			reverse(res.begin(), res.end());
			while (b != root) {
				res.push_back(b);
				b = p[b];
			}
			return { mn,res };
		}
	}
}