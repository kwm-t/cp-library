#include <vector>
#include <stack>
// returnされるvectorのサイズが頂点数と一致してなかったらdagではない。
std::vector<int>TopologicalSort(int n, std::vector<std::vector<int>>to) {
	std::vector<int>h(n);
	for (int i = 0; i < n; ++i) for (auto e : to[i]) ++h[e];
	std::stack<int>st;
	std::vector<int>ans;
	for (int i = 0; i < n; ++i) if (h[i] == 0) st.push(i);
	while (!st.empty()) {
		int v = st.top(); st.pop();
		ans.push_back(v);
		for (auto u : to[v]) {
			--h[u];
			if (h[u] == 0) st.push(u);
		}
	}
	return ans;
}