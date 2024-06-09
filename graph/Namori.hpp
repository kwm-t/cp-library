#include <vector>
#include <set>
// なもりグラフを与えるとサイクルに含むindexを返す
std::vector<int> namori(std::vector<std::vector<int>> &to) {
	std::vector<int>roop;
	std::set<int>st;
	auto dfs = [&](auto self, int v, int p = -1)->bool {
		if (st.count(v)) {
			std::reverse(roop.begin(),roop.end());
			while (v != roop.back()) roop.pop_back();
			std::reverse(roop.begin(), roop.end());
			return true;
		}
		st.insert(v);
		roop.push_back(v);
		for (auto nv : to[v]) {
			if (p == nv)continue;
			auto get = self(self, nv, v);
			if (get)return true;
		}
		roop.pop_back();
		st.erase(v);
		return false;
	};
	return roop;
}