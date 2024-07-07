#include <vector>
// dp[i] := 長さが i の増加部分列として最後尾の要素のとりうる最小値
template<class T> int lis(std::vector<T> a, bool is_strong = true) {
	const T INF = 1 << 30; // to be set appropriately
	int n = (int)a.size();
	std::vector<T> dp(n, INF);
	for (int i = 0; i < n; ++i) {
		if (is_strong) *lower_bound(dp.begin(), dp.end(), a[i]) = a[i];
		else *upper_bound(dp.begin(), dp.end(), a[i]) = a[i];
	}
	return lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
}