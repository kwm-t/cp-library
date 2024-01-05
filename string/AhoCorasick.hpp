#include <vector>
#include <queue>

struct AhoCorasick {
	int kind;
	std::vector<int>node, fail;
	std::vector<std::vector<int>>next;
	std::vector<bool>end;
	AhoCorasick(int _kind) :kind(_kind) {
		node.resize(1);
		end.resize(1);
		next.emplace_back(std::vector<int>(kind, -1));
	}
	void insert(const std::vector<int>&v) {
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (-1 == next[now][v[i]]) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				end.push_back(false);
				next.emplace_back(std::vector<int>(kind, -1));
			}
			now = next[now][v[i]];
			node[now]++;
		}
		end[now] = true;
	}
	// åƒÇ—èoÇµïKê{
	void build() {
		fail.resize(node.size(), -1);
		std::queue<int>que;
		que.push(0);
		while (!que.empty()) {
			int v = que.front(); que.pop();
			for (int i = 0; i < kind; ++i) {
				if (-1 == next[v][i])continue;
				fail[next[v][i]] = calc(fail[v], i);
				if (end[fail[next[v][i]]])end[next[v][i]] = true;
				que.push(next[v][i]);
			}
		}
	}
	int calc(int v, int idx) {
		while (-1 != v) {
			if (-1 != next[v][idx])return next[v][idx];
			v = fail[v];
		}
		return 0;
	}
	std::vector<int> trans(std::string s) {
		std::vector<int>ret(s.size());
		for (int i = 0; i < (int)s.size(); ++i) ret[i] = s[i] - 'a';
		return ret;
	}
};