#ifndef KWM_T_TREE_AUXILIARY_TREE_HPP
#define KWM_T_TREE_AUXILIARY_TREE_HPP

#include <vector>
#include <algorithm>
#include <cassert>

namespace kwm_t::tree {

/**
 * @brief Auxiliary Tree（圧縮木・高速版）
 *
 * - 外部の graph を再利用することで O(n) 初期化を回避
 * - vs は破壊される（LCAが追加される）
 *
 * 計算量:
 *   構築: O(|S| log |S|)
 *
 * 注意:
 *   - graph は n サイズ必要
 *   - 使用頂点のみ clear される
 * verified
 *   - https://atcoder.jp/contests/abc340/submissions/75096922
 *   - https://atcoder.jp/contests/abc359/submissions/75096934
 */
class AuxiliaryTree {
public:
    explicit AuxiliaryTree(const std::vector<std::vector<int>>& g, int root = 0)
        : n(g.size()), g(g) {
        build(root);
    }

    /**
     * @brief 圧縮木構築（高速版）
     *
     * @param vs 使用頂点集合（破壊される）
     * @param tree 出力用（サイズ n）
     * @return root
     */
    int build_auxiliary_tree(std::vector<int>& vs,
                             std::vector<std::vector<int>>& tree) {

        assert(!vs.empty());

        // Euler順ソート
        std::sort(vs.begin(), vs.end(),
                  [&](int a, int b) { return fs[a] < fs[b]; });

        int sz = vs.size();

        // LCA追加
        for (int i = 0; i < sz - 1; ++i) {
            vs.push_back(lca(vs[i], vs[i + 1]));
        }

        std::sort(vs.begin(), vs.end(),
                  [&](int a, int b) { return fs[a] < fs[b]; });

        int prv = -1;
        int cur = 0;
        std::vector<int> stk(vs.size());

        for (int v : vs) {
            if (v == prv) continue;

            while (cur > 0 && ls[stk[cur - 1]] < fs[v]) --cur;

            if (cur > 0) {
                tree[stk[cur - 1]].push_back(v);
            }

            tree[v].clear();

            stk[cur++] = v;
            prv = v;
        }

        return stk[0];
    }

    int lca(int u, int v) const {
        int l = fs[u], r = fs[v];
        if (l > r) std::swap(l, r);
        int k = log[r - l + 1];
        int a = st[k][l];
        int b = st[k][r - (1 << k) + 1];
        return (depth[a] <= depth[b] ? a : b);
    }

    int dist(int u, int v) const {
        int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    }

private:
    int n;
    std::vector<std::vector<int>> g;

    std::vector<int> fs, ls, depth;
    std::vector<int> euler, log;
    std::vector<std::vector<int>> st;

    void build(int root) {
        fs.assign(n, 0);
        ls.assign(n, 0);
        depth.assign(n, 0);
        euler.clear();
        euler.reserve(n * 2);

        dfs(root, -1, 0);

        int m = euler.size();
        log.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) log[i] = log[i >> 1] + 1;

        st.assign(log[m] + 1, std::vector<int>(m));
        st[0] = euler;

        for (int k = 0; k < log[m]; ++k) {
            for (int i = 0; i + (1 << (k + 1)) <= m; ++i) {
                int a = st[k][i];
                int b = st[k][i + (1 << k)];
                st[k + 1][i] = (depth[a] <= depth[b] ? a : b);
            }
        }
    }

    void dfs(int v, int p, int d) {
        fs[v] = euler.size();
        euler.push_back(v);
        depth[v] = d;

        for (int nv : g[v]) {
            if (nv == p) continue;
            dfs(nv, v, d + 1);
            euler.push_back(v);
        }

        ls[v] = euler.size() - 1;
    }
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_AUXILIARY_TREE_HPP