#include <iostream> 
#include <vector>
class UnionFind_Potential {
public:
	UnionFind_Potential(int sz) :n(sz), weight(sz), par_or_size(sz, -1), ng(sz), cnt(n) {
		//group.resize(n);
		//for (int i = 0; i < n; ++i)group[i].push_back(i);
	}
	int leader(int v) {
		if (par_or_size[v] < 0)return v;
		int root = leader(par_or_size[v]);
		weight[v] += weight[par_or_size[v]];
		return par_or_size[v] = root;
	}
	bool same(int u, int v) {
		return leader(u) == leader(v);
	}
	bool merge(int u, int v, long long w) {
		if (same(u, v)) {
			long long d = dist(u, v);
			if (d != w) ng[leader(u)] = true;
			return d == w;
		}
		int ul = leader(u);
		int vl = leader(v);
		w += weight[u] - weight[v];
		if (-par_or_size[ul] < -par_or_size[vl]) {
			std::swap(ul, vl);
			w *= -1;
		}
		par_or_size[ul] += par_or_size[vl];
		par_or_size[vl] = ul;
		if (ng[ul])ng[vl] = ng[ul];
		weight[vl] = w;
		//for (int i = 0; i < group[vl].size(); ++i) group[ul].push_back(group[vl][i]);
		//group[vl].clear();
		cnt--;
		return true;
	}
	int size(int v) {
		int l = leader(v);
		return -par_or_size[l];
	}
	long long dist(int u, int v) {
		if (!same(u, v))return -1;
		return weight[v] - weight[u];
	}
	void debug() {
		for (int i = 0; i < n; ++i) {
			std::cout << i << " " << par_or_size[i] << " " << weight[i] << std::endl;
		}
	}
private:
	int n;
	int cnt;// group‚Ì”
	std::vector<int> par_or_size;// leader‚Ì‚Æ‚«‚ÍƒTƒCƒY‚ª“ü‚é
	std::vector<long long>weight;
	std::vector<bool>ng;
	//std::vector<std::vector<int>>group;
};
// xor version
//https://atcoder.jp/contests/past15-open/submissions/47881880