/**
 * @file optimal_tree_linear_order.hpp
 * @brief 木の最適な線形順序
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/optimal_tree_linear_order.hpp
 */
#ifndef KWM_T_ALGORITHM_TREE_OPTIMAL_TREE_LINEAR_ORDER_HPP
#define KWM_T_ALGORITHM_TREE_OPTIMAL_TREE_LINEAR_ORDER_HPP

#include <vector>
#include <set>
#include <utility>
#include <atcoder/dsu>

/**
 * @brief 木の最適線形順序構築（01 on Tree）
 *
 * @details
 *  各頂点を初期状態で1要素の連結成分とし、
 *  比較関数に従って順に親とマージしていくことで、
 *  最終的に1本の線形順序を構築する。
 *
 *  各成分は「区間 [head, tail]」として管理され、
 *  マージ時に連結される。
 *
 *  用途:
 *    - 木DPの順序最適化
 *    - greedyに順序を決める系
 *
 *  引数:
 *    parent[i]: i の親（rootは 0 想定）
 *    merge(u, v, new_leader):
 *        成分 u, v をマージしたときの更新処理
 *    compare(a, b):
 *        set 内での優先順位（小さいほど先に処理）
 *
 *  計算量:
 *    O(N log N)
 *
 *  verified:
 *    - https://atcoder.jp/contests/agc023/submissions/75095837
 *    - https://atcoder.jp/contests/abc376/submissions/75095772
 */
namespace kwm_t::algorithm::tree {

template <class Merge, class Compare>
std::vector<int> optimal_tree_linear_order(std::vector<int> parent,
                                           Merge merge,
                                           Compare compare) {
    int n = parent.size();

    std::set<int, Compare> st(compare);
    for (int i = 1; i < n; ++i) {
        st.insert(i);
    }

    // 各連結成分の [head, tail]
    std::vector<std::pair<int, int>> lr(n);
    for (int i = 0; i < n; ++i) lr[i] = {i, i};

    // 次のノード（連結リスト）
    std::vector<int> next(n, -1);

    atcoder::dsu uf(n);

    for (int iter = 0; iter < n - 1; ++iter) {
        int v = *st.begin();
        st.erase(v);

        int u = uf.leader(parent[v]);
        st.erase(u);

        auto [hu, tu] = lr[u];
        auto [hv, tv] = lr[v];

        // u の末尾に v を接続
        next[tu] = hv;

        int nv = uf.merge(u, v);
        merge(u, v, nv);

        if (!uf.same(0, nv)) {
            st.insert(nv);
        }

        lr[nv] = {hu, tv};
        parent[nv] = parent[u];
    }

    // 復元
    std::vector<int> res(n);
    for (int i = 0, v = 0; i < n; ++i, v = next[v]) {
        res[i] = v;
    }

    return res;
}

} // namespace kwm_t::algorithm::tree

#endif // KWM_T_ALGORITHM_TREE_OPTIMAL_TREE_LINEAR_ORDER_HPP