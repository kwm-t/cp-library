#include <vector>
#include <algorithm>
// nの分割数を列挙する
std::vector<std::vector<int>> numberOfDivisions(const int n, const int maxSize) {
	std::vector<std::vector<int>>result;
	std::vector<int> now;
	int sum = 0;
	auto dfs = [&](auto &&self)->void {
		if (sum > n)return;
		if (sum == n) {
			result.push_back(now);
			return;
		}
		if (now.size() == maxSize)return;
		int mx = n - sum;
		if (!now.empty())mx = std::min(mx, now.back());
		for (int i = mx; i >= 1; --i) {
			now.push_back(i);
			sum += i;
			self(self);
			sum -= i;
			now.pop_back();
		}
	};
	dfs(dfs);
	return result;
}
std::vector<std::vector<int>> numberOfDivisions(const int n) {
	return numberOfDivisions(n, n);
}