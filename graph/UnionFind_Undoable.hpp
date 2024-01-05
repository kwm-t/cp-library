#include <vector>
struct UnionFind_Undoable {
	int n;
	std::vector<int>par_or_size;
	std::vector<std::pair<int, int>>history;
	UnionFind_Undoable(int sz) :n(sz), par_or_size(sz, -1) {}
	int leader(int v) {
		if (par_or_size[v] < 0)return v;
		return leader(par_or_size[v]);
	}
	bool same(int u, int v) {
		return leader(u) == leader(v);
	}
	bool merge(int u, int v) {
		u = leader(u);
		history.emplace_back(u, par_or_size[u]);
		v = leader(v);
		history.emplace_back(v, par_or_size[v]);
		if (u == v)return false;
		if (par_or_size[u] > par_or_size[v]) std::swap(u, v);
		par_or_size[u] += par_or_size[v];
		par_or_size[v] = u;
		return true;
	}
	int size(int v) {
		int l = leader(v);
		return -par_or_size[l];
	}
	void undo() {
		for (int i = 0; i < 2; ++i) {
			par_or_size[history.back().first] = history.back().second;
			history.pop_back();
		}
	}
	void snapshot() {
		history.clear();
	}
	void rollback() {
		while (!history.empty()) undo();
	}
};