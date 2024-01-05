#include<vector>
#include<algorithm>
// ì]ì|êî
long long op(long long a, long long b) { return a + b; }
long long e() { return 0; }
long long inversionNumber(std::vector<int>v) {
	auto tmp = v;
	std::sort(tmp.begin(),tmp.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	for (int i = 0; i < (int)v.size(); ++i) v[i] = std::lower_bound(tmp.begin(), tmp.end(), v[i]) - tmp.begin();
	long long ret = 0;
	atcoder::segtree<long long, op, e>seg(tmp.size());
	for (int i = 0; i < (int)v.size(); ++i) {
		ret += seg.prod(v[i] + 1, tmp.size());
		seg.set(v[i], seg.get(v[i]) + 1);
	}
	return ret;
}