/**
 * @file weighted_union_find.hpp
 * @brief Weighted Union-Find
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/disjoint_set_union/weighted_union_find.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_WEIGHTED_UNION_FIND_HPP
#define KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_WEIGHTED_UNION_FIND_HPP

#include <vector>

/**
 * @brief ポテンシャル付きUnion-Find（群対応）
 *
 * 典型用途:
 *   - 差分制約（x_j - x_i = w）
 *   - XOR制約
 *
 * 計算量:
 *   - ほぼ O(α(N))
 *
 * @tparam T 型
 * @tparam op 二項演算
 * @tparam inv 逆元
 * @tparam e 単位元
 * verified
 * - https://atcoder.jp/contests/past15-open/submissions/74912843
 * - https://atcoder.jp/contests/abc373/submissions/74912818
 * - https://atcoder.jp/contests/abc328/submissions/74913142
 */
namespace kwm_t::data_structure::disjoint_set_union {

template <typename T,
	T(*op)(T, T),
	T(*inv)(T),
	T(*e)()>
	class WeightedUnionFind {
	public:
		explicit WeightedUnionFind(int n)
			: n(n), par_or_size(n, -1), weight(n, e()), ng(n, false) {}

		int leader(int v) {
			if (par_or_size[v] < 0) return v;
			int p = par_or_size[v];
			int r = leader(p);
			weight[v] = op(weight[v], weight[p]);
			return par_or_size[v] = r;
		}

		bool same(int u, int v) {
			return leader(u) == leader(v);
		}

		/**
		 * @brief weight[v] = weight[u] ⊕ w を追加
		 */
		bool merge(int u, int v, T w) {
			int ru = leader(u);
			int rv = leader(v);

			// w' = w ⊕ weight[u]^{-1} ⊕ weight[v]
			w = op(op(weight[u], w), inv(weight[v]));

			if (ru == rv) {
				if (w != e()) ng[ru] = true;
				return w == e();
			}

			if (-par_or_size[ru] < -par_or_size[rv]) {
				std::swap(ru, rv);
				w = inv(w);
			}

			par_or_size[ru] += par_or_size[rv];
			par_or_size[rv] = ru;
			weight[rv] = w;

			ng[ru] = ng[ru] | ng[rv];
			return true;
		}

		int size(int v) {
			return -par_or_size[leader(v)];
		}

		/**
		 * @brief u → v の差分（v ⊖ u）
		 */
		T diff(int u, int v) {
			if (!same(u, v)) return e();
			return op(inv(weight[u]), weight[v]);
		}

		/**
		 * @brief 矛盾しているか
		 */
		bool is_invalid(int v) {
			return ng[leader(v)];
		}

	private:
		int n;
		std::vector<int> par_or_size;
		std::vector<T> weight;
		std::vector<bool> ng;
};

} // namespace kwm_t::data_structure::disjoint_set_union

#endif