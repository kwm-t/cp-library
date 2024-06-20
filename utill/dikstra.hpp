struct Edge {
	int to;
	long long cost;
	Edge(int _to, long long _cost) :to(_to), cost(_cost) {}
};
vector<Edge>g[200005];

long long dikstra(int st, int ed) {
	vector<long long> dp(200000, LINF);
	#define P pair<long long,int>//!!!!
	std::priority_queue<P, vector<P>, greater<P>> q;
	q.push({0, st});
	dp[st] = 0;
	while (!q.empty()) {
		auto tmp = q.top();
		q.pop();
		long long cost = tmp.first;
		int pos = tmp.second;
		if (cost > dp[pos]) continue;
		for (Edge e : g[pos]) {
			long long ncost = cost + e.cost;
			int npos = e.to;
			if(chmin(dp[npos],ncost)) q.push({ncost, npos});
		}
	}
	if (LINF == dp[ed]) dp[ed] = -1;
	return dp[ed];
}