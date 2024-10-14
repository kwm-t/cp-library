auto dijkstra = [&](const int st)->vector<long long> {
	vector<long long>dp(n, LINF);
	dp[st] = 0;
	using T = std::pair<long long, int>;
	std::priority_queue<T, std::vector<T>, std::greater<T>> pq;
	pq.push({ dp[st], st });
	while (!pq.empty()) {
		auto p = pq.top();
		pq.pop();
		long long cost = p.first;
		int v = p.second;
		if (dp[v] < cost) continue;
		for (const auto &e : to[v]) {
			if (dp[e.first] > dp[v] + e.second) {
				dp[e.first] = dp[v] + e.second;
				pq.push({ dp[e.first], e.first });
			}
		}
	}
	return dp;
};