/**
 * @file division.hpp
 * @brief 整数除算の補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/division.hpp
 */
#ifndef KWM_T_UTILITY_DIVISION_HPP
#define KWM_T_UTILITY_DIVISION_HPP

#include <type_traits>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 切り上げ除算 ⌈a / b⌉
 */
template <class T>
constexpr T ceil_div(T a, T b) {
    static_assert(std::is_integral_v<T>, "T must be integral");
    assert(b != 0);

    T q = a / b;
    T r = a % b;

    if (r != 0 && ((a > 0) == (b > 0))) {
        ++q;
    }
    return q;
}

/**
 * @brief 切り捨て除算 ⌊a / b⌋
 */
template <class T>
constexpr T floor_div(T a, T b) {
    assert(b != 0);

    T q = a / b;
    T r = a % b;

    if (r != 0 && ((a > 0) != (b > 0))) {
        --q;
    }
    return q;
}

/**
 * @brief 数学的 mod（常に非負）
 *
 * @details
 *   C++ の % は負になることがあるため補正する。
 *   常に 0 <= r < |m| を満たす。
 *
 * 使用例:
 *   floor_mod( 5, 3) -> 2
 *   floor_mod(-5, 3) -> 1
 *   floor_mod( 5,-3) -> 2
 *   floor_mod(-5,-3) -> 1
 */
template <class T>
constexpr T floor_mod(T a, T m) {
    assert(m != 0);

    T r = a % m;
    if (r < 0) r += (m > 0 ? m : -m);
    return r;
}

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_DIVISION_HPP