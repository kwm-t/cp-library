
/**
 * @file link_cut_tree.hpp
 * @brief 頂点と辺に値を持つ Link-Cut Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/tree/link_cut_tree.hpp
 */
#ifndef KWM_T_TREE_LINK_CUT_TREE_HPP
#define KWM_T_TREE_LINK_CUT_TREE_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace kwm_t::tree {

/**
 * @brief 頂点と辺に値を持つ Link-Cut Tree
 *
 * 典型用途:
 * - 動的な森に対する連結判定・辺の追加削除
 * - パス上の頂点値・辺値の集約
 * - パス上の頂点値・辺値の更新・取得
 * - 根付き木の LCA・距離・k 番目の頂点の取得
 *
 * 計算量:
 * - コンストラクタ: O(N)
 * - build_forest: O(N)
 * - size: O(1)
 * - その他の操作: 償却 O(log N)
 *
 * @tparam S モノイドの要素型
 * @tparam op モノイドの二項演算
 * @tparam e モノイドの単位元
 * @tparam commutative op が可換演算であるか
 * @tparam directional 順方向・逆方向で異なる値を扱うか
 *
 * 制約 / 注意:
 * - op は結合法則を満たすこと。
 * - e() は op の単位元であること。
 * - commutative=true は op が可換演算である場合のみ指定すること。
 * - directional=true の場合、頂点・辺それぞれに順方向・逆方向の値を保持する。
 * - prod(u,v) は頂点と辺を交互に含む順序で集約する。
 * - 辺のみを集約する場合は、全頂点の値を e() にすること。
 * - 経路操作の両端は連結である必要がある。
 * - build_forest に渡す辺集合は自己ループ・多重辺・閉路のない森とする。
 * - 頂点番号は 0-indexed。
 *
 * 使用例:
 * @code
 * using Tree = kwm_t::tree::LinkCutTree<long long, op, e, true>;
 * Tree tree({0, 0, 0});
 * tree.link(0, 1, 5);
 * tree.link(1, 2, 3);
 * auto ans = tree.prod(0, 2); // 8
 * @endcode
 *
 * verified:
 * - https://atcoder.jp/contests/awc0200/submissions/80803974
 */
template <class S, auto op, auto e, bool commutative = false, bool directional = false>
class LinkCutTree {
    static constexpr bool scalar = commutative && std::is_arithmetic_v<S> && !directional;

    struct Empty {};
    struct EmptyReverse {};

    using Back = std::conditional_t<commutative && !directional, Empty, S>;
    using Reverse = std::conditional_t<directional, S, EmptyReverse>;

    struct Node {
        int ch[2] = {}, parent = 0;
        std::uint32_t count : 31 = 1;
        std::uint32_t rev : 1 = 0;

        S value, prod;
        [[no_unique_address]] Back back;
        [[no_unique_address]] Reverse value_reverse;

        static Back make_back(const S &v) {
            if constexpr (commutative && !directional) return {};
            else return v;
        }

        static Reverse make_reverse(const S &v) {
            if constexpr (directional) return v;
            else return {};
        }

        explicit Node(const S &v)
            : Node(v, v) {}

        Node(const S &v, const S &rv)
            : value(v), prod(v), back(make_back(v)), value_reverse(make_reverse(rv)) {}
    };

    int n = 0;
    std::vector<Node> t;
    std::vector<int> free_edges;

    void init(int n_) {
        assert(0 <= n_ && n_ < std::numeric_limits<int>::max() / 2);

        n = n_;
        t.reserve(std::size_t(2) * n + 1);
        t.emplace_back(e());
        t[0].count = 0;
    }

    int node(int v) const {
        assert(0 <= v && v < n);
        return v + 1;
    }

    int alloc_edge(S w_uv, S w_vu) {
        if (free_edges.empty()) {
            int eid = static_cast<int>(t.size());
            t.emplace_back(w_uv, w_vu);
            return eid;
        }

        int eid = free_edges.back();
        free_edges.pop_back();
        t[eid] = Node(w_uv, w_vu);
        return eid;
    }

    void free_edge(int eid) {
        t[eid].ch[0] = t[eid].ch[1] = 0;
        t[eid].parent = 0;
        t[eid].count = 1;
        t[eid].rev = 0;

        free_edges.push_back(eid);
    }

    bool is_root(int x) const {
        return t[x].parent <= 0;
    }

    void pull(int x) {
        auto &v = t[x];
        int l = v.ch[0], r = v.ch[1];

        v.count = t[l].count + t[r].count + 1;

        if constexpr (scalar) {
            v.prod = op(op(t[l].prod, v.value), t[r].prod);
        } else {
            v.prod = l ? op(t[l].prod, v.value) : v.value;
            if (r) v.prod = op(std::move(v.prod), t[r].prod);
        }

        if constexpr (!commutative || directional) {
            if constexpr (directional) {
                v.back = r ? op(t[r].back, v.value_reverse) : v.value_reverse;
            } else {
                v.back = r ? op(t[r].back, v.value) : v.value;
            }
            if (l) v.back = op(std::move(v.back), t[l].back);
        }
    }

