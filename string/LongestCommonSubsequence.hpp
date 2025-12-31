#include <vector>
#include <string>
std::string lcs(const std::string &s, const std::string &t) {
	std::vector<std::vector<int>> dp(s.size() + 1, std::vector<int>(t.size() + 1));
	std::vector<std::vector<std::pair<int, int>>> dppre(s.size() + 1, std::vector<std::pair<int, int>>(t.size() + 1, { -1,-1 }));
	for (int i = 0; i < (int)s.size(); ++i)for (int j = 0; j < (int)t.size(); ++j) {
		if (s[i] == t[j]) {
			if (dp[i + 1][j + 1] < dp[i][j] + 1) {
				dp[i + 1][j + 1] = dp[i][j] + 1;
				dppre[i + 1][j + 1] = { i,j };
			}
		}
		else {
			if (dp[i + 1][j + 1] < dp[i + 1][j + 0]) {
				dp[i + 1][j + 1] = dp[i + 1][j + 0];
				dppre[i + 1][j + 1] = dppre[i + 1][j + 0];
			}
			if (dp[i + 1][j + 1] < dp[i + 0][j + 1]) {
				dp[i + 1][j + 1] = dp[i + 0][j + 1];
				dppre[i + 1][j + 1] = dppre[i + 0][j + 1];
			}
		}
	}
	//復元
	std::string str;
	std::pair<int, int> p = dppre[s.size()][t.size()];
	while (std::make_pair(-1, -1) != p) {
		str += s[p.first];
		p = dppre[p.first][p.second];
	}
	std::reverse(str.begin(), str.end());
	return str;
}