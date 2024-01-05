#include<vector>
#include<iostream> 
struct ReRooting {
	struct Edge {//š
		int to;
		Edge(int _to) :to(_to) {}
	};
	struct Data {//š
		mint val;
		Data() :val(1) {}
		Data(mint _val) :val(_val) {}
	};
	std::vector<std::vector<Edge>>g;
	std::vector<Data>dp, ans;
	int sz;
	ReRooting(int _sz) :sz(_sz) {
		g.resize(sz);
		dp.reserve(sz);
		ans.resize(sz);
	}
	void ReadGraph() {//š
		for (int i = 0; i < sz - 1; ++i) {
			int a, b; std::cin >> a >> b; a--; b--;
			g[a].push_back({ b });
			g[b].push_back({ a });
		}
	}
	Data  merge(const Data &lh, const Data &rh) {//šš
		Data ret = lh;
		ret.val *= rh.val + 1;
		return ret;
	}
	Data  mergeSub(const Data &lh, const Data &rh) {//šš
		Data ret = lh;
		ret.val *= rh.val;
		return ret;
	}
	void dfs(int v, int p = -1) {//š
		Data val;
		for (auto e : g[v]) {
			if (p == e.to)continue;
			dfs(e.to, v);
			val = merge(val, dp[e.to]);
		}
		dp[v] = val;
	}
	void dfsR(int v, int p = -1) {//š
		ans[v] = dp[v];
		if (-1 != p)ans[v] = merge(ans[v], dp[p]);
		std::vector<Data>vdata;
		for (auto e : g[v]) vdata.push_back(dp[e.to]);
		int sz = vdata.size();
		std::vector<Data>sumL(sz + 1), sumR(sz + 1);
		for (int i = 0; i < sz; ++i)sumL[i + 1] = merge(sumL[i], vdata[i]);
		for (int i = 0; i < sz; ++i)sumR[i + 1] = merge(sumR[i], vdata[sz - i - 1]);
		for (int i = 0; i < g[v].size(); ++i) {
			auto e = g[v][i];
			if (p == e.to)continue;
			dp[v] = mergeSub(sumL[i], sumR[sz - (i + 1)]);
			dfsR(e.to, v);
		}
	}
	void output() {//š
		for (int i = 0; i < sz; ++i)std::cout << ans[i].val.val() << std::endl;
	}
	void solve() {
		ReadGraph();
		dfs(0);
		dfsR(0);
		output();
	}
};