#ifndef KWM_T_GRAPH_EULERIAN_TRAIL_HPP
#define KWM_T_GRAPH_EULERIAN_TRAIL_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::graph {

/**
 * @brief 開始点決定
 */
int find_eulerian_start(
    const std::vector<std::vector<std::pair<int,int>>> &graph,
    bool directed
) {
    int n = graph.size();

    std::vector<int> indeg(n, 0), outdeg(n, 0);

    for (int v = 0; v < n; ++v) {
        for (auto [to, _] : graph[v]) {
            outdeg[v]++;
            indeg[to]++;
        }
    }

    if (directed) {
        int s = -1, t = -1;
        for (int i = 0; i < n; ++i) {
            if (outdeg[i] - indeg[i] == 1) {
                if (s != -1) return -1;
                s = i;
            } else if (indeg[i] - outdeg[i] == 1) {
                if (t != -1) return -1;
                t = i;
            } else if (indeg[i] != outdeg[i]) {
                return -1;
            }
        }
        if (s != -1) return s;

        for (int i = 0; i < n; ++i) {
            if (outdeg[i] > 0) return i;
        }
        return 0;
    } else {
        int start = -1;
        int odd = 0;

        for (int i = 0; i < n; ++i) {
            if ((int)graph[i].size() % 2 == 1) {
                odd++;
                start = i;
            }
        }

        if (!(odd == 0 || odd == 2)) return -1;

        if (start != -1) return start;

        for (int i = 0; i < n; ++i) {
            if (!graph[i].empty()) return i;
        }
        return 0;
    }
}

/**
 * @brief オイラー路 / オイラー閉路の復元（Hierholzer）
 *
 * @details
 * graph[v] = { {to, edge_id}, ... }
 *
 * - edge_id は [0, edge_count) の一意な番号
 * - 無向グラフの場合は「両方向に同じ edge_id を貼る」
 *
 * @param graph 隣接リスト
 * @param edge_count 辺数
 * @param directed 有向グラフかどうか
 * @param start 開始頂点（-1なら自動）
 * @return オイラー路（頂点列）。存在しない場合は空
 *
 * @note
 * 計算量: O(V + E)
 * 
 * Verified:
 *  https://atcoder.jp/contests/codequeen2025-final-Public/submissions/74426816
 */  
std::vector<int> eulerian_trail(
    const std::vector<std::vector<std::pair<int,int>>> &graph,
    int edge_count,
    bool directed = false,
    int start = -1
) {
    if (start == -1) {
        start = find_eulerian_start(graph, directed);
        if (start == -1) return {};
    }

    std::vector<bool> used(edge_count, false);
    std::vector<int> res;

    auto dfs = [&](auto&& self, int v) -> void {
        for (auto [to, id] : graph[v]) {
            if (used[id]) continue;
            used[id] = true;
            self(self, to);
        }
        res.push_back(v);
    };

    dfs(dfs, start);

    if ((int)res.size() != edge_count + 1) return {};

    std::reverse(res.begin(), res.end());
    return res;
}

} // namespace kwm_t::graph

#endif