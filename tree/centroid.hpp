/**
 * @file centroid.hpp
 * @brief 木の重心
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/centroid.hpp
 */
#ifndef KWM_T_TREE_CENTROID_HPP
#define KWM_T_TREE_CENTROID_HPP

#include <vector>

namespace kwm_t::tree {

/**
 * @brief 木の重心を求める
 *
 * 典型用途:
 *  - 重心分解
 *  - 木のバランス分割
 *
 * 計算量:
 *  - O(N)
 *
 * @param g 隣接リスト（無向木）
 * @return vector<int> 重心（1 or 2個）
 *
 * 制約 / 注意:
 *  - 木であること（連結 & 辺数 = N-1）
 *
 * verified:
 * 　- https://atcoder.jp/contests/abc453/submissions/74909500
 *   - https://atcoder.jp/contests/abc362/submissions/74909688
 */
std::vector<int> centroid(const std::vector<std::vector<int>>& g) {
    int n = g.size();
    std::vector<int> res;
    res.reserve(2);

    auto dfs = [&](auto&& self, int v, int p) -> int {
        int sz = 1;
        bool ok = true;

        for (int to : g[v]) {
            if (to == p) continue;
            int c = self(self, to, v);
            sz += c;
            if (c > n / 2) ok = false;
        }

        if (n - sz > n / 2) ok = false;

        if (ok) res.push_back(v);
        return sz;
    };

    dfs(dfs, 0, -1);
    return res;
}

} // namespace kwm_t::tree

#endif