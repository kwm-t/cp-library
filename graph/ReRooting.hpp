template <typename Cost, typename Data, Data(*merge)(const Data &, const Data &)
, Data(*mergeR)(const Data &, const Data &), Data(*e)(int), Data(*apply)(const Data &, Cost)>
	class ReRooting {
	public:
		ReRooting(int size) :size(size) {
			graph.resize(size);
		}

		void addEdge(int s, int t, Cost c = 0) {
			graph[s].emplace_back(s, t, c);
		}

		std::vector<Data>execute() {
			sdp.resize(size);
			dfs(0);
			dp = sdp;
			dfsR(0);
			return dp;
		}
	private:
		struct Edge {
			int from, to;
			Cost cost;
			Edge(int from, int to, Cost cost) :from(from), to(to), cost(cost) {}
		};

		int size;
		std::vector<std::vector<Edge>>graph;
		std::vector<Data>sdp, dp;

		void dfs(int v, int p = -1) {
			Data val = e(v);
			for (Edge &edge : graph[v]) {
				if (p == edge.to)continue;
				dfs(edge.to, v);
				val = merge(val, apply(sdp[edge.to], edge.cost));
			}
			sdp[v] = val;
		}

		void dfsR(int v, int p = -1) {
			for (Edge &edge : graph[v]) {
				if (p == edge.to)continue;
				auto tmp = mergeR(dp[v], apply(sdp[edge.to], edge.cost));
				dp[edge.to] = merge(dp[edge.to], apply(tmp, edge.cost));
				dfsR(edge.to, v);
			}
		}
};
/*
ReadMe
using Data = mint;
using Cost = long long;
Data merge(Data a, Data b) { return a * b; }
Data mergeR(Data a, Data b) { return a * b; }
Data apply(Data a, Cost c) { return a; }
Data e(int v = -1) { return 1; }

ReRooting <Cost, Data, merge, mergeR, e>reroot(n);
g.add_edge();
auto get = g.execute();
*/