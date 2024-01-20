vector<int>l(n), r(n);
auto dfs = [&](auto &&self, int &time = 0, int v = 0, int p = -1)->void {
	if (-1 != p && to[v].size() == 1) l[v] = time;
	else l[v] = n;
	for (auto &nv : to[v]) {
		if (p == nv)continue;
		self(self, time, nv, v);
		l[v] = min(l[v], l[nv]);
	}
	r[v] = time + 1;
	time++;
};
int time = 0;
dfs(dfs, time, 0);