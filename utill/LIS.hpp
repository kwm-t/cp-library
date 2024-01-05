#include <vector>
const int INF = 1e9;
// dp[i] := ’·‚³‚ª i ‚Ì‘‰Á•”•ª—ñ‚Æ‚µ‚ÄÅŒã”ö‚Ì—v‘f‚Ì‚Æ‚è‚¤‚éÅ¬’l
template<class T> int LIS(std::vector<T> a, bool is_strong = true) {
	const T INF = 1 << 30; // to be set appropriately
	int n = (int)a.size();
	std::vector<T> dp(n, INF);
	for (int i = 0; i < n; ++i) {
		if (is_strong) *lower_bound(dp.begin(), dp.end(), a[i]) = a[i];
		else *upper_bound(dp.begin(), dp.end(), a[i]) = a[i];
	}
	return lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
}