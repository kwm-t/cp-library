#ifndef KWM_T_MATH_RATIONAL_HPP
#define KWM_T_MATH_RATIONAL_HPP

#include <numeric>
#include <cassert>
#include <iostream>

namespace kwm_t::math {

/**
 * @brief 有理数（分数）
 *
 * 特徴:
 *   ・常に既約分数（正規化）
 *   ・分母は常に正
 *   ・比較はオーバーフロー軽減版
 *
 * 計算量:
 *   ・演算ごとに gcd
 *
 * 注意:
 *   ・long long 範囲外は未対応
 *
 * verified:
 */

struct Rational {
    long long num, den;

    // --- ctor ---
    Rational(long long n = 0, long long d = 1) : num(n), den(d) {
        normalize();
    }

private:
    void normalize() {
        assert(den != 0);

        if (den < 0) {
            num = -num;
            den = -den;
        }

        long long g = std::gcd(num, den);
        num /= g;
        den /= g;
    }

public:
    // --- arithmetic ---
    Rational& operator+=(const Rational& o) {
        long long g = std::gcd(den, o.den);
        long long lcm = den / g * o.den;

        num = num * (o.den / g) + o.num * (den / g);
        den = lcm;

        normalize();
        return *this;
    }

    Rational& operator-=(const Rational& o) {
        long long g = std::gcd(den, o.den);
        long long lcm = den / g * o.den;

        num = num * (o.den / g) - o.num * (den / g);
        den = lcm;

        normalize();
        return *this;
    }

    Rational& operator*=(const Rational& o) {
        long long g1 = std::gcd(num, o.den);
        long long g2 = std::gcd(o.num, den);

        num = (num / g1) * (o.num / g2);
        den = (den / g2) * (o.den / g1);

        return *this;
    }

    Rational& operator/=(const Rational& o) {
        assert(o.num != 0);

        long long g1 = std::gcd(num, o.num);
        long long g2 = std::gcd(o.den, den);

        num = (num / g1) * (o.den / g2);
        den = (den / g2) * (o.num / g1);

        if (den < 0) {
            num = -num;
            den = -den;
        }

        return *this;
    }

    // --- binary ops ---
    friend Rational operator+(Rational a, const Rational& b) { return a += b; }
    friend Rational operator-(Rational a, const Rational& b) { return a -= b; }
    friend Rational operator*(Rational a, const Rational& b) { return a *= b; }
    friend Rational operator/(Rational a, const Rational& b) { return a /= b; }

    // --- comparison（オーバーフロー軽減）---
    friend bool operator==(const Rational& a, const Rational& b) {
        return a.num == b.num && a.den == b.den;
    }

    friend bool operator!=(const Rational& a, const Rational& b) {
        return !(a == b);
    }

    friend bool operator<(const Rational& a, const Rational& b) {
        // a/b < c/d → a*d < c*b
        // ただし gcdで削る
        long long g1 = std::gcd(a.num, b.den);
        long long g2 = std::gcd(b.num, a.den);

        return (a.num / g1) * (b.den / g2)
             < (b.num / g2) * (a.den / g1);
    }

    friend bool operator>(const Rational& a, const Rational& b) {
        return b < a;
    }

    friend bool operator<=(const Rational& a, const Rational& b) {
        return !(b < a);
    }

    friend bool operator>=(const Rational& a, const Rational& b) {
        return !(a < b);
    }

    // --- utility ---
    double to_double() const {
        return (double)num / den;
    }

    void print() const {
        if (den == 1) std::cout << num;
        else std::cout << num << "/" << den;
    }
};

} // namespace kwm_t::math

#endif