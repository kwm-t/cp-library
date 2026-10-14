/**
 * @file shortest_cycle.hpp
 * @brief 最短閉路
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/shortest_cycle.hpp
 */
#ifndef KWM_T_GRAPH_SHORTEST_CYCLE_HPP
#define KWM_T_GRAPH_SHORTEST_CYCLE_HPP

#include <vector>
#include <limits>
#include <algorithm>

namespace kwm_t::graph {

/**
 * @brief root を含む最短サイクルを求める（無向グラフ想定）
 *
 * @param G 隣接行列（重み付き、到達不能は INF）
 * @param root 開始頂点
 *
 * @return {最短距離, 頂点列}（存在しない場合は {INF, {}}）
 *
 * 計算量:
 *   O(N^2)
 *
 * 注意:
 *   - G は対称（無向）を想定
 *   - 自己ループは無視される
 */
template <class T>
std::pair<T, std::vector<int>>
shortest_cycle(const std::vector<std::vector<T>>& G, int root) {

    int n = G.size();
    const T INF = std::numeric_limits<T>::max() / 4;

    std::vector<T> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<int> group(n, -1); // rootから最初にどこへ行ったか
    std::vector<bool> used(n, false);

    dist[root] = 0;
    group[root] = root;

    // O(N^2) Dijkstra
    for (int t = 0; t < n; ++t) {
        int v = -1;
        T mn = INF;
        for (int i = 0; i < n; ++i) {
            if (!used[i] && dist[i] < mn) {
                mn = dist[i];
                v = i;
            }
        }
        if (v == -1) break;

        used[v] = true;

        for (int to = 0; to < n; ++to) {
            if (G[v][to] >= INF) continue;
            if (dist[to] > dist[v] + G[v][to]) {
                dist[to] = dist[v] + G[v][to];
                parent[to] = v;
                group[to] = (v == root ? to : group[v]);
            }
        }
    }

    // 最短サイクル探索
    T best = INF;
    int x = -1, y = -1;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (parent[i] == j || parent[j] == i) continue;
            if (group[i] == group[j]) continue;
            if (G[i][j] >= INF) continue;

            T cand = dist[i] + dist[j] + G[i][j];
            if (cand < best) {
                best = cand;
                x = i;
                y = j;
            }
        }
    }

    if (best == INF) {
        return {INF, {}};
    }

    // 経路復元
    std::vector<int> path;

    int a = x;
    while (a != -1) {
        path.push_back(a);
        if (a == root) break;
        a = parent[a];
    }
    std::reverse(path.begin(), path.end());

    int b = y;
    std::vector<int> tail;
    while (b != root) {
        tail.push_back(b);
        b = parent[b];
    }

    path.insert(path.end(), tail.begin(), tail.end());

    return {best, path};
}

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_SHORTEST_CYCLE_HPP