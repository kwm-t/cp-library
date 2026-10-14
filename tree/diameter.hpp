/**
 * @file diameter.hpp
 * @brief 木の直径
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/diameter.hpp
 */
#ifndef KWM_T_TREE_DIAMETER_HPP
#define KWM_T_TREE_DIAMETER_HPP

#include <vector>
#include <queue>

/**
 * @brief 木の直径（端点・パス・距離付き）
 *
 * 典型用途:
 *   - 直径の長さ・パス取得
 *   - 中心計算
 *   - 距離配列の再利用
 *
 * 計算量:
 *   - O(N)
 *
 * 使用例:
 *   auto d = tree_diameter(g);
 *   int len = d.length;
 *   auto path = d.path;
 * 
 * verified
 *   https://atcoder.jp/contests/past20-open/submissions/74911947
 */
namespace kwm_t::tree {

struct Diameter {
    int s, t;
    int length;
    std::vector<int> path;  // t → s
    std::vector<int> dist;  // s からの距離
};

namespace internal {

inline std::pair<int, std::vector<int>>
bfs(int s, const std::vector<std::vector<int>>& g, std::vector<int>& parent) {
    int n = g.size();
    std::vector<int> dist(n, -1);
    parent.assign(n, -1);

    std::queue<int> q;
    q.push(s);
    dist[s] = 0;

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int nv : g[v]) {
            if (dist[nv] != -1) continue;
            dist[nv] = dist[v] + 1;
            parent[nv] = v;
            q.push(nv);
        }
    }

    int far = s;
    for (int i = 0; i < n; i++) {
        if (dist[i] > dist[far]) far = i;
    }

    return { far, dist };
}

} // namespace internal

/**
 * @brief 木の直径を取得
 */
inline Diameter tree_diameter(const std::vector<std::vector<int>>& g) {
    std::vector<int> parent;

    // 1回目
    auto [s, _] = internal::bfs(0, g, parent);

    // 2回目
    auto [t, dist] = internal::bfs(s, g, parent);

    // 経路復元（t → s）
    std::vector<int> path;
    int cur = t;
    while (cur != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }

    return Diameter{
        s,
        t,
        (int)path.size() - 1,
        path,
        dist
    };
}

} // namespace kwm_t::tree

#endif // KWM_T_TREE_DIAMETER_HPP