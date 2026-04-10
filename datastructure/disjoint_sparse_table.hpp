#ifndef KWM_T_DATA_STRUCTURE_DISJOINT_SPARSE_TABLE_HPP
#define KWM_T_DATA_STRUCTURE_DISJOINT_SPARSE_TABLE_HPP

#include <vector>
#include <algorithm>
#include <cassert>

namespace kwm_t::data_structure {
/**
 * @brief Disjoint Sparse Table（静的区間積データ構造）
 *
 * 各レベルごとに左右からの累積演算結果を事前計算し、
 * 任意区間クエリを O(1) で処理するデータ構造。
 *
 * Segment Tree と異なり「結合可能な演算（冪等でなくても可）」に対して
 * 静的配列上で高速に動作する。
 *
 * ----------------------------------------
 * 典型用途:
 * - 区間 min / max
 * - 区間 gcd
 * - 区間 bitwise AND / OR / XOR
 * - 冪等でないが結合則を持つ演算の静的クエリ
 *
 * （※更新は不可）
 *
 * ----------------------------------------
 * 計算量:
 * - 前計算: O(N log N)
 * - クエリ: O(1)
 * - メモリ: O(N log N)
 *
 * ----------------------------------------
 * @tparam T
 *         要素型
 * @tparam op
 *         結合演算（結合則を満たす必要あり）
 *         op(op(a,b),c) == op(a,op(b,c))
 *
 * ----------------------------------------
 * @param a
 *        初期配列（静的）
 *
 * ----------------------------------------
 * 制約 / 注意:
 * - 静的データ構造（更新不可）
 * - op は結合則が必要（非結合だと壊れる）
 * - サイズは 2冪でなくても動作する
 * - Segment Tree より速いが汎用性は低い
 *
 * verified:
 * - https://judge.yosupo.jp/submission/365513
 */
template <typename T, T(*op)(T, T)>
struct DisjointSparseTable {
private:
	int n;
	std::vector<T> v;
	std::vector<std::vector<T>> table;
	std::vector<int> log_table;

public:
	DisjointSparseTable(const std::vector<T>& a) : n((int)a.size()), v(a) {
		build();
	}

	// [l, r)
	T query(int l, int r) const {
		assert(0 <= l && l < r && r <= n);
		if (l + 1 == r) return v[l];
		int k = log_table[l ^ (r - 1)];
		return op(table[k][l], table[k][r - 1]);
	}

private:
	void build() {
		int log = 0;
		while ((1 << log) < n) ++log;

		table.assign(log, v);

		for (int bit = 0; bit < log; ++bit) {
			int width = 1 << bit;

			for (int a = 0; a < n; a += 2 * width) {
				int c = std::min(n, a + width);
				int b = std::min(n, a + 2 * width);

				// left half (decreasing)
				for (int i = c - 2; i >= a; --i) {
					table[bit][i] = op(v[i], table[bit][i + 1]);
				}

				// right half (increasing)
				for (int i = c + 1; i < b; ++i) {
					table[bit][i] = op(v[i], table[bit][i - 1]);
				}
			}
		}

		log_table.assign(1 << log, 0);
		for (int bit = 0; bit < log; ++bit) {
			for (int i = (1 << bit); i < (1 << (bit + 1)) && i < (int)log_table.size(); ++i) {
				log_table[i] = bit;
			}
		}
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_DISJOINT_SPARSE_TABLE_HPP