#include <vector>
#include <queue>

std::vector<int> namori_cycle(const std::vector<std::vector<int>>& to) {
	int n = to.size();
	std::vector<int> deg(n);
	for (int i = 0; i < n; i++) deg[i] = to[i].size();

	std::queue<int> q;
	for (int i = 0; i < n; i++) if (deg[i] == 1) q.push(i);

	std::vector<int> in_cycle(n, 1);
	while (!q.empty()) {
		int v = q.front(); q.pop();
		in_cycle[v] = 0;
		for (auto nv : to[v]) {
			if (--deg[nv] == 1) q.push(nv);
		}
	}

	// 並び順復元
	int start = -1;
	for (int i = 0; i < n; i++) if (in_cycle[i]) { start = i; break; }

	std::vector<int> roop;
	int prev = -1, cur = start;
	while (true) {
		roop.push_back(cur);
		int nxt = -1;
		for (auto nv : to[cur]) {
			if (in_cycle[nv] && nv != prev) {
				nxt = nv;
				break;
			}
		}
		if (nxt == -1 || nxt == start) break;
		prev = cur;
		cur = nxt;
	}
	return roop;
}