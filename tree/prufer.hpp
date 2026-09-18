#ifndef KWM_T_TREE_PRUFER_HPP
#define KWM_T_TREE_PRUFER_HPP

#include <queue>
#include <vector>

namespace kwm_t::tree {

/**
 * @brief 木の隣接リストから Prüfer 列を生成する。
 *
 * 頂点番号は 0, 1, ..., n-1 とする。
 * 各ステップで、残っている葉のうち頂点番号が最小のものを削除し、
 * その葉の唯一の隣接頂点を列に追加する。
 * これを頂点が 2 個になるまで繰り返す。
 *
 * @param graph 木を表す隣接リスト。
 * @return 長さ n-2 の Prüfer 列。
 *
 * 計算量:
 *   O(n log n)
 *
 * 空間計算量:
 *   O(n)
 *
 * 制約 / 注意:
 *   - graph は単純な木でなければならない。
 *   - 頂点番号は 0 から n-1 まで連続しているものとする。
 *   - n <= 2 の場合、空列を返す。
 *
 * verified:
 *  https://yukicoder.me/submissions/1187823
 */
std::vector<int> prufer_encode(
	const std::vector<std::vector<int>>& graph
) {
	const int n = static_cast<int>(graph.size());

	std::vector<int> degree(n);
	std::vector<bool> removed(n, false);
	std::priority_queue<
		int,
		std::vector<int>,
		std::greater<int>
	> leaves;

	for (int v = 0; v < n; ++v) {
		degree[v] = static_cast<int>(graph[v].size());
		if (degree[v] == 1) {
			leaves.push(v);
		}
	}

	std::vector<int> code;
	code.reserve(n >= 2 ? n - 2 : 0);

	for (int i = 0; i < n - 2; ++i) {
		const int leaf = leaves.top();
		leaves.pop();
		removed[leaf] = true;

		int parent = -1;
		for (const int v : graph[leaf]) {
			if (!removed[v]) {
				parent = v;
				break;
			}
		}

		code.push_back(parent);

		--degree[parent];
		if (degree[parent] == 1) {
			leaves.push(parent);
		}
	}

	return code;
}

/**
 * @brief Prüfer 列から木を復元する。
 *
 * code の長さを n-2 とすると、n 頂点の木を復元する。
 * 頂点番号は 0, 1, ..., n-1 とする。
 *
 * 復元される木は、prufer_encode で使用している
 * 「最小番号の葉を削除する」という規約に対応する。
 *
 * @param code Prüfer 列。
 * @return 復元された木の隣接リスト。
 *
 * 計算量:
 *   O(n log n)
 *
 * 空間計算量:
 *   O(n)
 *
 * 制約 / 注意:
 *   - code の各要素は 0 以上 n-1 以下である必要がある。
 *   - n = code.size() + 2。
 *   - code が空なら 2 頂点 1 辺の木を返す。
 *
 * verified:
 *   https://yukicoder.me/submissions/1187823
 */
std::vector<std::vector<int>> prufer_decode(
	const std::vector<int>& code
) {
	const int n = static_cast<int>(code.size()) + 2;

	std::vector<int> degree(n, 1);

	for (const int v : code) ++degree[v];

	std::priority_queue<
		int,
		std::vector<int>,
		std::greater<int>
	> leaves;

	for (int v = 0; v < n; ++v) {
		if (degree[v] == 1) leaves.push(v);
	}

	std::vector<std::vector<int>> graph(n);

	for (const int v : code) {
		const int leaf = leaves.top();
		leaves.pop();

		graph[leaf].push_back(v);
		graph[v].push_back(leaf);

		--degree[v];
		if (degree[v] == 1) leaves.push(v);
	}

	const int u = leaves.top();
	leaves.pop();

	const int v = leaves.top();

	graph[u].push_back(v);
	graph[v].push_back(u);

	return graph;
}

} // namespace kwm_t::tree

#endif // KWM_T_TREE_PRUFER_HPP