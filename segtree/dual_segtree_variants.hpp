#ifndef KWM_T_SEGTREE_DUAL_SEGTREE_VARIANTS_HPP
#define KWM_T_SEGTREE_DUAL_SEGTREE_VARIANTS_HPP

#include <algorithm>
#include <limits>

/**
 * @brief DualSegtree 用の典型パターン集
 *
 * 提供:
 *   - Range Add / Point Get
 *   - Range Update / Point Get
 *   - Range Chmin / Point Get
 *   - Range Chmax / Point Get
 *
 * 計算量:
 *   - apply: O(log N)
 *   - get:   O(log N)
 *
 * 注意:
 *   - op(x, f) は「既存作用 x に新しい作用 f を適用」
 *   - つまり op(old, new)
 *
 * verified:
 *   - 自作DualSegtreeで使用
 */

namespace kwm_t::segtree {

    static constexpr long long INF  = (long long)4e18;
    static constexpr long long NINF = -(long long)4e18;

    // ================= Range Add =================
    namespace RangeAdd {
        // 区間加算 / 一点取得
        using S = long long;

        S op(S x, S f) { return x + f; }
        S e() { return 0; }
    }

    // ================= Range Update =================
    namespace RangeUpdate {
        // 区間更新（後勝ち） / 一点取得
        using S = long long;
        static constexpr long long ID = NINF;

        S op(S x, S f) { return (f == ID ? x : f); }
        S e() { return ID; }
    }

    // ================= Range Chmin =================
    namespace RangeChmin {
        // 区間chmin / 一点取得
        using S = long long;

        S op(S x, S f) { return std::min(x, f); }
        S e() { return INF; }
    }

    // ================= Range Chmax =================
    namespace RangeChmax {
        // 区間chmax / 一点取得
        using S = long long;

        S op(S x, S f) { return std::max(x, f); }
        S e() { return NINF; }
    }

} // namespace kwm_t::segtree

#endif // KWM_T_SEGTREE_DUAL_SEGTREE_VARIANTS_HPP