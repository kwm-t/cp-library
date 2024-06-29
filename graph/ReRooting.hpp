template <typename Data, Data(*e)(), Data(*addEdge)(const Data&, int)
	, Data(*addVertex)(const Data&, int), Data(*merge)(const Data&, const Data&)>
	class ReRooting {
	public:
		ReRooting(int size) :size(size) {
			edges.resize(size);
		}

		void setEdge(int s, int t, int index) {
			edges[s].emplace_back(s, t, index);
			edges[t].emplace_back(t, s, index);
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
			int index;
			Edge(int from, int to, int index) :from(from), to(to), index(index) {}
		};

		int size;
		std::vector<std::vector<Edge>>edges;
		std::vector<Data>sdp, dp;

		void dfs(int v, int p = -1) {
			auto val = e();
			for (Edge &edge : edges[v]) {
				if (p == edge.to)continue;
				dfs(edge.to, v);
				val = merge(val, addEdge(sdp[edge.to], edge.index));
			}
			sdp[v] = addVertex(val, v);
		}

		void dfsR(int v, int p = -1) {
			std::vector<Data>tmp;
			for (Edge &edge : edges[v]) {
				tmp.push_back(addEdge(sdp[edge.to], edge.index));
			}
			int n = tmp.size();
			std::vector<Data> head(n + 1, e()), tail(n + 1, e());
			for (int i = 0; i++ < n;) head[i] = merge(head[i - 1], tmp[i - 1]);
			for (int i = n; i-- > 0;) tail[i] = merge(tail[i + 1], tmp[i]);
			dp[v] = head[n];
			int index = 0;
			for (Edge &edge : edges[v]) {
				if (p != edge.to) {
					sdp[v] = addVertex(merge(head[index], tail[index + 1]), v);
					dfsR(edge.to, v);
				}
				index++;
			}
		}
};
// sumple ARC179D
// ReRooting<Data, e, addEdge, addVertex, merge> reroot(n);
struct Data {
	int e;// 辺の数
	int h;// 高さ
	int v0;// 根に戻る場合の最小コスト
	int v1;// 最小コスト
};
Data e() {
	return { 0,0,0,0 };
}
Data addEdge(const Data &x, int edgeIndex) {
	return{ x.e + 1,x.h + 1, min(x.v0 + 2,1 + 2 * x.e - x.h) ,x.v1 + 1 };
}
Data addVertex(const Data &x, int vertexIndex) {
	return x;
}
Data merge(const Data &l, const Data&r) {
	return { l.e + r.e,max(l.h,r.h),l.v0 + r.v0,min(l.v0 + r.v1,l.v1 + r.v0) };
}