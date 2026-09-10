#ifndef KWM_T_GEOMETRY_COMMON_TANGENTS_HPP
#define KWM_T_GEOMETRY_COMMON_TANGENTS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace kwm_t::geometry {

/**
 * @brief 2つの円の共通接線をすべて求める。
 *
 * 直線 ax + by + c = 0 を返す。
 * 返される直線は a^2 + b^2 = 1 に正規化されている。
 *
 * 円の半径 d1, d2 は 0 でもよく、点を半径0の円として扱える。
 *
 * 計算量: O(1)
 *
 * @param x1 1つ目の円の中心の x 座標
 * @param y1 1つ目の円の中心の y 座標
 * @param d1 1つ目の円の半径
 * @param x2 2つ目の円の中心の x 座標
 * @param y2 2つ目の円の中心の y 座標
 * @param d2 2つ目の円の半径
 * @return 共通接線 ax + by + c = 0 の配列。
 *         各要素は {a, b, c}。
 *
 * 制約 / 注意:
 * - 通常、共通接線は最大4本。
 * - 半径0の円も扱える。
 * - 2つの円が同心かつ同半径の場合は共通接線が無限本あるため、
 *   空配列を返す。
 * - 2つの円が同心かつ半径が異なる場合も空配列を返す。
 * - 浮動小数点誤差を考慮して判定している。
 *
 * 使用例:
 *   auto lines = kwm_t::geometry::common_tangents(
 *       0.0, 0.0, 2.0,
 *       5.0, 0.0, 1.0
 *   );
 *
 *   for (auto [a, b, c] : lines) {
 *       // ax + by + c = 0
 *   }
 *
 * verified:
 *   https://atcoder.jp/contests/awc0154/submissions/79121561
 */
std::vector<std::array<double, 3>> common_tangents(
    double x1, double y1, double d1,
    double x2, double y2, double d2
) {
    constexpr double EPS = 1e-12;

    std::vector<std::array<double, 3>> result;

    const double dx = x2 - x1;
    const double dy = y2 - y1;
    const double dist2 = dx * dx + dy * dy;

    // 同心円。
    if (dist2 < EPS * EPS) {
        // 同じ円なら共通接線は無限本。
        // 異なる半径なら共通接線は存在しない。
        return result;
    }

    const double dist = std::sqrt(dist2);

    // s1, s2 は各中心から直線への符号付き距離の符号。
    for (int s1 : {-1, 1}) {
        for (int s2 : {-1, 1}) {
            const double v = s2 * d2 - s1 * d1;

            // |n| = 1 かつ n・(C2-C1) = v
            // を満たす n=(a,b) が存在する必要がある。
            if (v > dist + EPS || v < -dist - EPS) {
                continue;
            }

            const double t = v / dist;

            // C2-C1 方向の単位ベクトル。
            const double ux = dx / dist;
            const double uy = dy / dist;

            // それと垂直な単位ベクトル。
            const double vx = -uy;
            const double vy = ux;

            // n = t*u +/- sqrt(1-t^2)*v
            const double q = std::sqrt(
                std::max(0.0, 1.0 - t * t)
            );

            for (int sign : {-1, 1}) {
                const double a = t * ux + sign * q * vx;
                const double b = t * uy + sign * q * vy;

                // 中心1から直線までの符号付き距離を
                // s1 * d1 とする。
                const double c =
                    -a * x1 - b * y1 + s1 * d1;

                result.push_back({a, b, c});
            }
        }
    }

    // 接する場合などに同一の直線が重複するため除去する。
    std::vector<std::array<double, 3>> ans;

    for (const std::array<double, 3>& line : result) {
        bool duplicate = false;

        for (const std::array<double, 3>& other : ans) {
            if (std::abs(line[0] - other[0]) < EPS &&
                std::abs(line[1] - other[1]) < EPS &&
                std::abs(line[2] - other[2]) < EPS) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            ans.push_back(line);
        }
    }

    return ans;
}

} // namespace kwm_t::geometry

#endif // KWM_T_GEOMETRY_COMMON_TANGENTS_HPP