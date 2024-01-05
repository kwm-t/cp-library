bool namoriCheck() {
	int n, m; std::cin >> n >> m;
	dsu uf(n);
	std::map<int, int>mp;
	for (int i = 0; i < m; ++i) {
		int u, v; std::cin >> u >> v; u--; v--;
		int x = mp[uf.leader(u)] + 1;
		if (!uf.same(uf.leader(u), uf.leader(v)))x += mp[uf.leader(v)];
		uf.merge(u, v);
		mp[uf.leader(u)] = x;
	}
	// 単純サイクルも含む
	for (int i = 0; i < n; ++i){
		if (i != uf.leader(i))continue;
		if (uf.size(i) == mp[i])continue;
		else return false;
	}
	return true;
}