#include <vector>
template<typename T>
std::vector<std::vector<T>> travelingSalesmanProblem(
	int n, const std::vector<std::vector<T>> &dist, T inf, int start = 0) {
	std::vector dp(1 << n, std::vector<T>(n, inf));
	dp[1 << start][start] = 0;
	for (int i = 0; i < (1 << n); ++i) {
		for (int j = 0; j < n; ++j) {
			if (dp[i][j] == inf)continue;
			for (int k = 0; k < n; ++k)if (!(1 & (i >> k))) {
				int ni = i + (1 << k);
				T nval = dp[i][j] + dist[j][k];
				if (nval < dp[ni][k])dp[ni][k] = nval;
			}
		}
	}
	return dp;
}