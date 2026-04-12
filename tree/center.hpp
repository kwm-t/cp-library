#ifndef KWM_T_TREE_CENTER_HPP
#define KWM_T_TREE_CENTER_HPP

#include <vector>
#include "diameter.hpp"

/**
 * @brief 木の中心取得
 *
 * 典型用途:
 *   - 木の中心（1 or 2点）取得
 *   - 木DPの根選び
 *
 * 計算量:
 *   - O(N)
 *
 * 使用例:
 *   auto centers = tree_centers(g);
 *
 *   auto d = tree_diameter(g);
 *   auto centers2 = tree_centers(d);
 * 
 * verified
 *   https://atcoder.jp/contests/past20-open/submissions/74911947
 */
namespace kwm_t::tree {

	/**
 * @brief 木の中心（Diameterから）
 */
inline std::vector<int> tree_centers(const Diameter& d) {
    const auto& path = d.path;

    int m = path.size();
    if (m % 2 == 1) {
        return { path[m / 2] };
    } else {
        return { path[m / 2 - 1], path[m / 2] };
    }
}

/**
 * @brief 木の中心（graphから）
 */
inline std::vector<int> tree_centers(const std::vector<std::vector<int>>& g) {
    return tree_centers(tree_diameter(g));
}
} // namespace kwm_t::tree

#endif // KWM_T_TREE_CENTER_HPP