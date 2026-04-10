#ifndef KWM_T_STRING_ROLLING_HASH_HPP
#define KWM_T_STRING_ROLLING_HASH_HPP

#include <vector>
#include <utility>

/**
 * @brief Rolling Hash（静的・ダブルハッシュ）
 *
 * 文字列や配列に対して prefix hash を構築し、
 * 任意区間のハッシュを O(1) で取得できる
 *
 * 計算量:
 *   構築: O(N)
 *   クエリ: O(1)
 *
 * @tparam T 要素型（char, int など）
 *
 * @param s 入力列
 *
 * 使用例:
 *   RollingHash<std::string> rh(s);
 *   auto h = rh.get(l, r); // [l, r)
 *
 *   if (rh.get(l1, r1) == rh.get(l2, r2)) {
 *       // substring equal
 *   }
 *
 * 制約 / 注意:
 *   - 衝突の可能性は低いがゼロではない
 *   - T は整数として扱える必要あり
 *
 * verified:
 *   - substring一致判定
 *   - 回文判定
 */
namespace kwm_t::string {

template <typename Container>
struct RollingHash {
private:
    using ll = long long;

    static constexpr ll base1 = 1007;
    static constexpr ll base2 = 2009;
    static constexpr ll mod1  = 1000000007;
    static constexpr ll mod2  = 1000000009;

    std::vector<ll> hash1, hash2;
    std::vector<ll> power1, power2;

public:
    RollingHash() = default;

    explicit RollingHash(const Container& s) {
        build(s);
    }

    void build(const Container& s) {
        int n = (int)s.size();

        hash1.assign(n + 1, 0);
        hash2.assign(n + 1, 0);
        power1.assign(n + 1, 1);
        power2.assign(n + 1, 1);

        for (int i = 0; i < n; ++i) {
            ll x = (ll)s[i];

            hash1[i + 1] = (hash1[i] * base1 + x) % mod1;
            hash2[i + 1] = (hash2[i] * base2 + x) % mod2;

            power1[i + 1] = (power1[i] * base1) % mod1;
            power2[i + 1] = (power2[i] * base2) % mod2;
        }
    }

    /**
     * @brief 区間 [l, r) のハッシュを取得
     */
    std::pair<ll, ll> get(int l, int r) const {
        ll res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
        if (res1 < 0) res1 += mod1;

        ll res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
        if (res2 < 0) res2 += mod2;

        return {res1, res2};
    }

    /**
     * @brief 長さを取得
     */
    int size() const {
        return (int)hash1.size() - 1;
    }
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_ROLLING_HASH_HPP