    void flip(int x) {
        if (!x) return;

        std::swap(t[x].ch[0], t[x].ch[1]);

        if constexpr (!commutative || directional) {
            std::swap(t[x].prod, t[x].back);
        }

        if constexpr (directional) {
            std::swap(t[x].value, t[x].value_reverse);
        }

        t[x].rev ^= 1;
    }

    void push(int x) {
        if (!t[x].rev) return;

        flip(t[x].ch[0]);
        flip(t[x].ch[1]);
        t[x].rev = 0;
    }

    void rotate(int x) {
        int p = t[x].parent, g = t[p].parent;
        int d = t[p].ch[1] == x, c = t[x].ch[d ^ 1];

        if (g > 0) t[g].ch[t[g].ch[1] == p] = x;

        t[x].parent = g;
        t[p].ch[d] = c;
        t[c].parent = p;
        t[x].ch[d ^ 1] = p;
        t[p].parent = x;

        pull(p);
		pull(x);
    }

    void splay(int x) {
        if (is_root(x)) {
            push(x);
            return;
        }

        do {
            int p = t[x].parent, g = t[p].parent;
            bool top = is_root(p);

            if (!top) push(g);
            push(p);
            push(x);

            if (!top) {
                rotate((t[p].ch[0] == x) == (t[g].ch[0] == p) ? p : x);
            }

            rotate(x);
        } while (!is_root(x));
    }

    int access(int x) {
        splay(x);

        if (t[x].ch[1]) t[t[x].ch[1]].parent = -x;
        t[x].ch[1] = 0;
		pull(x);

        int last = x;

        while (t[x].parent) {
            int p = -t[x].parent;
            splay(p);

            if (t[p].ch[1]) t[t[p].ch[1]].parent = -p;
            t[p].ch[1] = x;
            t[x].parent = p;

            rotate(x);
            last = p;
        }

        return last;
    }

    void reroot(int x) {
        access(x);
        flip(x);
    }

    int expose(int u, int v) {
        u = node(u);
        v = node(v);

        reroot(u);
        access(v);

        assert(u == v || t[u].parent != 0);
        return v;
    }

    /**
     * @brief パス上の辺ノードを取得する
     * @note expose(u,v) の直後に呼び出す。
     */
    int edge_node(int u, int v) const {
        int l = t[v].ch[0];
        assert(t[l].count == 2);

        return l == u ? t[l].ch[1] : l;
    }

  public:
    explicit LinkCutTree(int n_ = 0) {
        init(n_);
        for (int i = 0; i < n; ++i) t.emplace_back(t[0].value);
    }

    explicit LinkCutTree(const std::vector<S> &values) {
        assert(values.size() < std::size_t(std::numeric_limits<int>::max() / 2));

        init(static_cast<int>(values.size()));
        for (const S &v : values) t.emplace_back(v);
    }

    int size() const {
        return n;
    }

    /**
     * @brief 辺のない森として再構築する
     * @note 既存の頂点値は保持する。
     */
    void build_forest(std::span<const std::pair<int, int>> edges) {
        std::vector<std::tuple<int, int, S, S>> weighted;
        weighted.reserve(edges.size());

        for (auto [u, v] : edges) {
            weighted.emplace_back(u, v, e(), e());
        }

        build_forest(std::span<const std::tuple<int, int, S, S>>(weighted));
    }

    /**
     * @brief 辺に値を持つ森として再構築する
     * @param edges (u, v, w_uv, w_vu) の配列
     * @note 既存の頂点値は保持する。
     */
    void build_forest(std::span<const std::tuple<int, int, S, S>> edges) {
        assert(edges.size() < std::size_t(n) || (edges.empty() && n == 0));

        int m = static_cast<int>(edges.size());
        int total = n + m;

        t.resize(n + 1, Node(t[0].value));
        free_edges.clear();

        std::vector<int> degree(total), neighbors(total);
        std::vector<int> parent(total, -1), order;
        order.reserve(total);

        for (int i = 0; i < m; ++i) {
            const auto &[u, v, w_uv, w_vu] = edges[i];

            assert(0 <= u && u < n);
            assert(0 <= v && v < n && u != v);

            int eid = alloc_edge(w_uv, w_vu) - 1;

            ++degree[u];
            ++degree[eid];
            ++degree[v];
            ++degree[eid];

            neighbors[u] ^= eid;
            neighbors[eid] ^= u;
            neighbors[v] ^= eid;
            neighbors[eid] ^= v;
        }

        for (int v = 0; v < total; ++v) {
            if (degree[v] <= 1) order.push_back(v);
        }

        for (std::size_t i = 0; i < order.size(); ++i) {
            int v = order[i];
            if (!degree[v]) continue;

            int p = neighbors[v];
            parent[v] = p;
            degree[v] = 0;
            neighbors[p] ^= v;

            if (--degree[p] == 1) order.push_back(p);
        }

        assert(order.size() == std::size_t(total));

        for (int i = 0; i < m; ++i) {
            const auto &[u, v, w_uv, w_vu] = edges[i];

            int eid = n + i;
            auto &x = t[eid + 1];

            if (parent[eid] == v) {
                x.value = w_vu;
                if constexpr (directional) x.value_reverse = w_uv;
            }
        }

        for (int v = 0; v < total; ++v) {
            auto &x = t[v + 1];

            x.ch[0] = x.ch[1] = 0;
            x.parent = -parent[v] - 1;
            x.count = 1;
            x.rev = 0;
            x.prod = x.value;

            if constexpr (!commutative || directional) {
                if constexpr (directional) x.back = x.value_reverse;
                else x.back = x.value;
            }
        }
    }

