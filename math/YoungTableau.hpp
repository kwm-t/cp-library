// 01文字列(数列から単調増加列に変換する)
// 0:x++,1:y++
// ex:10100->[1,2,2]
std::vector<int> exchange01(const std::vector<int>&v) {
	int now = 0;
	std::vector<int>ret;
	for (auto e : v) {
		if (0 == e)ret.push_back(now);
		else now++;
	}
	return ret;
}
// 単調増加列に対して標準盤を数える
// O(n^2)
mint hookLengthFormula(const std::vector<int>&v) {
	if (v.empty())return 1;
	int sum = 0;
	for (auto e : v)sum += e;
	mint p = 1;
	for (int i = 1; i <= sum; ++i)p *= i;

	mint q = 1;
	std::vector<int>cnt(v.back() + 1);
	for (int i = 0; i < (int)v.size(); ++i) {
		for (int j = 1; j <= v[i]; ++j) {
			int tmp = cnt[j] + v[i] - j + 1;
			q *= tmp;
			cnt[j]++;
		}
	}
	return p * q.inv();
}