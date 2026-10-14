/**
 * @file sqrt_tree.hpp
 * @brief Sqrt Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/sqrt_tree.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_SQRT_TREE_HPP
#define KWM_T_DATA_STRUCTURE_SQRT_TREE_HPP

#include <vector>
#include <cassert>

namespace kwm_t::data_structure {

/**
 * @brief Sqrt Tree（区間クエリ: 静的配列・結合演算）
 *
 * 配列に対する区間クエリを O(1) で処理するデータ構造。
 * Disjoint Sparse Table と同様に「非冪等」演算にも対応可能。
 *
 * 内部では複数レベルに分割し、
 * - prefix
 * - suffix
 * - between（ブロック間）
 * を前計算することで高速クエリを実現する。
 *
 * ---
 * 典型用途:
 *   - RMQ（min/max）
 *   - 区間和 / 区間積
 *   - GCD
 *   - 非可換モノイド（順序付き演算）
 *
 * ---
 * 計算量:
 *   - 構築: O(N log log N)
 *   - クエリ: O(1)
 *   - メモリ: O(N log log N)
 *
 * ---
 * @tparam T
 *   要素型
 *
 * @tparam op
 *   結合的二項演算（モノイドであることが望ましい）
 *
 * ---
 * @param l 左端（含む）
 * @param r 右端（含まない）
 *
 * ---
 * 制約 / 注意:
 *   - 静的配列（更新不可）
 *   - op は結合的である必要がある
 *   - 単位元は不要（DSTと同様）
 *   - 非可換でもOK
 *
 * ---
 * 使用例:
 * @code
 * int op(int a, int b) { return min(a, b); }
 * std::vector<int> v = {1, 3, 2, 5, 4};
 * SqrtTree<int, op> st(v);
 * auto ans = st.query(1, 4); // min(3,2,5) = 2
 * @endcode
 *
 * ---
 * verified:
 *   - https://judge.yosupo.jp/submission/365527
 */
template <typename T, T(*op)(T, T)>
struct SqrtTree {
private:
	int n;
	std::vector<T> data;

	// bit長 → level対応
	std::vector<int> bit_to_level;

	// 各levelのブロックサイズ(log)
	std::vector<int> sz_log;

	// 前計算
	std::vector<std::vector<T>> prefix;
	std::vector<std::vector<T>> suffix;
	std::vector<std::vector<T>> between;

public:
	SqrtTree() = default;

	explicit SqrtTree(const std::vector<T>& a)
		: n((int)a.size()), data(a) {
		build();
	}

	/**
	 * @brief 区間クエリ [l, r)
	 */
	T query(int l, int r) const {
		assert(0 <= l && l < r && r <= n);

		--r;
		if (l == r) return data[l];

		int level = bit_to_level[top_bit(l ^ r)];

		// 同ブロック
		if (level == -1) return op(data[l], data[r]);

		int k1 = sz_log[level];
		int k2 = k1 / 2;

		T result = suffix[level][l];

		int a = 1 + (l >> k2);
		int b = (r >> k2) - 1;
		int m = 1 << (k1 - k2);

		if (a <= b) {
			int k = a >> (k1 - k2);
			a -= k * m;
			b -= k * m;

			int idx = k * (m * (m + 1) / 2);
			idx += (b + 1) * b / 2;
			idx += b - a;

			result = op(result, between[level][idx]);
		}

		result = op(result, prefix[level][r]);
		return result;
	}

private:
	void build() {
		int bit = 0;
		while ((1 << bit) < n) ++bit;

		bit_to_level.assign(bit, -1);

		// レベル構築
		while (bit > 1) {
			for (int i = bit / 2; i < bit; ++i) {
				bit_to_level[i] = (int)sz_log.size();
			}
			sz_log.emplace_back(bit);
			bit /= 2;
		}

		for (int i = 0; i < (int)sz_log.size(); ++i) {
			build_core(i);
		}
	}

	void build_core(int level) {
		int k1 = sz_log[level];
		int k2 = k1 / 2;
		int mask2 = (1 << k2) - 1;

		// prefix
		std::vector<T> p = data;
		for (int i = 0; i < n; ++i) {
			if (i & mask2) {
				p[i] = op(p[i - 1], data[i]);
			}
		}
		prefix.emplace_back(std::move(p));

		// suffix
		std::vector<T> s = data;
		for (int i = n - 1; i > 0; --i) {
			if (i & mask2) {
				s[i - 1] = op(s[i], data[i - 1]);
			}
		}
		suffix.emplace_back(std::move(s));

		// block product
		int n2 = n >> k2;
		std::vector<T> blk_prod(n2);
		for (int i = 0; i < n2; ++i) {
			blk_prod[i] = suffix[level][i << k2];
		}

		// between
		std::vector<T> b;
		for (int j = 0; j < n2; ++j) {
			int start = (j >> (k1 - k2)) << (k1 - k2);

			b.emplace_back(blk_prod[j]);

			for (int i = j - 1; i >= start; --i) {
				b.emplace_back(op(b.back(), blk_prod[i]));
			}
		}

		between.emplace_back(std::move(b));
	}

	// 最上位bit
	int top_bit(int x) const {
		return (x == 0 ? -1 : 31 - __builtin_clz(x));
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_SQRT_TREE_HPP