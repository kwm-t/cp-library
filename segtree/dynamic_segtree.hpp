/**
 * @file dynamic_segtree.hpp
 * @brief 動的 Segment Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/segtree/dynamic_segtree.hpp
 */
#ifndef KWM_T_SEGTREE_DYNAMIC_SEGTREE_HPP
#define KWM_T_SEGTREE_DYNAMIC_SEGTREE_HPP

#include <memory>
#include <cassert>

/**
 * @brief 動的セグメント木 (Dynamic Segment Tree)
 *
 * @details
 * 必要なノードのみを動的に確保するセグメント木。
 * 非ゼロ要素や更新された点のみ保持するため、
 * 座標圧縮なしで巨大な範囲 [0, n) を扱える。
 *
 * 内部は「implicit treap風」の構造で、
 * index をキーとして二分探索木のように分岐する。
 *
 * ---
 * ■ 典型用途:
 * - 座標が最大 1e9 などの sparse な配列
 * - 遅延なし区間演算 (モノイド)
 * - map + segtree の代替
 *
 * ---
 * ■ 計算量:
 * - set / get / prod : O(log N) (期待値)
 * - 空間 : O(更新回数)
 *
 * ---
 * @tparam S モノイド型
 * @tparam op 二項演算 (結合律必要)
 * @tparam e 単位元
 *
 * ---
 * ■ 制約 / 注意:
 * - op は結合的であること
 * - e() は単位元を返すこと
 * - 再帰実装のためスタックに注意
 *
 * ---
 * ■ 使用例:
 * dynamic_segtree<long long, op, e> seg(1e9);
 * seg.set(5, 10);
 * auto x = seg.prod(0, 10);
 *
 * ---
 * verified:
 * - https://atcoder.jp/contests/abc403/submissions/75015263
 * - https://yukicoder.me/submissions/1157729
 */
namespace kwm_t::segtree {

template <class S, S(*op)(S, S), S(*e)()>
class DynamicSegTree {
private:
    struct Node;
    using NodePtr = std::unique_ptr<Node>;

    struct Node {
        size_t index;
        S value, product;
        NodePtr left, right;

        Node(size_t idx, S val)
            : index(idx), value(val), product(val),
              left(nullptr), right(nullptr) {}

        void update() {
            product = op(
                op(left ? left->product : e(), value),
                right ? right->product : e()
            );
        }
    };

    size_t n;
    NodePtr root;

public:
    explicit DynamicSegTree(size_t n) : n(n), root(nullptr) {}

    // --- 点更新 ---
    void set(size_t p, S x) {
        assert(p < n);
        set(root, 0, n, p, x);
    }

    // --- 点取得 ---
    S get(size_t p) const {
        assert(p < n);
        return get(root, 0, n, p);
    }

    // --- 区間積 [l, r) ---
    S prod(size_t l, size_t r) const {
        assert(l <= r && r <= n);
        return prod(root, 0, n, l, r);
    }

    // --- 全体 ---
    S all_prod() const {
        return root ? root->product : e();
    }

    // --- 区間削除 ---
    void reset(size_t l, size_t r) {
        assert(l <= r && r <= n);
        reset(root, 0, n, l, r);
    }

    // --- max_right ---
    template <class F>
    size_t max_right(size_t l, const F& f) const {
        assert(l <= n);
        S prod = e();
        assert(f(prod));
        return max_right(root, 0, n, l, f, prod);
    }

    template <bool(*f)(S)>
    size_t max_right(size_t l) const {
        return max_right(l, [](S x) { return f(x); });
    }

    // --- min_left ---
    template <class F>
    size_t min_left(size_t r, const F& f) const {
        assert(r <= n);
        S prod = e();
        assert(f(prod));
        return min_left(root, 0, n, r, f, prod);
    }

    template <bool(*f)(S)>
    size_t min_left(size_t r) const {
        return min_left(r, [](S x) { return f(x); });
    }

private:
    // --- 内部実装 ---

    void set(NodePtr& t, size_t a, size_t b, size_t p, S x) const {
        if (!t) {
            t = std::make_unique<Node>(p, x);
            return;
        }

        if (t->index == p) {
            t->value = x;
            t->update();
            return;
        }

        size_t c = (a + b) >> 1;

        if (p < c) {
            if (t->index < p) {
                std::swap(t->index, p);
                std::swap(t->value, x);
            }
            set(t->left, a, c, p, x);
        } else {
            if (p < t->index) {
                std::swap(p, t->index);
                std::swap(x, t->value);
            }
            set(t->right, c, b, p, x);
        }

        t->update();
    }

    S get(const NodePtr& t, size_t a, size_t b, size_t p) const {
        if (!t) return e();
        if (t->index == p) return t->value;

        size_t c = (a + b) >> 1;
        if (p < c) return get(t->left, a, c, p);
        else return get(t->right, c, b, p);
    }

    S prod(const NodePtr& t, size_t a, size_t b, size_t l, size_t r) const {
        if (!t || b <= l || r <= a) return e();
        if (l <= a && b <= r) return t->product;

        size_t c = (a + b) >> 1;
        S res = prod(t->left, a, c, l, r);

        if (l <= t->index && t->index < r)
            res = op(res, t->value);

        return op(res, prod(t->right, c, b, l, r));
    }

    void reset(NodePtr& t, size_t a, size_t b, size_t l, size_t r) const {
        if (!t || b <= l || r <= a) return;

        if (l <= a && b <= r) {
            t.reset();
            return;
        }

        size_t c = (a + b) >> 1;
        reset(t->left, a, c, l, r);
        reset(t->right, c, b, l, r);

        t->update();
    }

    template <class F>
    size_t max_right(const NodePtr& t, size_t a, size_t b,
                     size_t l, const F& f, S& prod) const {
        if (!t || b <= l) return n;

        if (f(op(prod, t->product))) {
            prod = op(prod, t->product);
            return n;
        }

        size_t c = (a + b) >> 1;

        size_t res = max_right(t->left, a, c, l, f, prod);
        if (res != n) return res;

        if (l <= t->index) {
            prod = op(prod, t->value);
            if (!f(prod)) return t->index;
        }

        return max_right(t->right, c, b, l, f, prod);
    }

    template <class F>
    size_t min_left(const NodePtr& t, size_t a, size_t b,
                    size_t r, const F& f, S& prod) const {
        if (!t || r <= a) return 0;

        if (f(op(t->product, prod))) {
            prod = op(t->product, prod);
            return 0;
        }

        size_t c = (a + b) >> 1;

        size_t res = min_left(t->right, c, b, r, f, prod);
        if (res != 0) return res;

        if (t->index < r) {
            prod = op(t->value, prod);
            if (!f(prod)) return t->index + 1;
        }

        return min_left(t->left, a, c, r, f, prod);
    }
};

} // namespace kwm_t::segtree

#endif // KWM_T_SEGTREE_DYNAMIC_SEGTREE_HPP