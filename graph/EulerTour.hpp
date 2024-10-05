std::pair<std::vector<int>, std::vector<int>>eulerTour(int n, const std::vector<std::vector<int>> &e) {
	vector<int>l(n), r(n);
	auto dfs = [&](auto &&self, int &time = 0, int v = 0, int p = -1)->void {
		if (-1 != p && e[v].size() == 1) l[v] = time;
		else l[v] = n - 1;
		for (auto &nv : e[v]) {
			if (p == nv)continue;
			self(self, time, nv, v);
			l[v] = std::min(l[v], l[nv]);
		}
		r[v] = time + 1;
		time++;
	};
	int time = 0;
	dfs(dfs, time, 0);
	return {l, r};
}