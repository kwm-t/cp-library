/**
 * @file mo_on_tree.hpp
 * @brief 木上の Mo's algorithm
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/mo_on_tree.hpp
 */
#ifndef KWM_T_ALGORITHM_MO_ON_TREE_HPP
#define KWM_T_ALGORITHM_MO_ON_TREE_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

/**
 * @brief 木上の Mo's Algorithm
 *
 * Euler Tour によって木上のパスクエリを区間クエリへ変換し、
 * Mo's Algorithm によって処理する。
 *
 * 頂点上の値、辺上の値、およびその両方を対象にできる。
 *
 * 辺は (頂点, 辺 ID) の形で隣接リストを与える。
 * 同じ無向辺の両方向には同じ辺 ID を設定する。
 *
 * クエリは頂点対 (u, v) で指定し、u から v までのパスを対象とする。
 *
 * API:
 *   run_vertex(add_vertex, remove_vertex, output)
 *     頂点のみを対象とする。
 *
 *   run_edge(add_edge, remove_edge, output)
 *     辺のみを対象とする。
 *
 *   run(add_vertex, remove_vertex, add_edge, remove_edge, output)
 *     頂点と辺の両方を対象とする。
 *
 * 計算量:
 *   前処理: O(N log N)
 *   クエリ順序決定: O(Q log Q)
 *   Mo 部分: O((N + Q) sqrt(N))
 *   LCA: O(log N) / query
 *
 * 制約 / 注意:
 *   - 頂点は 0-indexed。
 *   - 根は頂点 0。
 *   - graph は木である必要がある。
 *   - 辺 ID は各辺に対して一意である必要がある。
 *
 * 使用例:
 *
 *   std::vector<std::vector<std::pair<int, int>>> graph(n);
 *
 *   graph[u].emplace_back(v, edge_id);
 *   graph[v].emplace_back(u, edge_id);
 *
 *   std::vector<std::pair<int, int>> queries;
 *
 *   kwm_t::algorithm::MoOnTree mo(graph, queries);
 *
 *   mo.run_vertex(
 *       [&](int v) {
 *           // 頂点 v を追加
 *       },
 *       [&](int v) {
 *           // 頂点 v を削除
 *       },
 *       [&](int qi) {
 *           // クエリ qi の答えを確定
 *       }
 *   );
 *
 * verified:
 *   https://atcoder.jp/contests/abc477/submissions/79568639
 */
namespace kwm_t::algorithm {

class MoOnTree {
private:
	int n;
	int q;
	int block_size;
	int log;

	std::vector<std::vector<std::pair<int, int>>> graph;

	std::vector<int> parent;
	std::vector<int> parent_edge;
	std::vector<int> depth;

	std::vector<std::vector<int>> up;

	std::vector<int> euler;
	std::vector<int> tin;

	std::vector<int> left;
	std::vector<int> right;
	std::vector<int> lca;

	std::vector<int> order;

private:
	static std::vector<std::vector<std::pair<int, int>>> to_graph(
		const std::vector<std::vector<int>>& graph
	) {
		std::vector<std::vector<std::pair<int, int>>> result(graph.size());

		for (int v = 0; v < static_cast<int>(graph.size()); ++v) {
			for (int to : graph[v]) {
				result[v].emplace_back(to, -1);
			}
		}

		return result;
	}

	void build_tree() {
		parent.assign(n, -1);
		parent_edge.assign(n, -1);
		depth.assign(n, 0);
		tin.assign(n, -1);

		euler.clear();
		euler.reserve(2 * n);

		std::vector<int> iterator(n, 0);
		std::vector<int> stack;
		stack.reserve(n);

		parent[0] = 0;
		stack.push_back(0);

		while (!stack.empty()) {
			int v = stack.back();

			if (iterator[v] == 0) {
				tin[v] = static_cast<int>(euler.size());
				euler.push_back(v);
			}

			if (iterator[v] == static_cast<int>(graph[v].size())) {
				euler.push_back(v);
				stack.pop_back();
				continue;
			}

			auto [to, edge_id] = graph[v][iterator[v]++];

			if (to == parent[v]) continue;

			parent[to] = v;
			parent_edge[to] = edge_id;
			depth[to] = depth[v] + 1;

			stack.push_back(to);
		}
	}

	void build_lca() {
		log = 1;

		while ((1LL << log) <= n) ++log;

		up.assign(log, std::vector<int>(n));

		for (int v = 0; v < n; ++v) up[0][v] = parent[v];

		for (int k = 1; k < log; ++k) {
			for (int v = 0; v < n; ++v) {
				up[k][v] = up[k - 1][up[k - 1][v]];
			}
		}
	}

	int get_lca(int u, int v) const {
		if (depth[u] < depth[v])std::swap(u, v);

		int diff = depth[u] - depth[v];

		for (int k = 0; k < log; ++k) {
			if ((diff >> k) & 1) u = up[k][u];
		}

		if (u == v) return u;

		for (int k = log - 1; k >= 0; --k) {
			if (up[k][u] != up[k][v]) {
				u = up[k][u];
				v = up[k][v];
			}
		}

		return parent[u];
	}

	void set_queries(
		const std::vector<std::pair<int, int>>& queries
	) {
		q = static_cast<int>(queries.size());

		left.resize(q);
		right.resize(q);
		lca.resize(q);

		for (int i = 0; i < q; ++i) {
			int u = queries[i].first;
			int v = queries[i].second;

			if (tin[u] > tin[v]) std::swap(u, v);

			/*
			 * Euler Tour:
			 *
			 *   E[tin[u]], ..., E[tin[v]]
			 *
			 * のうち、
			 *
			 *   E[tin[u] + 1], ..., E[tin[v]]
			 *
			 * の奇数回出現する頂点が LCA を除くパス頂点になる。
			 *
			 * 内部では半開区間 [left, right) とする。
			 */
			left[i] = tin[u] + 1;
			right[i] = tin[v] + 1;

			lca[i] = get_lca(u, v);
		}

		block_size = std::max(
			1,
			int(double(2LL * n) / std::sqrt(std::max(1, q)))
		);

		order.resize(q);
		std::iota(order.begin(), order.end(), 0);
	}

