/**
 * @file gray_code.hpp
 * @brief Gray code
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/gray_code.hpp
 */

#ifndef KWM_T_MATH_GRAY_CODE_HPP
#define KWM_T_MATH_GRAY_CODE_HPP

#include <vector>

/**
 * @brief Gray code（グレイコード列生成）
 *
 * 長さ 2^n の列を生成し、隣接要素が1bitだけ異なる。
 *
 * 典型用途:
 *   - bit DP
 *   - 巡回順列生成
 *   - XOR系問題
 *
 * 計算量:
 *   - O(2^n)
 *
 * 制約 / 注意:
 *   - n >= 0
 *   - n が大きいとメモリ注意
 *
 * 使用例:
 *   auto g = kwm_t::math::gray_code(3);
 *   // {0,1,3,2,6,7,5,4}
 *
 * verified:
 */
namespace kwm_t::math {

inline std::vector<int> gray_code(int n) {
    std::vector<int> gray(1 << n);
    for (int i = 0; i < (1 << n); ++i) {
        gray[i] = i ^ (i >> 1);
    }
    return gray;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_GRAY_CODE_HPP