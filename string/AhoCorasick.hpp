#include <vector>
#include <queue>
template <int kind = 1>
struct AhoCorasick {
	std::vector<int>node, fail;
	std::vector<std::vector<int>>next;
	std::vector<int>cnt;// endCnt
	std::vector<int> bfsOrder;// bfs探索順
	AhoCorasick() {
		node.resize(1);
		cnt.resize(1);
		next.emplace_back(std::vector<int>(kind, -1));
	}
	// Trieの構築
	int insert(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (-1 == next[now][v[i]]) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				cnt.push_back(0);
				next.emplace_back(std::vector<int>(kind, -1));
			}
			now = next[now][v[i]];
			node[now]++;
		}
		cnt[now] = 1;
		return now;
	}
	// insertの終了後に呼び出し必須
	void build() {
		// 最長のsuffixに対してfailure-linkを貼る
		fail.resize(node.size(), -1);
		std::queue<int>que;
		que.push(0);
		while (!que.empty()) {
			int v = que.front(); que.pop();
			bfsOrder.push_back(v);
			for (int i = 0; i < kind; ++i) {
				if (-1 == next[v][i])continue;
				fail[next[v][i]] = calcIndex(fail[v], i);
				cnt[next[v][i]] += cnt[fail[next[v][i]]];
				que.push(next[v][i]);
			}
		}
	}
	int calcIndex(int v, int idx) {
		while (-1 != v) {
			if (-1 != next[v][idx])return next[v][idx];
			v = fail[v];
		}
		return 0;
	}
	void dump() {
		int sz = node.size();
		std::cout << node.size() << std::endl;
		for (int i = 0; i < sz; ++i) {
			std::cout << node[i] <<" " << fail[i] << " " << cnt[i] << std::endl;
		}
	}
};