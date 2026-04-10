#ifndef KWM_T_MATH_FLOAT_EPS_HPP
#define KWM_T_MATH_FLOAT_EPS_HPP

#include <cmath>
#include <type_traits>

namespace kwm_t::math {

/**
 * @brief EPS付き浮動小数点ラッパ
 *
 * double / long double を安全に比較するための型
 *
 * 性質:
 *   ・== は EPS 以内で一致とみなす
 *   ・< は厳密順序を壊さないように定義
 *
 * 注意:
 *   ・完全な順序ではない（EPSの影響）
 *   ・sort等では注意
 *
 * @tparam T double or long double
 * @tparam EPS 比較誤差
 *
 * verified:
 */

template <class T = double, T EPS = (T)1e-9>
struct FloatEPS {
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

    T value;

    // --- ctor ---
    constexpr FloatEPS() : value(0) {}
    constexpr FloatEPS(T v) : value(v) {}

    // --- arithmetic ---
    constexpr FloatEPS& operator+=(const FloatEPS& o) { value += o.value; return *this; }
    constexpr FloatEPS& operator-=(const FloatEPS& o) { value -= o.value; return *this; }
    constexpr FloatEPS& operator*=(const FloatEPS& o) { value *= o.value; return *this; }
    constexpr FloatEPS& operator/=(const FloatEPS& o) { value /= o.value; return *this; }

    constexpr FloatEPS operator+(const FloatEPS& o) const { return FloatEPS(*this) += o; }
    constexpr FloatEPS operator-(const FloatEPS& o) const { return FloatEPS(*this) -= o; }
    constexpr FloatEPS operator*(const FloatEPS& o) const { return FloatEPS(*this) *= o; }
    constexpr FloatEPS operator/(const FloatEPS& o) const { return FloatEPS(*this) /= o; }

    // --- comparison ---
    friend bool operator==(const FloatEPS& a, const FloatEPS& b) {
        return std::abs(a.value - b.value) <= EPS;
    }

    friend bool operator!=(const FloatEPS& a, const FloatEPS& b) {
        return !(a == b);
    }

    friend bool operator<(const FloatEPS& a, const FloatEPS& b) {
        return a.value < b.value - EPS;
    }

    friend bool operator>(const FloatEPS& a, const FloatEPS& b) {
        return b < a;
    }

    friend bool operator<=(const FloatEPS& a, const FloatEPS& b) {
        return !(b < a);
    }

    friend bool operator>=(const FloatEPS& a, const FloatEPS& b) {
        return !(a < b);
    }

    // --- utility ---
    explicit operator T() const { return value; }
};

} // namespace kwm_t::math

#endif