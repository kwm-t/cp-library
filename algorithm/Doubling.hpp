class Doubling {
public:
	Doubling(const std::vector<int> &v, int bitsize = 60) :bitSize(bitsize) {
		init(v);
	}
	// 要素pからk回遷移した先を返す
	int query(int p, long long k) {
		for (int i = 0; i < bitSize; ++i) {
			if (1 & (k >> i))p = dp[i][p];
		}
		return p;
	}
private:
	int bitSize;
	std::vector<std::vector<int>> dp;
	void init(const std::vector<int>&v) {
		dp.resize(bitSize);
		for (int i = 0; i < bitSize; ++i) {
			dp[i].resize((int)v.size());
		}
		dp[0] = v;
		for (int i = 1; i < bitSize; ++i) {
			for (int j = 0; j < (int)v.size(); ++j) {
				dp[i][j] = dp[i - 1][dp[i - 1][j]];
			}
		}
	}
};
// 値を載せたいとき
// https://atcoder.jp/contests/abc438/submissions/72017004