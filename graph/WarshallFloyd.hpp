#include <algorithm>
#include <utility>
#include <vector>
template<typename T>
long long warshallFloyd(int n, std::vector<std::vector<T>> &d, T inf) {
	for (int k = 0; k < n; ++k)for (int i = 0; i < n; ++i)for (int j = 0; j < n; ++j) {
		if (inf == d[i][k] || inf == d[k][j])continue;
		d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
	}
	long long ret = 0;
	for (int i = 0; i < n; ++i)for (int j = 0; j < n; ++j)if (d[i][j] != inf)ret += d[i][j];
	return ret;
}


template<typename T>
long long warshallFloydUpdate(int n, std::vector<std::vector<T>> &dp, T inf, int p, int q, long long w, bool twoway) {
	if (dp[p][q] <= w)return 0;
	long long diff = 0;
	for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
		long long val = dp[i][p] + w + dp[q][j];
		if (twoway)val = std::min(val, dp[i][q] + w + dp[p][j]);
		if (val >= dp[i][j])continue;
		if (dp[i][j] != inf) diff -= dp[i][j];
		dp[i][j] = val;
		diff += dp[i][j];
	}
	return diff;
}