    S get(int v) const requires (!directional) {
        return t[node(v)].value;
    }

    void set(int v, S value) requires (!directional) {
        int x = node(v);
        access(x);

        t[x].value = std::move(value);
        pull(x);
    }

    std::pair<S, S> get(int v) requires (directional) {
        int x = node(v);
        access(x);

        return {t[x].value, t[x].value_reverse};
    }

    void set(int v, S value, S reverse) requires (directional) {
        int x = node(v);
        access(x);

        t[x].value = std::move(value);
        t[x].value_reverse = std::move(reverse);
        pull(x);
    }

    bool link(int u, int v) {
        return link(u, v, e(), e());
    }

    bool link(int u, int v, S w) {
        return link(u, v, w, w);
    }

    bool link(int u, int v, S w_uv, S w_vu) {
        int nu = node(u), nv = node(v);

        if (nu == nv) return false;

        reroot(nv);
        access(nu);

        if (t[nv].parent) return false;

        int eid = alloc_edge(std::move(w_uv), std::move(w_vu));

        t[nv].parent = -eid;
        t[eid].parent = -nu;

        return true;
    }

    bool cut(int u, int v) {
        u = node(u);
        v = node(v);

        if (u == v) return false;

        reroot(u);
        access(v);

        int l = t[v].ch[0];

        if (!t[u].parent || t[l].count != 2) return false;

        int eid = l == u ? t[u].ch[1] : l;

        t[v].ch[0] = 0;

        t[u].ch[0] = t[u].ch[1] = t[u].parent = 0;
        t[u].count = 1;
        t[u].rev = 0;

        free_edge(eid);
        pull(v);

        return true;
    }

    void set_edge(int u, int v, S w) {
        set_edge(u, v, w, w);
    }

    void set_edge(int u, int v, S w_uv, S w_vu) {
        int x = expose(u, v);
        int eid = edge_node(node(u), node(v));

        splay(eid);

        t[eid].value = std::move(w_uv);

        if constexpr (directional) {
            t[eid].value_reverse = std::move(w_vu);
        }

        pull(eid);
        (void)x;
    }

    S get_edge(int u, int v) {
        expose(u, v);

        int eid = edge_node(node(u), node(v));
        splay(eid);

        return t[eid].value;
    }

    void make_root(int v) {
        reroot(node(v));
    }

    int root(int v) {
        int x = node(v);
        access(x);
        push(x);

        while (t[x].ch[0]) {
            x = t[x].ch[0];
            push(x);
        }

        splay(x);
        return x - 1;
    }

    bool same(int u, int v) {
        u = node(u);
        v = node(v);

        if (u == v) return true;

        access(u);
        access(v);

        return t[u].parent != 0;
    }

    S prod(int u, int v) {
        int x = expose(u, v);
        return t[x].prod;
    }

    int distance(int u, int v) {
        int x = expose(u, v);
        return t[t[x].ch[0]].count / 2;
    }

    int path_size(int u, int v) {
        return distance(u, v) + 1;
    }

    int jump(int u, int v, int k) {
        int x = expose(u, v);

        if (k < 0 || 2 * k > int(t[t[x].ch[0]].count)) return -1;

        int target = 2 * k;

        for (;;) {
            push(x);

            int l = t[x].ch[0];
            int count = t[l].count;

            if (target == count) break;

            if (target < count) {
                x = l;
            } else {
                target -= count + 1;
                x = t[x].ch[1];
            }
        }

        splay(x);
        return x - 1;
    }

    int lca(int r, int u, int v) {
        r = node(r);
        u = node(u);
        v = node(v);

        reroot(r);
        access(u);

        assert(r == u || t[r].parent != 0);

        int w = access(v);

        assert(u == v || t[u].parent != 0);
        return w - 1;
    }
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_LINK_CUT_TREE_HPP
