template<typename T, class F>
std::vector<T> mergeSort(std::vector<T> vec, const F &f) {
	auto dfs = [&](auto && self, int l, int r) {
		if (l + 1 == r)return;
		int m = (l + r) / 2;
		self(self, l, m);
		self(self, m, r);
		std::vector<T>d;
		d.reserve(r - l);
		int i0 = l, i1 = m;
		for (int i = 0; i < r - l; ++i) {
			if (i0 == m) d.push_back(vec[i1++]);
			else if (i1 == r)d.push_back(vec[i0++]);
			else {
				if (f(vec[i0], vec[i1]))d.push_back(vec[i0++]);
				else d.push_back(vec[i1++]);
			}
		}
		for (int i = l; i < r; ++i) {
			vec[i] = d[i - l];
		}
	};
	dfs(dfs, 0, vec.size());
	return vec;
}
// How To Use
//a = mergeSort(a, [](int l, int r) {
//	return l < r;
//});