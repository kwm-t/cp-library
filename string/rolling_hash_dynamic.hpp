#ifndef KWM_T_STRING_ROLLING_HASH_DYNAMIC_HPP
#define KWM_T_STRING_ROLLING_HASH_DYNAMIC_HPP

#include <vector>
#include <string>
#include <atcoder/segtree>
#include <atcoder/modint>

/**
 * @brief Rolling Hash（動的・1点更新対応）
 *
 * セグメント木を用いてRolling Hashを管理することで、
 * 1点更新と区間ハッシュ取得を両立する
 *
 * 計算量:
 *   構築: O(N)
 *   更新: O(log N)
 *   クエリ: O(log N)
 *
 * 使用例:
 *   RollingHashDynamic rh(s);
 *   rh.update(i, 'c');
 *   auto h = rh.get(l, r);
 *
 * 制約 / 注意:
 *   - modintを使用（ACL依存）
 *   - static版より遅い
 *
 * verified:
 *   - 動的文字列一致判定
 */
namespace kwm_t::string {

using mint = atcoder::modint1000000007;

// -------------------------
// segtree定義
// -------------------------
namespace internal {

inline mint op(mint a, mint b) { return a + b; }
inline mint e() { return 0; }

} // namespace internal

class RollingHashDynamic {
private:
    int n;
    mint base;

    std::vector<mint> pow;
    atcoder::segtree<mint, internal::op, internal::e> seg;

public:
    RollingHashDynamic() = default;

    explicit RollingHashDynamic(const std::string& s, int base_ = 27)
        : base(base_) {
        std::vector<int> v(s.size());
        for (int i = 0; i < (int)s.size(); ++i) {
            v[i] = s[i];
        }
        build(v);
    }

    template <typename T>
    RollingHashDynamic(const std::vector<T>& v, int base_)
        : base(base_) {
        build(v);
    }

    template <typename T>
    void build(const std::vector<T>& v) {
        n = (int)v.size();

        pow.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            pow[i + 1] = pow[i] * base;
        }

        std::vector<mint> init(n);
        for (int i = 0; i < n; ++i) {
            init[i] = pow[i] * v[i];
        }

        seg = atcoder::segtree<mint, internal::op, internal::e>(init);
    }

    /**
     * @brief 1点更新
     */
    template <typename T>
    void update(int i, T x) {
        seg.set(i, pow[i] * x);
    }

    void update(int i, char c) {
        update(i, (int)c);
    }

    /**
     * @brief 区間 [l, r) のハッシュ
     */
    mint get(int l, int r) const {
        return seg.prod(l, r) / pow[l];
    }

    int size() const {
        return n;
    }
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_ROLLING_HASH_DYNAMIC_HPP