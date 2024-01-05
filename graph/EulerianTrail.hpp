#include <vector>
void f() {
	int n;
	std::vector<int>ans;
	std::vector<std::vector<int>>g(n, std::vector<int>(n));
	auto dfs = [&](auto self, int v)->void {
		for (int i = 0; i < n; ++i)if (g[v][i]) {
			g[v][i]--;
			g[i][v]--;
			self(self, i);
		}
		ans.push_back(v);
	};
	dfs(dfs, 0);
	std::reverse(ans.begin(), ans.end());
}