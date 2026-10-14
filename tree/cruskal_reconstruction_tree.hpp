/**
 * @file cruskal_reconstruction_tree.hpp
 * @brief Kruskal Reconstruction Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/cruskal_reconstruction_tree.hpp
 */
#ifndef KWM_T_TREE_KRUSKAL_RECONSTRUCTION_TREE_HPP
#define KWM_T_TREE_KRUSKAL_RECONSTRUCTION_TREE_HPP

#include <array>
#include <numeric>
#include <vector>
#include <utility>


namespace kwm_t::tree {

/**
 * @brief Kruskal Reconstruction Tree のマージ過程を管理する。
 *
 * 各連結成分が現在どの再構築木上の頂点に対応しているかを管理し、
 * 2つの異なる連結成分をマージするたびに新しい頂点を割り当てる。
 *
 * 典型用途:
 *   - Kruskal 法のマージ過程の木構造化
 *   - bottleneck path
 *   - LCA と組み合わせた連結可能性判定
 *   - マージ過程を利用した木上クエリ
 *
 * 計算量:
 *   - コンストラクタによる構築: O(E α(V))
 *   - merge: 償却 O(α(V))
 *
 * @param n 元の頂点数
 * @param edges ソート済みの辺
 *
 * 制約 / 注意:
 *   - 元の頂点は 0, ..., n-1。
 *   - マージによって作られる頂点は n 以降。
 *   - edges は Kruskal 法で処理したい順番にあらかじめソートしておく。
 *   - merge() はKRTの木そのものを構築せず、マージ結果のみ返す。
 *   - merge() の返り値は {親, 子0, 子1}。
 *   - すでに同一連結成分なら {-1, -1, -1} を返す。
 *
 * 使用例:
 *   using KRT = kwm_t::graph::KruskalReconstructionTree;
 *
 *   KRT krt(n);
 *
 *   auto [p, c0, c1] = krt.merge(a, b);
 *   if (p == -1) continue;
 *
 *   g.push_back({});
 *   g[p].emplace_back(c0, 0);
 *   g[p].emplace_back(c1, 0);
 *
 * verified:
 *   https://atcoder.jp/contests/codequeen2026-final-Public/submissions/77992319
 */
class KruskalReconstructionTree {
public:
	using MergeResult = std::array<int, 3>;
	using Edge = std::array<int, 3>;

	explicit KruskalReconstructionTree(int n)
		: parent_(n), size_(n, 1), node_(n), next_node_(n) {
		std::iota(parent_.begin(), parent_.end(), 0);
		std::iota(node_.begin(), node_.end(), 0);
	}

	KruskalReconstructionTree(
		int n,
		const std::vector<Edge>& edges
	) : KruskalReconstructionTree(n) {
		build(edges);
	}

	/**
	 * @brief 現在の頂点 v に対応する KRT 上の頂点を返す。
	 *
	 * v が属する連結成分が、現在どの KRT 上の頂点で
	 * 表現されているかを返す。
	 */
	int node(int v) {
		return node_[leader(v)];
	}

	/**
	 * @brief 2つの頂点が属する連結成分をマージする。
	 *
	 * @param a 元グラフ上の頂点
	 * @param b 元グラフ上の頂点
	 *
	 * @return {新しい親, 子0, 子1}
	 *         すでに同一連結成分なら {-1, -1, -1}
	 */
	MergeResult merge(int a, int b) {
		int la = leader(a);
		int lb = leader(b);

		if (la == lb) return { -1, -1, -1 };

		int c0 = node_[la];
		int c1 = node_[lb];
		int p = next_node_++;

		int r = merge_dsu(la, lb);
		node_[r] = p;

		return { p, c0, c1 };
	}

private:
	std::vector<int> parent_;
	std::vector<int> size_;
	std::vector<int> node_;
	int next_node_;

	int leader(int x) {
		if (parent_[x] == x) return x;
		return parent_[x] = leader(parent_[x]);
	}

	int merge_dsu(int a, int b) {
		if (size_[a] < size_[b]) std::swap(a, b);

		parent_[b] = a;
		size_[a] += size_[b];

		return a;
	}

	void build(const std::vector<Edge>& edges) {
		for (auto [_, a, b] : edges) {
			merge(a, b);
		}
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_KRUSKAL_RECONSTRUCTION_TREE_HPP