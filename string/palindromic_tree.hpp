/**
 * @file palindromic_tree.hpp
 * @brief Palindromic Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/palindromic_tree.hpp
 */
#ifndef KWM_T_STRING_PALINDROMIC_TREE_HPP
#define KWM_T_STRING_PALINDROMIC_TREE_HPP

#include <vector>
#include <map>
#include <string>

/**
 * @brief Palindromic Tree（Eertree）
 *
 * 文字列中のすべての回文部分文字列を管理するデータ構造
 *
 * 主な機能:
 *   - 回文の列挙（種類数 O(N)）
 *   - 各回文の出現位置管理
 *   - suffixLink による回文遷移
 *   - deltaLink による回文DP高速化
 *
 * 計算量:
 *   構築: O(N log Σ)（map使用）
 *
 * @tparam T 文字型（char など）
 *
 * 使用例:
 *   PalindromicTree<> pt(s);
 *   int sz = pt.size();
 *
 * 制約 / 注意:
 *   - map使用のためやや重い（必要ならunordered_mapや配列に変更可能）
 *   - indexは出現位置（末尾）を保持
 *
 * verified:
 *   - https://judge.yosupo.jp/submission/365550
 */
namespace kwm_t::string {

template <typename T = char>
struct PalindromicTree {

public:
	struct Node {
		std::map<T, int> link;  // 子ノード
		int suffixLink;         // 最長回文suffix
		int length;             // 回文長
		std::vector<int> index; // 出現位置（末尾）
		int deltaLink;          // 差分が異なるsuffix

		Node() = default;
		Node(int suffixLink_, int length_)
			: suffixLink(suffixLink_), length(length_), deltaLink(-1) {}
	};

	std::vector<Node> nodes;
	int pointer;        // 現在のsuffix回文
	std::vector<T> vs;  // 文字列

private:
	/**
	 * @brief 直前の回文を辿って拡張可能なノードを探す
	 */
	int find_prev_palindrome(int cur) const {
		int pos = (int)vs.size() - 1;
		while (true) {
			int rev = pos - nodes[cur].length - 1;
			if (rev >= 0 && vs[rev] == vs.back()) break;
			cur = nodes[cur].suffixLink;
		}
		return cur;
	}

	/**
	 * @brief 長さ差分
	 */
	int diff(int index) const {
		if (nodes[index].suffixLink <= 0) return -1;
		return nodes[index].length - nodes[nodes[index].suffixLink].length;
	}

public:
	/**
	 * @brief 初期化
	 *
	 * nodes[0]: 長さ -1 のダミー
	 * nodes[1]: 長さ 0 の空文字
	 */
	PalindromicTree() : pointer(0) {
		nodes.emplace_back(0, -1);
		nodes.emplace_back(0, 0);
	}

	explicit PalindromicTree(const std::string& s) : PalindromicTree() {
		add(s);
	}

	/**
	 * @brief 文字列追加
	 */
	void add(const std::string& s) {
		for (const auto& c : s) add(c);
	}

	/**
	 * @brief 1文字追加
	 * @return 対応するノードID
	 */
	int add(const T& c) {
		int idx = (int)vs.size();
		vs.push_back(c);

		int cur = find_prev_palindrome(pointer);
		auto res = nodes[cur].link.insert({ c, (int)nodes.size() });

		pointer = res.first->second;

		if (res.second) {
			// 新ノード作成
			nodes.emplace_back(-1, nodes[cur].length + 2);

			// suffixLink設定
			if (nodes.back().length == 1) {
				nodes.back().suffixLink = 1;
			}
			else {
				int link = find_prev_palindrome(nodes[cur].suffixLink);
				nodes.back().suffixLink = nodes[link].link[c];
			}

			// deltaLink設定
			if (diff(pointer) == diff(nodes.back().suffixLink)) {
				nodes.back().deltaLink = nodes[nodes.back().suffixLink].deltaLink;
			}
			else {
				nodes.back().deltaLink = nodes.back().suffixLink;
			}
		}

		nodes[pointer].index.emplace_back(idx);
		return pointer;
	}

	/**
	 * @brief 回文DP更新（add直後に使用）
	 *
	 * @tparam I 初期化関数
	 * @tparam U 遷移関数
	 *
	 * init(node_id, position)
	 * apply(node_id, suffixLink)
	 */
	template <typename I, typename U>
	std::vector<int> updateDp(const I& init, const U& apply) {
		int i = (int)vs.size() - 1;
		int id = pointer;

		std::vector<int> updated;

		while (nodes[id].length > 0) {
			init(id, i + 1 - nodes[nodes[id].deltaLink].length - diff(id));

			if (nodes[id].suffixLink != nodes[id].deltaLink) {
				apply(id, nodes[id].suffixLink);
			}

			updated.push_back(id);
			id = nodes[id].deltaLink;
		}

		return updated;
	}

	/**
	 * @brief ノード数
	 */
	int size() const {
		return (int)nodes.size();
	}

	/**
	 * @brief ノードアクセス
	 */
	Node& operator[](int index) {
		return nodes[index];
	}

	const Node& operator[](int index) const {
		return nodes[index];
	}
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_PALINDROMIC_TREE_HPP