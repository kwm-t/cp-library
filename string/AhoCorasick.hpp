#include <vector>
#include <queue>
struct AhoCorasick {
	static const int csize = 26;
	static const int offset = 'a';
	struct Node {
		std::vector<int> to;
		int flink;

		Node() { to = std::vector<int>(csize, -1); }

		// problem-specific process
		int ends = 0;
		void mark(int i) { ends = 1 << i; }
		void marge(const Node &other) { ends |= other.ends; }
	};
	AhoCorasick() {
		nodes.resize(1);
	}
	std::vector<Node> nodes;
	int add(const std::string& s, int index) {
		int now = 0;
		for (auto c : s) {
			int i = c - offset;
			int next = nodes[now].to[i];
			if (next == -1) {
				next = nodes[now].to[i] = (int)nodes.size();
				nodes.emplace_back();
			}
			now = next;
		}
		nodes[now].mark(index);
		return now;
	}
	void build() {
		// 最長のsuffixに対してfailure-linkを貼る
		nodes[0].flink = -1;
		std::queue<int> q;
		q.emplace(0);
		while (!q.empty()) {
			const int u = q.front();
			q.pop();
			for (int c = 0; c < csize; c++) {
				int v = nodes[u].to[c];
				if (v == -1) continue;
				q.emplace(v);
				int f = next(nodes[u].flink, c);
				nodes[v].flink = f;
				nodes[v].marge(nodes[f]);
			}
		}
	}
	int next(int now, int index) {
		while (now != -1) {
			int next = nodes[now].to[index];
			if (next != -1) return next;
			now = nodes[now].flink;
		}
		return 0;
	}
	int next(int u, char c) {
		return next(u, c - offset);
	}
};