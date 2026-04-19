#ifndef KWM_T_MATH_HOOK_LENGTH_FORMULA_HPP
#define KWM_T_MATH_HOOK_LENGTH_FORMULA_HPP

#include <vector>

namespace kwm_t::math {

/**
 * @brief 01列からYoung図形（単調増加列）へ変換
 *
 * 0: x++, 1: y++ とみなしたときの格子経路に対応する図形
 *
 * 例:
 *   10100 → [1,2,2]
 *
 * 典型用途:
 *   - Dyck path / Young図形対応
 */
inline std::vector<int> binary_to_young_shape(const std::vector<int>& v) {
    int height = 0;
    std::vector<int> res;

    for (int x : v) {
        if (x == 0) {
            res.push_back(height);
        } else {
            ++height;
        }
    }
    return res;
}

/**
 * @brief Hook Length Formula により標準Young図形の数を求める
 *
 * 入力は「単調増加列」v:
 *   v[i] = i 行目の長さ（Young diagram）
 *
 * 戻り値:
 *   標準Young tableau の個数
 *
 * 計算量:
 *   O(n^2)
 *
 * 制約 / 注意:
 *   - v は非減少列
 *   - mint は modint 等（inv() が使える）
 *
 * verified:
 */
template <class Mint>
Mint hook_length_formula(const std::vector<int>& v) {
    if (v.empty()) return Mint(1);

    int total = 0;
    for (int x : v) total += x;

    // 分子: (総マス数)!
    Mint numerator = 1;
    for (int i = 1; i <= total; ++i) {
        numerator *= i;
    }

    // 分母: hook の積
    Mint denominator = 1;

    std::vector<int> col_count(v.back() + 1, 0);

    for (int i = 0; i < (int)v.size(); ++i) {
        for (int j = 1; j <= v[i]; ++j) {
            int hook = col_count[j] + (v[i] - j) + 1;
            denominator *= hook;
            col_count[j]++;
        }
    }

    return numerator * denominator.inv();
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_HOOK_LENGTH_FORMULA_HPP