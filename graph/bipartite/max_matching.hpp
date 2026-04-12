#ifndef KWM_T_GRAPH_MATCHING_BIPARTITE_MAX_MATCHING_HPP
#define KWM_T_GRAPH_MATCHING_BIPARTITE_MAX_MATCHING_HPP

#include <vector>

/**
 * @brief 二部グラフ最大マッチング（DFSベース）
 *
 * 典型用途:
 *   - 二部グラフの最大マッチング
 *   - 最小頂点被覆（Kőnigの定理と併用）
 *
 * 計算量:
 *   - O(VE)
 *
 * @param n 頂点数（左右まとめて管理）
 *
 * 制約 / 注意:
 *   - グラフは二部グラフであること
 *   - 頂点は 0..n-1
 *   - 左右の区別は特に持たない実装
 *
 * 使用例:
 *   BipartiteMaxMatching bpm(n);
 *   bpm.add_edge(u, v);
 *   int sz = bpm.max_matching();
 *
 * verified:
 *   - 典型問題（増加路）
 */
namespace kwm_t::graph::bipartite {

class BipartiteMaxMatching {
public:
    explicit BipartiteMaxMatching(int n)
        : graph(n), match(n, -1), alive(n, 1), used(n, 0), timestamp(0) {}

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    /**
     * @brief 最大マッチングサイズ
     */
    int max_matching() {
        int ret = 0;
        for (int i = 0; i < (int)graph.size(); i++) {
            if (!alive[i]) continue;
            if (match[i] == -1) {
                ++timestamp;
                if (dfs(i)) ret++;
            }
        }
        return ret;
    }

    /**
     * @brief マッチング結果
     * match[v] = 相手 or -1
     */
    const std::vector<int>& get_match() const {
        return match;
    }

private:
    std::vector<std::vector<int>> graph;
    std::vector<int> match;
    std::vector<int> alive;
    std::vector<int> used;
    int timestamp;

    bool dfs(int v) {
        used[v] = timestamp;
        for (int nv : graph[v]) {
            if (!alive[nv]) continue;
            int w = match[nv];
            if (w == -1 || (used[w] != timestamp && dfs(w))) {
                match[v] = nv;
                match[nv] = v;
                return true;
            }
        }
        return false;
    }
};

} // namespace kwm_t::graph::bipartite

#endif // KWM_T_GRAPH_MATCHING_BIPARTITE_MAX_MATCHING_HPP