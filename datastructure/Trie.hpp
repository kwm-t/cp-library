#ifndef KWM_T_DATA_STRUCTURE_TRIE_HPP
#define KWM_T_DATA_STRUCTURE_TRIE_HPP

#include <vector>
#include <functional>
#include <optional>

/**
 * @brief 汎用Trie（配列ベース）
 *
 * 任意の整数列（例：文字列, bit列）を扱うTrie
 * trans/inverseは外部注入可能
 *
 * 典型用途:
 *   - 文字列Trie
 *   - bit列Trie（BinaryTrieの代替）
 *   - 辞書順k番目
 *
 * 計算量:
 *   - 各操作 O(|S|)
 *
 * 注意:
 *   - eraseは存在する要素にのみ使うこと
 *   - inverse未指定時はvector<int>のみ返す
 *   - 問題固有の操作は関数追加して対応してください 
 * 
 * 使用例
 *   - https://atcoder.jp/contests/abc403/submissions/74526192
 *   - https://atcoder.jp/contests/abc377/submissions/74526225
 */
namespace kwm_t::data_structure {
// ---------------- デフォルトNode ----------------
struct EmptyNode {
	void add(int, int, int) {}
	void erase(int, int, int) {}
};

// ---------------- Trie本体 ----------------
template<class T = std::vector<int>, class Node = EmptyNode>
struct Trie {
private:
	int kind;

	std::vector<int> cnt;   // 部分木サイズ（必須）
	std::vector<std::vector<int>> next;
	std::vector<bool> end;
	std::vector<Node> node;

	// 外部注入
	std::function<std::vector<int>(const T&)> trans;
	std::function<T(const std::vector<int>&)> inverse;
	bool has_inverse = false;

	// 初期化
	void init() {
		cnt.assign(1, 0);
		end.assign(1, false);
		next.assign(1, std::vector<int>(kind, -1));
		node.assign(1, Node());
	}
public:
	// ---------------- コンストラクタ ----------------

	// 最小構成（そのままvector<int>使う）
	Trie(int _kind)
		: kind(_kind),
		trans([](const T& v) { return v; }) {
		init();
	}

	// transのみ
	Trie(int _kind,
		std::function<std::vector<int>(const T&)> _trans)
		: kind(_kind), trans(_trans) {
		init();
	}

	// trans + inverse
	Trie(int _kind,
		std::function<std::vector<int>(const T&)> _trans,
		std::function<T(const std::vector<int>&)> _inverse)
		: kind(_kind), trans(_trans), inverse(_inverse), has_inverse(true) {
		init();
	}

	// ---------------- 基本 ----------------

	void insert(const T& x, int id = -1) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (next[now][v[i]] == -1) {
				next[now][v[i]] = (int)cnt.size();
				cnt.emplace_back(0);
				end.push_back(false);
				next.emplace_back(std::vector<int>(kind, -1));
				node.emplace_back(Node());
			}
			now = next[now][v[i]];
			cnt[now]++;
		}
		end[now] = true;
		node[now].add(v.size() - 1, v.size(), id);
	}

	// prefixも追加
	void insert_prefix(const T& x, int id = -1) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (next[now][v[i]] == -1) {
				next[now][v[i]] = node.size();
				cnt.emplace_back(0);
				end.push_back(false);
				next.emplace_back(std::vector<int>(kind, -1));
				node.emplace_back(Node());
			}
			now = next[now][v[i]];
			cnt[now] += (int)v.size() - i;
			end[now] = true;
			node[now].add(i, v.size(), id);
		}
	}

	bool search(const T& x) const {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (next[now][v[i]] == -1) return false;
			if (cnt[next[now][v[i]]] <= 0) return false;
			now = next[now][v[i]];
		}
		return end[now];
	}

	void erase(const T& x, int id = -1) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			now = next[now][v[i]];
			cnt[now]--;
		}
		end[now] = false;
		node[now].erase(v.size() - 1, v.size(), id);
	}

	void erase_prefix(const T& x, int id = -1) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			now = next[now][v[i]];
			cnt[now]--;
			node[now].erase(i, v.size(), id);
		}
		end[now] = false;
	}

	// ---------------- kth ----------------

	std::vector<int> kth_element_vec(int k) const {
		int now = 0;
		std::vector<int> ret;
		while (true) {
			bool ok = false;
			for (int i = 0; i < kind; ++i) {
				int nxt = next[now][i];
				if (nxt == -1) continue;
				if (k <= cnt[nxt]) {
					ret.push_back(i);
					now = nxt;
					ok = true;
					break;
				}
				else {
					k -= cnt[nxt];
				}
			}
			if (!ok) break;
		}
		return ret;
	}

	// inverseがある場合だけ使える
	std::optional<T> kth_element(int k) const {
		if (!has_inverse) return std::nullopt;
		return inverse(kth_element_vec(k));
	}

	// ---------------- index ----------------

	int getIndex(const T& x) const {
		auto v = trans(x);
		int ret = 0, now = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (now != 0) {
				ret += cnt[now];
				for (int j = 0; j < kind; ++j) {
					int nxt = next[now][j];
					if (nxt != -1) ret -= cnt[nxt];
				}
			}
			for (int j = 0; j < v[i]; ++j) {
				int nxt = next[now][j];
				if (nxt != -1) ret += cnt[nxt];
			}
			if (next[now][v[i]] == -1) break;
			now = next[now][v[i]];
		}
		return ret;
	}
};

} // namespace kwm_t::data_structure
#endif // KWM_T_DATA_STRUCTURE_TRIE_HPP