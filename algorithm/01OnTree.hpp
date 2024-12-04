// https://atcoder.jp/contests/abc376/submissions/60434950
// https://atcoder.jp/contests/agc023/submissions/60434772
template <class F, class G>
std::vector<int> optimalTopologicalTree_Order(std::vector<int>parent, const F merge, G compare) {
	int n = parent.size();

	std::set<int, decltype(compare)>st(compare);
	for (int i = 1; i < n; ++i) {
		st.insert(i);
	}

	std::vector<std::pair<int, int>>lr(n);
	for (int i = 0; i < n; ++i)lr[i] = { i,i };

	std::vector<int>next(n, -1);

	atcoder::dsu uf(n);
	for (int _ = 0; _ < n - 1; ++_) {
		auto v = *st.begin();
		st.erase(v);

		auto u = uf.leader(parent[v]);
		st.erase(u);

		auto[hu, tu] = lr[u];
		auto[hv, tv] = lr[v];
		next[tu] = hv;

		int nv = uf.merge(u, v);
		merge(u, v, nv);
		if (!uf.same(0, nv))st.insert(nv);
		lr[nv] = { hu,tv };
		parent[nv] = parent[u];
	}
	std::vector<int>res(n);
	for (int i = 0, v = 0; i < n; ++i, v = next[v]) {
		res[i] = v;
	}
	return res;
}
