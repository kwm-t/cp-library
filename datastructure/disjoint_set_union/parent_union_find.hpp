/**
 * @file parent_union_find.hpp
 * @brief 親方向に統合するUnion-Find
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/disjoint_set_union/parent_union_find.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_PARENT_UNION_FIND_HPP
#define KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_PARENT_UNION_FIND_HPP

#include <numeric>
#include <vector>

namespace kwm_t::data_structure::disjoint_set_union {

/**
 * @brief 親方向に代表元を統合するUnion-Find
 *
 * merge(x, p) は x の属する集合を p の属する集合に統合する。
 * 通常のUnion-Findと異なり、代表元は必ず p 側になる。
 *
 * 典型用途:
 *   - 木上で処理済みの頂点を親方向にスキップする
 *   - 木のパス上の頂点を一度だけ処理する
 *
 * 注意:
 *   - union by size / rank は行わない
 *   - 統合方向に循環が生じないこと
 * verify
 *  https://atcoder.jp/contests/agc079/submissions/80725283
 */
class ParentUnionFind {
public:
	explicit ParentUnionFind(int n) : par(n) {
		std::iota(par.begin(), par.end(), 0);
	}

	/**
	 * @brief 代表元を取得する
	 */
	int leader(int v) {
		if (par[v] == v) return v;
		return par[v] = leader(par[v]);
	}

	/**
	 * @brief 同じ集合に属するか
	 */
	bool same(int u, int v) {
		return leader(u) == leader(v);
	}

	/**
	 * @brief x 側の集合を p 側の集合に統合する
	 * @return 統合が行われたか
	 */
	bool merge(int x, int p) {
		x = leader(x);
		p = leader(p);

		if (x == p) return false;

		par[x] = p;
		return true;
	}

private:
	std::vector<int> par;
};

} // namespace kwm_t::data_structure::disjoint_set_union

#endif