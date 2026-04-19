#ifndef KWM_T_MATH_PRIMITIVE_ROOT_HPP
#define KWM_T_MATH_PRIMITIVE_ROOT_HPP

#include <vector>
#include <cstdint>

#include <atcoder/math>

namespace kwm_t::math {

/**
 * @brief 原始根を求める（mod が素数）
 *
 * 素数 p に対して、原始根 g を返す。
 * g は位数 p-1 の元（生成元）。
 *
 * 典型用途:
 *   - NTT
 *   - 離散対数
 *
 * 計算量:
 *   - O(√p)
 *
 * 制約 / 注意:
 *   - p は素数
 *
 * verified:
 *   - AtCoder Library 相当実装
 */
inline int primitive_root(int p) {
    if (p == 2) return 1;

    // よく使う素数はハードコード
    if (p == 167772161) return 3;
    if (p == 469762049) return 3;
    if (p == 754974721) return 11;
    if (p == 998244353) return 3;
    if (p == 1000000007) return 5;

    // p-1 を素因数分解
    std::vector<int> divs;
    divs.push_back(2);

    std::int64_t x = (p - 1) / 2;

    while (x % 2 == 0) x /= 2;

    for (std::int64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            divs.push_back((int)i);
            while (x % i == 0) x /= i;
        }
    }
    if (x > 1) divs.push_back((int)x);

    // 原始根探索
    for (int g = 2;; ++g) {
        bool ok = true;
        for (int d : divs) {
            if (atcoder::pow_mod(g, (p - 1) / d, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_PRIMITIVE_ROOT_HPP