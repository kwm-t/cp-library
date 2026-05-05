#ifndef KWM_T_GRAPH_TOPOLOGICAL_SORT_HPP
#define KWM_T_GRAPH_TOPOLOGICAL_SORT_HPP

#include <vector>
#include <queue>


namespace kwm_t::graph {
/**
 * @brief トポロジカルソート（Kahn法）
 *
 * 典型用途:
 *   - DAG（有向非巡回グラフ）の順序付け
 *   - 依存関係の解決
 *
 * 計算量:
 *   - O(N + M)
 *
 * @param g 有向グラフの隣接リスト
 *
 * 戻り値:
 *   - トポロジカル順序（DAGでない場合はサイズ < N）
 *
 * 制約 / 注意:
 *   - 有向グラフで使用
 *
 * 使用例:
 *   auto ord = topological_sort(g);
 *   if (ord.size() != n) {
 *       // DAGではない
 *   }
 *
 * verified:
 *   - https://atcoder.jp/contests/arc197/submissions/74911405
 *   - https://atcoder.jp/contests/ndpc/submissions/75521703
 */

/**
 * @brief トポロジカルソート
 */
std::vector<int> topological_sort(const std::vector<std::vector<int>>& g) {
    int n = g.size();

    std::vector<int> indeg(n, 0);
    for (int v = 0; v < n; ++v) {
        for (int nv : g[v]) {
            indeg[nv]++;
        }
    }

    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    std::vector<int> order;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        order.push_back(v);

        for (int nv : g[v]) {
            if (--indeg[nv] == 0) {
                q.push(nv);
            }
        }
    }

    return order;
}

/**
 * @brief DAGかどうか判定
 */
bool is_dag(const std::vector<std::vector<int>>& g) {
    return topological_sort(g).size() == g.size();
}

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_TOPOLOGICAL_SORT_HPP