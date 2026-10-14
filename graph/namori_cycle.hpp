/**
 * @file namori_cycle.hpp
 * @brief ナモリグラフのサイクル
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/namori_cycle.hpp
 */
#ifndef KWM_T_GRAPH_NAMORI_CYCLE_HPP
#define KWM_T_GRAPH_NAMORI_CYCLE_HPP

#include <vector>
#include <queue>

/**
 * @brief なもりグラフのサイクル抽出
 *
 * 典型用途:
 *   - なもりグラフ（木 + 1本の閉路）から閉路部分を取り出す
 *   - 各頂点がサイクル上かどうかの判定の前処理
 *
 * 計算量:
 *   - O(N)
 *
 * @param to 無向グラフの隣接リスト
 *
 * 制約 / 注意:
 *   - グラフは連結で「なもりグラフ」であること
 *   - 多重辺・自己ループは想定していない
 *
 * 戻り値:
 *   - サイクル上の頂点列（順序付き）
 *
 * 使用例:
 *   auto cycle = namori_cycle(g);
 *
 * verified:
 *   - https://atcoder.jp/contests/KeioPC2025/submissions/74910804
 */
namespace kwm_t::graph {

/**
 * @brief なもりグラフのサイクル頂点を順序付きで取得
 */
std::vector<int> namori_cycle(const std::vector<std::vector<int>>& to) {
    int n = to.size();

    std::vector<int> deg(n);
    for (int i = 0; i < n; i++) {
        deg[i] = to[i].size();
    }

    std::queue<int> q;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 1) q.push(i);
    }

    // サイクル上かどうか
    std::vector<int> in_cycle(n, 1);

    // 葉を削る
    while (!q.empty()) {
        int v = q.front(); q.pop();
        in_cycle[v] = 0;

        for (int nv : to[v]) {
            if (--deg[nv] == 1) {
                q.push(nv);
            }
        }
    }

    // サイクルの開始点
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (in_cycle[i]) {
            start = i;
            break;
        }
    }

    // サイクル復元
    std::vector<int> cycle;
    int prev = -1;
    int cur = start;

    while (true) {
        cycle.push_back(cur);

        int nxt = -1;
        for (int nv : to[cur]) {
            if (in_cycle[nv] && nv != prev) {
                nxt = nv;
                break;
            }
        }

        if (nxt == -1 || nxt == start) break;

        prev = cur;
        cur = nxt;
    }

    return cycle;
}

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_NAMORI_CYCLE_HPP