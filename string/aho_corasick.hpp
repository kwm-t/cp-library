/**
 * @file aho_corasick.hpp
 * @brief Aho-Corasick 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/aho_corasick.hpp
 */
#ifndef KWM_T_STRING_AHO_CORASICK_HPP
#define KWM_T_STRING_AHO_CORASICK_HPP

#include <vector>
#include <queue>
#include <string>

/**
 * @brief Aho-Corasick法（多パターン文字列検索）
 *
 * 複数の文字列をTrieに登録し、
 * テキスト中の出現を一括で高速に検出する
 *
 * 計算量:
 *   構築: O(Σ|S|)
 *   クエリ: O(|T|)
 *
 * 使用例:
 *   AhoCorasick ac;
 *   ac.add("abc", 0);
 *   ac.add("bc", 1);
 *   ac.build();
 *
 *   int state = 0;
 *   for (char c : text) {
 *       state = ac.next(state, c);
 *       if (ac.nodes[state].ends) {
 *           // マッチあり
 *       }
 *   }
 *
 * 制約 / 注意:
 *   - 英小文字前提（csize=26）
 *   - endsはbitmask（最大32パターン程度）
 *
 * verified:
 *   - https://atcoder.jp/contests/abc268/submissions/74811082
 *   - https://atcoder.jp/contests/abc362/submissions/74811147
 *   - https://atcoder.jp/contests/abc419/submissions/74811201
 */
namespace kwm_t::string {

struct AhoCorasick {
	static constexpr int csize = 26;
	static constexpr char offset = 'a';

	struct Node {
		std::vector<int> to; // 遷移
		int flink;           // failure link

		// problem-specific
		int ends = 0;        // マッチしたパターン集合（bitmask）

		Node() : to(csize, -1), flink(-1) {}

		void mark(int i) {
			ends |= (1 << i);
		}

		void merge(const Node& other) {
			ends |= other.ends;
		}
	};

	std::vector<Node> nodes;
	std::vector<int> bfs_order;

	AhoCorasick() {
		nodes.emplace_back(); // root
	}

	/**
	 * @brief パターン追加
	 * @param s 文字列
	 * @param index パターンID（bit位置）
	 */
	int add(const std::string& s, int index = 0) {
		int now = 0;
		for (char c : s) {
			int i = c - offset;
			if (nodes[now].to[i] == -1) {
				nodes[now].to[i] = (int)nodes.size();
				nodes.emplace_back();
			}
			now = nodes[now].to[i];
		}
		nodes[now].mark(index);
		return now;
	}

	/**
	 * @brief failure link 構築
	 */
	void build() {
		std::queue<int> q;

		nodes[0].flink = -1;
		q.push(0);

		while (!q.empty()) {
			int u = q.front(); q.pop();
			bfs_order.push_back(u);

			for (int c = 0; c < csize; ++c) {
				int v = nodes[u].to[c];
				if (v == -1) continue;

				q.push(v);

				int f = next(nodes[u].flink, c);
				nodes[v].flink = f;

				// マッチ情報引き継ぎ
				nodes[v].merge(nodes[f]);
			}
		}
	}

	/**
	 * @brief 状態遷移（index版）
	 */
	int next(int now, int c) const {
		while (now != -1) {
			int nxt = nodes[now].to[c];
			if (nxt != -1) return nxt;
			now = nodes[now].flink;
		}
		return 0;
	}

	/**
	 * @brief 状態遷移（char版）
	 */
	int next(int now, char c) const {
		return next(now, c - offset);
	}

	/**
	 * @brief ノード数
	 */
	int size() const {
		return (int)nodes.size();
	}

	Node& operator[](int i) {
		return nodes[i];
	}

	const Node& operator[](int i) const {
		return nodes[i];
	}
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_AHO_CORASICK_HPP