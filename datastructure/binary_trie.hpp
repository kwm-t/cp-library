#ifndef KWM_T_DATA_STRUCTURE_BINARY_TRIE_HPP
#define KWM_T_DATA_STRUCTURE_BINARY_TRIE_HPP

#include <array>
#include <vector>
#include <type_traits>

/**
 * @brief Binary Trie（XOR系クエリ対応Trie）
 *
 * 整数を2進表現として管理するTrie
 *
 * 典型用途:
 *   - xor最大/最小
 *   - k番目の値
 *   - xor制約付きカウント
 *
 * 計算量:
 *   - 各操作 O(bit)
 *
 * @tparam T int / long long を想定
 *
 * 制約 / 注意:
 *   - eraseは存在する値にのみ使用すること
 *   - bit長はコンストラクタで調整可能
 *
 * verified:
 *   - https://atcoder.jp/contests/abc451/submissions/74523720
 */
namespace kwm_t::data_structure {

template<class T = int>
struct BinaryTrie {
private:
	static constexpr int default_bit() {
		if constexpr (std::is_same_v<T, int>) return 30;
		else return 60;
	}

	int bit;
	int kind;

	std::vector<int> node;
	std::vector<std::array<int, 2>> next;
	const std::array<int, 2> e{ -1, -1 };

	std::vector<int> trans(T x) const {
		std::vector<int> ret(bit, 0);
		for (int i = 0; i < bit; ++i) {
			if ((x >> i) & 1) ret[bit - 1 - i] = 1;
		}
		return ret;
	}

	T inverse(const std::vector<int>& v) const {
		T ret = 0;
		for (int i = 0; i < (int)v.size(); ++i) {
			ret = (ret << 1) | v[i];
		}
		return ret;
	}

public:
	BinaryTrie(int _bit = default_bit(), int _kind = 2)
		: bit(_bit), kind(_kind) {
		node.resize(1);
		next.emplace_back(e);
	}

	static int calc_bit(T max_val) {
		int b = 0;
		while ((T(1) << b) <= max_val) b++;
		return b;
	}

	void insert(T x) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < bit; ++i) {
			if (next[now][v[i]] == -1) {
				next[now][v[i]] = node.size();
				node.emplace_back(0);
				next.emplace_back(e);
			}
			now = next[now][v[i]];
			node[now]++;
		}
	}

	void erase(T x) {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < bit; ++i) {
			now = next[now][v[i]];
			node[now]--;
		}
	}

	bool search(T x) const {
		auto v = trans(x);
		int now = 0;
		for (int i = 0; i < bit; ++i) {
			if (next[now][v[i]] == -1) return false;
			if (node[next[now][v[i]]] <= 0) return false;
			now = next[now][v[i]];
		}
		int cnt = node[now];
		for (int i = 0; i < kind; ++i) {
			if (next[now][i] != -1) cnt -= node[next[now][i]];
		}
		return cnt > 0;
	}

	T kth_element(int k) const {
		int now = 0;
		std::vector<int> v;
		while (true) {
			bool ok = false;
			for (int i = 0; i < kind; ++i) {
				if (next[now][i] == -1) continue;
				if (k <= node[next[now][i]]) {
					v.push_back(i);
					now = next[now][i];
					ok = true;
					break;
				} else {
					k -= node[next[now][i]];
				}
			}
			if (!ok) break;
		}
		return inverse(v);
	}

	T xor_max(T x) const {
		auto v = trans(x);
		std::vector<int> ret;
		int now = 0;
		for (int i = 0; i < bit; ++i) {
			int want[2] = {1 - v[i], v[i]};
			for (int t : want) {
				if (next[now][t] == -1) continue;
				if (node[next[now][t]] <= 0) continue;
				ret.push_back(t);
				now = next[now][t];
				break;
			}
		}
		return inverse(ret);
	}

	T xor_min(T x) const {
		auto v = trans(x);
		std::vector<int> ret;
		int now = 0;
		for (int i = 0; i < bit; ++i) {
			int want[2] = {v[i], 1 - v[i]};
			for (int t : want) {
				if (next[now][t] == -1) continue;
				if (node[next[now][t]] <= 0) continue;
				ret.push_back(t);
				now = next[now][t];
				break;
			}
		}
		return inverse(ret);
	}

	int xor_count(T x, T k) const {
		auto vx = trans(x);
		auto vk = trans(k);

		int ret = 0, now = 0;
		for (int i = 0; i < bit; ++i) {
			if (vk[i] == 0) {
				if (next[now][vx[i]] == -1) break;
				now = next[now][vx[i]];
			} else {
				if (next[now][vx[i]] != -1)
					ret += node[next[now][vx[i]]];
				if (next[now][1 - vx[i]] == -1) break;
				now = next[now][1 - vx[i]];
			}
		}
		return ret;
	}

	int over_count(T x) const {
		auto v = trans(x);
		int ret = 0, now = 0;
		for (int i = 0; i < bit; ++i) {
			if (v[i] == 0) {
				if (next[now][1] != -1)
					ret += node[next[now][1]];
			}
			if (next[now][v[i]] == -1) break;
			now = next[now][v[i]];
		}
		return ret;
	}

	int size() const {
		return node[0];
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_BINARY_TRIE_HPP