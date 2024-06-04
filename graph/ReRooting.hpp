template <typename Data, typename Cost, Data(*e)(), Data(*leaf)(int), Data(*addEdge)(const Data&)
	, Data(*addVertex)(const Data&), Data(*merge)(const Data&, const Data&)>
	class ReRooting {
	public:
		ReRooting(int size) :size(size) {
			graph.resize(size);
		}

		void setEdge(int s, int t, Cost c = 0) {
			graph[s].emplace_back(s, t, c);
			graph[t].emplace_back(t, s, c);
		}

		std::vector<Data>execute() {
			sdp.resize(size);
			dfs(0, -1);
			dp = sdp;
			dfsR(0, -1);
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
			auto val = e();
			bool upd = false;
			for (Edge &edge : graph[v]) {
				if (p == edge.to)continue;
				dfs(edge.to, v);
				upd = true;
				val = merge(val, addEdge(sdp[edge.to]));
			}
			if (!upd)sdp[v] = leaf(v);
			else sdp[v] = addVertex(val);
		}

		void dfsR(int v, int p) {
			std::vector<Data>tmp;
			for (Edge &edge : graph[v]) {
				if (edge.to == p) tmp.push_back(addEdge(sdp[p]));
				else tmp.push_back(addEdge(sdp[edge.to]));
			}
			int n = tmp.size();
			std::vector<Data> head(n + 1, e()), tail(n + 1, e());
			for (int i = 0; i++ < n;) head[i] = merge(head[i - 1], tmp[i - 1]);
			for (int i = n; i-- > 0;) tail[i] = merge(tail[i + 1], tmp[i]);
			dp[v] = head[n];
			int index = 0;
			for (Edge &edge : graph[v]) {
				if (p != edge.to) {
					sdp[v] = addVertex(merge(head[index], tail[index + 1]));
					dfsR(edge.to, v);
				}
				index++;
			}
		}
};
// ReRooting<Data, Cost, e, leaf, addEdge, addVertex, merge> reroot(n);
// sumple ARC179D
using Cost = int;
struct Data {
	int e;// 辺の数
	int h;// 高さ
	int v0;// 根に戻る場合の最小コスト
	int v1;// 最小コスト
};
Data e() {
	return { 0,0,0,0 };
}
Data leaf(int v) {
	return e();
}
Data addEdge(const Data &x) {
	return{ x.e + 1,x.h + 1, min(x.v0 + 2,1 + 2 * x.e - x.h) ,x.v1 + 1 };
}
Data addVertex(const Data &x) {
	return x;
}
Data merge(const Data &l, const Data&r) {
	return { l.e + r.e,max(l.h,r.h),l.v0 + r.v0,min(l.v0 + r.v1,l.v1 + r.v0) };
}