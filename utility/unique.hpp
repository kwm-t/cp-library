/**
 * @file unique.hpp
 * @brief vector のソートと重複要素の削除
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/unique.hpp
 */
#ifndef KWM_T_UTILITY_UNIQUE_HPP
#define KWM_T_UTILITY_UNIQUE_HPP

#include <algorithm>
#include <vector>

namespace kwm_t::utility {

/**
 * @brief vector をソートして重複要素を削除する
 *
 * 計算量: O(N log N)
 *
 * @tparam T 要素の型
 *
 * @param org 元の vector（コピーして受け取る）
 * @return ソート済みかつ重複を削除した vector
 *
 * 制約 / 注意:
 * - T は std::sort による比較が可能であること
 * - 元の vector は変更しない
 *
 * 使用例:
 * @code
 * std::vector<int> a = {3, 1, 2, 1, 3};
 * auto b = kwm_t::utility::unique(a);
 * // b = {1, 2, 3}
 * @endcode
 *
 * verified:
 *  未verified
 */
template <typename T>
std::vector<T> unique(std::vector<T> org) {
    std::sort(org.begin(), org.end());
    org.erase(std::unique(org.begin(), org.end()), org.end());
    return org;
}

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_UNIQUE_HPP