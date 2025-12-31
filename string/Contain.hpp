#include <string>
#include <vector>
#include <atcoder/all>
bool contain(const std::string &s, const std::string &t) {
	// 文字列sが文字列tに含まれているかの判定
	int n = s.size(), m = t.size();
	std::vector<int> z = atcoder::z_algorithm(s + t);
	for (int i = n; i <= m; ++i) {
		if (z[i] >= n)return true;
	}
	return false;
}