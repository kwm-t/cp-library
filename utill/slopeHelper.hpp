#include <vector>
std::vector<long long>slopeHelper(const std::vector<std::pair<long long, long long>> &slope) {
	std::vector<long long>ret = { 0 };
	for (int i = 0; i < (int)slope.size(); ++i) {
		auto k = slope[i].first;
		auto v = slope[i].second;
		if (0 == k)continue;
		auto prek = slope[i - 1].first;
		auto prev = slope[i - 1].second;
		long long x = (v - prev) / (k - prek);
		for (int j = prek + 1; j < k + 1; ++j) {
			long long val = prev + x * (j - prek);
			ret.push_back(val);
		}
	}
	return ret;
}
std::vector<long long>slopeHelper(const std::vector<std::pair<long long, long long>>& slope, long long big) {
	auto ret = slopeHelper(slope);
	for (int i = 0; i < (int)ret.size(); ++i) ret[i] -= big * i;
	return ret;
}