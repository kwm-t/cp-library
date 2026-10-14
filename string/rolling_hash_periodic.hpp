/**
 * @file rolling_hash_periodic.hpp
 * @brief 周期性を扱うローリングハッシュ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/rolling_hash_periodic.hpp
 */
#ifndef KWM_T_STRING_ROLLING_HASH_PERIODIC_HPP
#define KWM_T_STRING_ROLLING_HASH_PERIODIC_HPP

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief Rolling Hash（周期文字列 / 無限列対応）
 *
 * 文字列 S を無限に繰り返した列：
 *   SSSSSSSS...
 *
 * に対して、区間 [l, r) のハッシュを取得する
 *
 * 計算量:
 *   構築: O(N)
 *   クエリ: O(1)
 *
 * @tparam mint modint型（atcoder::modintなど）
 *
 * 使用例:
 *   using mint = atcoder::modint1000000007;
 *   RollingHashPeriodic<mint> rh("abc");
 *   auto h = rh.get(2, 10); // "cabcabca"
 *
 * 制約 / 注意:
 *   - mint は加減乗除が定義されている必要あり
 *   - l, r は long long
 *
 * verified:
 *   - 周期文字列問題
 */
namespace kwm_t::string {

template <typename mint>
class RollingHashPeriodic {
private:
    int n;
    mint base;

    std::vector<mint> hash; // prefix
    std::vector<mint> pow;

public:
    RollingHashPeriodic() = default;

    explicit RollingHashPeriodic(const std::string& s, mint base_ = 27)
        : base(base_) {
        std::vector<int> v(s.size());
        for (int i = 0; i < (int)s.size(); ++i) {
            v[i] = s[i];
        }
        build(v);
    }

    template <typename T>
    RollingHashPeriodic(const std::vector<T>& v, mint base_)
        : base(base_) {
        build(v);
    }

    template <typename T>
    void build(const std::vector<T>& v) {
        n = (int)v.size();

        hash.assign(n + 1, mint(0));
        pow.assign(n + 1, mint(1));

        for (int i = 0; i < n; ++i) {
            hash[i + 1] = hash[i] * base + mint(v[i]);
            pow[i + 1] = pow[i] * base;
        }
    }

    /**
     * @brief 区間 [l, r) のハッシュ（無限列）
     */
    mint get(long long l, long long r) const {
        long long len = r - l;

        long long start = l % n;
        long long h = std::min(len, (long long)n - start);
        long long b = ((len - h) / n) * n;
        long long t = len - h - b;

        mint res = mint(0);

        // head
        if (h) {
            res += get_sub(start, start + h);
        }

        // full blocks
        if (b) {
            mint block = hash[n];
            mint geom = (pow[b] - mint(1)) / (pow[n] - mint(1));
            res += block * geom * pow[h];
        }

        // tail
        if (t) {
            res += get_sub(0, t) * pow[h + b];
        }

        return res;
    }

    int size() const {
        return n;
    }

private:
    /**
     * @brief 区間 [l, r) の通常ハッシュ
     */
    mint get_sub(int l, int r) const {
        return hash[r] - hash[l] * pow[r - l];
    }
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_ROLLING_HASH_PERIODIC_HPP