	template<class AddVertex, class RemoveVertex,
		class AddEdge, class RemoveEdge, class Out>
	void run_impl(
		AddVertex add_vertex,
		RemoveVertex remove_vertex,
		AddEdge add_edge,
		RemoveEdge remove_edge,
		Out output
	) {
		std::sort(order.begin(), order.end(), [&](int a, int b) {
			int ablock = left[a] / block_size;
			int bblock = left[b] / block_size;
			if (ablock != bblock) return ablock < bblock;
			if (ablock & 1) return right[a] < right[b];
			return right[a] > right[b];
		});

		/*
		 * parity[v] == 1:
		 *   Euler 区間内で v が奇数回現れている。
		 *
		 * このとき v はパス上に存在する。
		 *
		 * また、v != 0 なら
		 * parent_edge[v] がパス上の辺になる。
		 */
		std::vector<unsigned char> parity(n, 0);

		auto toggle = [&](int pos) {
			int v = euler[pos];
			parity[v] ^= 1;
			if (parity[v]) {
				add_vertex(v);
				if (v != 0) add_edge(parent_edge[v]);
			}
			else {
				remove_vertex(v);
				if (v != 0) remove_edge(parent_edge[v]);
			}
		};

		int nl = 0;
		int nr = 0;

		for (int idx : order) {
			while (nl > left[idx]) toggle(--nl);
			while (nr < right[idx]) toggle(nr++);
			while (nl < left[idx]) toggle(nl++);
			while (nr > right[idx]) toggle(--nr);

			/*
			 * Euler 区間では LCA が除外されているため、
			 * 頂点パスの場合だけ LCA を一時的に追加する。
			 *
			 * 辺については LCA の親側の辺を追加してはいけないので、
			 * edge callback はここでは呼ばない。
			 */
			add_vertex(lca[idx]);
			output(idx);
			remove_vertex(lca[idx]);
		}
	}

public:
	/**
	 * @brief 木上の Mo を構築する。
	 *
	 * @param graph (頂点, 辺 ID) の隣接リスト
	 * @param queries 頂点対によるクエリ
	 */
	MoOnTree(
		const std::vector<std::vector<int>>& graph,
		const std::vector<std::pair<int, int>>& queries
	)
		: MoOnTree(to_graph(graph), queries) {}
	MoOnTree(
		const std::vector<std::vector<std::pair<int, int>>>& graph,
		const std::vector<std::pair<int, int>>& queries
	)
		: n(static_cast<int>(graph.size())),
		q(0),
		block_size(1),
		log(0),
		graph(graph) {
		build_tree();
		build_lca();
		set_queries(queries);
	}

	/**
	 * @brief クエリを設定する。
	 *
	 * 同じ木に対してクエリだけ変更する場合に使用する。
	 */
	void query(const std::vector<std::pair<int, int>>& queries) { set_queries(queries); }

	/**
	 * @brief 頂点数を取得する。
	 */
	int size() const { return n; }

	/**
	 * @brief クエリ数を取得する。
	 */
	int query_size() const { return q; }

	/**
	 * @brief 頂点 v の親を取得する。
	 */
	int parent_of(int v) const { return parent[v]; }

	/**
	 * @brief 頂点 v と親を結ぶ辺の ID を取得する。
	 *
	 * 根の場合は -1。
	 */
	int parent_edge_of(int v) const { return parent_edge[v]; }

	/**
	 * @brief 頂点 v の深さを取得する。
	 */
	int depth_of(int v) const { return depth[v]; }

	/**
	 * @brief 頂点 v の Euler Tour 上の開始位置を取得する。
	 */
	int tin_of(int v) const { return tin[v]; }

	/**
	 * @brief 頂点のみを対象として Mo を実行する。
	 *
	 * add / remove に渡されるのは頂点 ID。
	 *
	 * 内部では辺用 callback に空関数を渡して run() を呼ぶ。
	 */
	template<class AddVertex, class RemoveVertex, class Out>
	void run_vertex(AddVertex add_vertex, RemoveVertex remove_vertex, Out output) {
		run(add_vertex, remove_vertex, [](int) {}, [](int) {}, output);
	}

	/**
	 * @brief 辺のみを対象として Mo を実行する。
	 *
	 * add / remove に渡されるのは辺 ID。
	 *
	 * 内部では頂点用 callback に空関数を渡して run() を呼ぶ。
	 */
	template<class AddEdge, class RemoveEdge, class Out>
	void run_edge(AddEdge add_edge, RemoveEdge remove_edge, Out output) {
		run([](int) {}, [](int) {}, add_edge, remove_edge, output);
	}

	/**
	 * @brief 頂点と辺の両方を対象として Mo を実行する。
	 *
	 * add_vertex / remove_vertex:
	 *   頂点 ID を受け取る。
	 *
	 * add_edge / remove_edge:
	 *   辺 ID を受け取る。
	 *
	 * 頂点のみ、辺のみの場合も、この run() を本体として使用する。
	 */
	template<class AddVertex, class RemoveVertex, class AddEdge, class RemoveEdge, class Out>
	void run(AddVertex add_vertex, RemoveVertex remove_vertex, AddEdge add_edge, RemoveEdge remove_edge, Out output) {
		run_impl(add_vertex, remove_vertex, add_edge, remove_edge, output);
	}
};

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_MO_ON_TREE_HPP