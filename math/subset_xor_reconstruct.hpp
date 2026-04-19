#ifndef KWM_T_MATH_XOR_BASIS_RECONSTRUCT_HPP
#define KWM_T_MATH_XOR_BASIS_RECONSTRUCT_HPP

#include <vector>
#include <cstdint>

namespace kwm_t::math {

/**
 * @brief 部分集合の XOR が b になるか判定し、構成を復元
 *
 * a の部分集合 S を選んで XOR_{i∈S} a[i] = b が可能か判定する。
 * 可能な場合、used[i] = 1 なら i を選ぶことで b を作れる。
 *
 * 典型用途:
 *   - XOR = K を作る問題（構成付き）
 *   - XOR基底の復元
 *
 * 計算量:
 *   - O(n * B)（B = bit数 ≈ 60）
 *
 * 制約 / 注意:
 *   - mask に uint64_t を使っているため、独立なベクトルは最大64個まで
 *   - a[i] は 64bit 整数想定
 *
 * @param a 入力配列
 * @param b 目標 XOR
 * @param used 結果（サイズ n、選んだ要素に1が入る）
 * @return true なら構成あり
 *
 * verified:
 */
inline bool subset_xor_reconstruct(
    const std::vector<long long>& a,
    long long b,
    std::vector<int>& used
) {
    int n = (int)a.size();

    std::vector<long long> basis;
    std::vector<std::uint64_t> mask;
    std::vector<int> origin;

    // 基底構築
    for (int i = 0; i < n; ++i) {
        long long x = a[i];
        std::uint64_t m = 0;

        for (int j = 0; j < (int)basis.size(); ++j) {
            if ((x ^ basis[j]) < x) {
                x ^= basis[j];
                m ^= mask[j];
            }
        }

        if (x != 0) {
            m |= (1ULL << basis.size());
            basis.push_back(x);
            mask.push_back(m);
            origin.push_back(i);
        }
    }

    // b を基底で消す
    std::uint64_t comb = 0;
    for (int j = 0; j < (int)basis.size(); ++j) {
        if ((b ^ basis[j]) < b) {
            b ^= basis[j];
            comb ^= mask[j];
        }
    }

    if (b != 0) return false;

    // 復元
    used.assign(n, 0);
    for (int j = 0; j < (int)basis.size(); ++j) {
        if (comb & (1ULL << j)) {
            used[origin[j]] = 1;
        }
    }

    return true;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_XOR_BASIS_RECONSTRUCT_HPP