#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
struct CenterTree {
public:
	CenterTree(int n) :sz(n), c0(-1), c1(-1) {
		pre.resize(sz);
		to.resize(sz);
	}
	void solve() {
		ReadGraph();
		auto get = dfs(dfs(0).first);
		int now = get.first;
		d = get.second;
		while (-1 != now) { v.push_back(now), now = pre[now]; }
		if (1 == d % 2) c0 = v[v.size() / 2 - 1], c1 = v[v.size() / 2];
		else c0 = v[v.size() / 2];
		//output();
	}
	std::vector<std::string> isomorphic() {
		std::vector<std::string>v;
		v.push_back(IsomorphicCalc(c0));
		if (-1 != c1)v.push_back(IsomorphicCalc(c1));
		return v;
	}
private:
	void ReadGraph() {
		for (int i = 0; i < sz - 1; ++i) {
			int a, b; std::cin >> a >> b; a--; b--;
			to[a].push_back(b);
			to[b].push_back(a);
		}
	}
	std::pair<int, int> dfs(int v, int d = 0, int p = -1) {
		pre[v] = p;
		std::pair<int, int> ret = { v,0 };
		for (int e : to[v]) {
			if (p == e) continue;
			auto p = dfs(e, d + 1, v);
			p.second++;
			if (p.second > ret.second) ret = p;
		}
		return ret;
	}
	void output() {
		std::cout << "Diameter " << d << std::endl;
		std::cout << "Center0 " << c0 << std::endl;
		std::cout << "Center1 " << c1 << std::endl;
	}
	// 同型判定
	std::string IsomorphicCalc(int v, int p = -1) {
		std::vector<std::string>vs;
		for (int e : to[v]) {
			if (p == e) continue;
			auto s = IsomorphicCalc(e, v);
			vs.push_back(s);
		}
		std::sort(vs.begin(), vs.end());
		std::string ret;
		ret += '(';
		for (int i = 0; i < vs.size(); ++i)ret += vs[i];
		ret += ')';
		return ret;
	}
private:
	int sz, c0, c1, d;//Treesize,Center0,Center1,Diameter
	std::vector<int> pre;
	std::vector<std::vector<int>>to;
	std::vector<int>v;//直径上の点(一例)
};