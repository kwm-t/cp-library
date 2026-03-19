auto dfs = [&](auto &&self, int v, int p = -1)->void {
	for (auto nv : to[v]) {
		if (p == nv)continue;
		self(self, nv, v);
	}
};
dfs(dfs, 0);