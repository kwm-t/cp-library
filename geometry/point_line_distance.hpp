#ifndef KWM_T_GEOMETRY_POINT_LINE_DISTANCE_HPP
#define KWM_T_GEOMETRY_POINT_LINE_DISTANCE_HPP

#include <cmath>

namespace kwm_t::geometry {

/**
 * @brief 点と直線の距離を求める。
 *
 * 直線を ax + by + c = 0 で表す。
 *
 * 計算量: O(1)
 *
 * @param x 点の x 座標
 * @param y 点の y 座標
 * @param a 直線の x 係数
 * @param b 直線の y 係数
 * @param c 直線の定数項
 * @return 点と直線の距離
 *
 * 制約 / 注意:
 * - a = b = 0 の場合は直線を表さないため使用不可。
 *
 * 使用例:
 *   double d = kwm_t::geometry::point_line_distance(
 *       1.0, 2.0, 3.0, 4.0, -10.0
 *   );
 *
 * verified:
 *   https://atcoder.jp/contests/awc0154/submissions/79121561
 */
double point_line_distance(
    double x, double y,
    double a, double b, double c
) {
    return std::abs(a * x + b * y + c)
        / std::sqrt(a * a + b * b);
}

} // namespace kwm_t::geometry

#endif // KWM_T_GEOMETRY_POINT_LINE_DISTANCE_HPP