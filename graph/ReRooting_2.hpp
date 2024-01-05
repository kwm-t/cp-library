template <typename Cost, typename Data, Data(*merge)(Data, Data),
	Data(*e)(), Data(*leaf)(), Data(*apply)(Data, int, int, Cost)>
	struct Rerooting {
	struct Edge {
		int src, to;
		Cost cost;
		Edge(int s, int t, Cost c = 1) : src(s), to(t), cost(c) {}
	};

	Rerooting(int n) :sz(n) {
		graph.resize(sz);
	}

	void add_edge(int s, int t, Cost c = 1) {
		graph[s].emplace_back(s, t, c);
	}

	std::vector<Data>execute() {
		sdp.resize(sz, e());
		dp.resize(sz);
		dfs0(0, -1);
		dfs1(0, -1, e());
		return dp;
	}

	int sz;
	std::vector<Data>dp, sdp;
	std::vector<std::vector<Edge>>graph;

	void dfs0(int v, int p) {
		bool upd = false;
		for (Edge &edge : graph[v]) {
			if (edge.to == p)continue;
			dfs0(edge.to, v);
			upd = true;
			sdp[v] = merge(sdp[v], apply(sdp[edge.to], edge.to, v, edge.cost));
		}
		if (!upd)sdp[v] = leaf();
	}

	void dfs1(int v, int p, const Data &val) {
		vector<Data>tmp{ val };
		for (Edge &edge : graph[v]) {
			if (edge.to == p)continue;
			tmp.push_back(apply(sdp[edge.to], edge.to, v, edge.cost));
		}
		int n = tmp.size();
		std::vector<Data> head(n + 1, e()), tail(n + 1, e());
		for (int i = 0; i++ < n;) head[i] = merge(head[i - 1], tmp[i - 1]);
		for (int i = n; i-- > 0;) tail[i] = merge(tail[i + 1], tmp[i]);
		dp[v] = head[n];
		int idx = 1;
		for (Edge &edge : graph[v]) {
			if (edge.to == p)continue;
			Data sub = merge(head[idx], tail[idx + 1]);
			dfs1(edge.to, v, apply(sub, v, edge.to, edge.cost));
			idx++;
		}
	}
	/*
	ReadMe
	using Data = mint;
	using Cost = long long;
	Data merge(Data a, Data b) { return a * b; }
	Data e() { return 1; }
	Data leaf() { return 1; }
	Data apply(Data a, int c, int, Cost w) { return 1 + a; }

	Rerooting<Cost, Data, merge, e, leaf, apply> g(n);
	g.add_edge()
	atup get = g.execute
	*/
};
