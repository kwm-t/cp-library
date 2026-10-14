/**
 * @file eulerTour.hpp
 * @brief Euler Tour
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/eulerTour.hpp
 */
#ifndef KWM_T_TREE_EULER_TOUR_HPP
#define KWM_T_TREE_EULER_TOUR_HPP

#include <vector>

namespace kwm_t::tree {

/**
 * @brief オイラーツアー（部分木用・軽量版）
 *
 * @details
 * 各頂点 v に対して:
 *   部分木が区間 [in[v], out[v]) に対応
 *
 * 用途:
 *   - 部分木クエリ（セグ木 / BIT）
 *
 * 計算量:
 *   O(N)
 * 
 * Verified:
 *  https://judge.yosupo.jp/submission/365628
 */
struct EulerTourSubtree {
    std::vector<int> in, out;

    EulerTourSubtree() = default;

    EulerTourSubtree(const std::vector<std::vector<int>> &g, int root = 0) {
        build(g, root);
    }

    void build(const std::vector<std::vector<int>> &g, int root = 0) {
        int n = g.size();
        in.assign(n, 0);
        out.assign(n, 0);

        int timer = 0;

        auto dfs = [&](auto&& self, int v, int p) -> void {
            in[v] = timer++;
            for (int to : g[v]) {
                if (to == p) continue;
                self(self, to, v);
            }
            out[v] = timer;
        };

        dfs(dfs, root, -1);
    }
};


/**
 * @brief オイラーツアー（完全版）
 *
 * @details
 * - tour: DFSの訪問ログ（長さ ≈ 2N-1）
 * - depth: 各時点の深さ
 * - first[v]: vがtourに最初に出現する位置（LCA用）
 * - in[v]: vに最初に入った時刻（部分木用）
 * - out[v]: vの探索終了時刻（部分木用）
 *
 * 用途:
 *   - LCA（RMQ）
 *   - 部分木クエリ
 *   - パス系処理
 *
 * 計算量:
 *   O(N)
 * 
 * Verified:
 *  https://atcoder.jp/contests/awc0082/submissions/76350633
 */ 
struct EulerTourFull {
    std::vector<int> tour;   // 頂点列
    std::vector<int> depth;  // 深さ
    std::vector<int> first;  // tour上の初出位置
    std::vector<int> in;     // 入時刻
    std::vector<int> out;    // 出時刻

    EulerTourFull() = default;

    EulerTourFull(const std::vector<std::vector<int>> &g, int root = 0) {
        build(g, root);
    }

    void build(const std::vector<std::vector<int>> &g, int root = 0) {
        int n = g.size();

        tour.clear();
        depth.clear();
        first.assign(n, -1);
        in.assign(n, 0);
        out.assign(n, 0);

        tour.reserve(2 * n);
        depth.reserve(2 * n);

        int timer = 0;

        auto dfs = [&](auto&& self, int v, int p, int d) -> void {
           in[v] = timer;
            if (first[v] == -1) first[v] = (int)tour.size();

            tour.push_back(v);
            depth.push_back(d);
            timer++;

            for (int to : g[v]) {
                if (to == p) continue;
                self(self, to, v, d + 1);
				
                tour.push_back(v);
                depth.push_back(d);
                timer++;
            }

            out[v] = timer;
        };

        dfs(dfs, root, -1, 0);
    }
};

} // namespace kwm_t::tree

#endif