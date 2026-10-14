/**
 * @file gaussian_integer.hpp
 * @brief ガウス整数
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/gaussian_integer.hpp
 */
#ifndef KWM_T_MATH_GAUSSIAN_INTEGER_HPP
#define KWM_T_MATH_GAUSSIAN_INTEGER_HPP

#include <tuple>
#include <utility>
#include <cassert>
#include <cmath>

namespace kwm_t::math {

/**
 * @brief ガウス整数 Z[i]
 * verified
 *  https://atcoder.jp/contests/ttpc2023/submissions/74801198
 */

template <class T>
struct GaussianInteger {
    T x, y;

    using Self = GaussianInteger<T>;

    // --- ctor ---
    constexpr GaussianInteger(T x = 0, T y = 0) : x(x), y(y) {}
    constexpr GaussianInteger(std::pair<T,T> p) : x(p.first), y(p.second) {}

    // --- basic ---
    constexpr T norm() const {
        return x * x + y * y;
    }

    constexpr Self conjugate() const {
        return {x, -y};
    }

    // --- arithmetic ---
    constexpr Self& operator+=(const Self& g) {
        x += g.x; y += g.y;
        return *this;
    }

    constexpr Self& operator-=(const Self& g) {
        x -= g.x; y -= g.y;
        return *this;
    }

    constexpr Self& operator*=(const Self& g) {
        T nx = x * g.x - y * g.y;
        T ny = x * g.y + y * g.x;
        x = nx; y = ny;
        return *this;
    }

private:
    // round division helper（Gaussian integer用）
    static T round_div(T a, T b) {
        assert(b != 0);
        if (b < 0) a = -a, b = -b;

        if (a >= 0) return (a + b / 2) / b;
        return -((-a + b / 2) / b);
    }

public:
    constexpr Self& operator/=(const Self& g) {
        T n = g.norm();
        assert(n != 0);

        Self num = (*this) * g.conjugate();

        x = round_div(num.x, n);
        y = round_div(num.y, n);

        return *this;
    }

    constexpr Self& operator%=(const Self& g) {
        Self q = (*this) / g;
        *this -= q * g;
        return *this;
    }

    // --- binary ---
    friend constexpr Self operator+(Self a, const Self& b) { return a += b; }
    friend constexpr Self operator-(Self a, const Self& b) { return a -= b; }
    friend constexpr Self operator*(Self a, const Self& b) { return a *= b; }
    friend constexpr Self operator/(Self a, const Self& b) { return a /= b; }
    friend constexpr Self operator%(Self a, const Self& b) { return a %= b; }

    constexpr Self operator-() const {
        return {-x, -y};
    }

    // --- compare ---
    friend constexpr bool operator==(const Self& a, const Self& b) {
        return a.x == b.x && a.y == b.y;
    }

    friend constexpr bool operator!=(const Self& a, const Self& b) {
        return !(a == b);
    }

    // --- gcd ---
    static Self gcd(Self a, Self b) {
        while (b.x != 0 || b.y != 0) {
            Self r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    // --- extgcd ---
    // a*x + b*y = gcd(a,b)
    static std::tuple<Self, Self, Self> extgcd(Self a, Self b) {
        if (b.x != 0 || b.y != 0) {
            Self q = a / b;
            auto [g, x, y] = extgcd(b, a - q * b);
            return {g, y, x - q * y};
        }
        return {a, Self{1,0}, Self{0,0}};
    }
};

} // namespace kwm_t::math

#endif