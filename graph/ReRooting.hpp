#include<vector>
#include<iostream> 
struct ReRooting {
	struct Edge {//šš
		int to;
		mint cost;
		Edge(int _to, mint _cost) :to(_to), cost(_cost) {}
	};
	struct Data {//šš
		mint sump;
		mint sum;
		mint count;
		Data() :sump(0), sum(0), count(0) {}
		Data(mint _sump, mint _sum, mint _count) :sump(_sump), sum(_sum), count(_count) {}
	};
	std::vector<std::vector<Edge>>g;
	std::vector<Data>dp;
	std::vector<Data>ans;
	int sz;
	ReRooting(int _sz) :sz(_sz) {
		g.resize(sz);
		dp.reserve(sz);
		ans.resize(sz);
	}
	void ReadGraph() {//š
		for (int i = 0; i < sz - 1; ++i) {
			int a, b, c; std::cin >> a >> b >> c; a--; b--;
			g[a].push_back({ b,c });
			g[b].push_back({ a,c });
		}
	}
	Data merge(const Data &lh, const Data &rh, mint cost) {//šš
		Data ret = lh;
		ret.sump += rh.sump + rh.sum * cost * 2 + cost * cost * (rh.count + 1);
		ret.sum += rh.sum + cost * (rh.count + 1);
		ret.count += rh.count + 1;
		return ret;
	}
	Data mergeR(const Data &lh, const Data &rh, mint cost) {//šš
		Data ret = lh;
		ret.sump -= rh.sump + rh.sum * cost * 2 + cost * cost * (rh.count + 1);
		ret.sum -= rh.sum + cost * (rh.count + 1);
		ret.count -= rh.count + 1;
		return ret;
	}
	void dfs(int v, int p = -1) {//š
		Data val;
		for (auto e : g[v]) {
			if (p == e.to)continue;
			dfs(e.to, v);
			val = merge(val, dp[e.to], e.cost);
		}
		dp[v] = val;
	}
	void dfsR(int v, int p = -1) {//š
		ans[v] = dp[v];
		if (-1 != p)ans[v] = merge(ans[v], dp[p], 0);
		for (auto e : g[v]) {
			if (p == e.to)continue;
			dp[v] = mergeR(ans[v], dp[e.to], e.cost);
			dfsR(e.to, v);
		}
	}
	void output() {//š
		mint res = 0;
		for (int i = 0; i < sz; ++i)res += ans[i].sump;
		res *= inv_mod(2, mod);
		std::cout << res.val() << std::endl;
	}
	void solve() {
		ReadGraph();
		dfs(0);
		dfsR(0);
		output();
	}
};