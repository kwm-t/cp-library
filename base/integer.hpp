/**
 * @file integer.hpp
 * @brief 整数型に関する補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/base/integer.hpp
 */
#ifndef KWM_T_BASE_INTEGER_HPP
#define KWM_T_BASE_INTEGER_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace kwm_t {
namespace base {

/**
 * @brief 128bit符号付き整数
 * @note MSVC環境用。2の補数表現を使用する。
 */
class Int128 {
private:
    using ull = std::uint64_t;

    ull hi = 0;
    ull lo = 0;

    static bool negative(const Int128& x) {
        return (x.hi >> 63) != 0;
    }

    static int compare_unsigned(const Int128& a, const Int128& b) {
        if (a.hi != b.hi) return a.hi < b.hi ? -1 : 1;
        if (a.lo != b.lo) return a.lo < b.lo ? -1 : 1;
        return 0;
    }

    static Int128 negate(const Int128& x) {
        Int128 res;
        res.hi = ~x.hi;
        res.lo = ~x.lo + 1;
        if (res.lo == 0) ++res.hi;
        return res;
    }

    static Int128 abs_unsigned(const Int128& x) {
        return negative(x) ? negate(x) : x;
    }

    static std::pair<ull, ull> multiply_64(ull a, ull b) {
        const ull a0 = static_cast<std::uint32_t>(a);
        const ull a1 = a >> 32;
        const ull b0 = static_cast<std::uint32_t>(b);
        const ull b1 = b >> 32;

        ull t = a0 * b0;
        const ull w0 = static_cast<std::uint32_t>(t);
        ull k = t >> 32;

        t = a1 * b0 + k;
        const ull w1 = static_cast<std::uint32_t>(t);
        const ull w2 = t >> 32;

        t = a0 * b1 + w1;

        const ull low = (t << 32) | w0;
        const ull high = a1 * b1 + w2 + (t >> 32);

        return {high, low};
    }

    static void divmod_unsigned(
        const Int128& a,
        const Int128& b,
        Int128& q,
        Int128& r
    ) {
        q = 0;
        r = 0;

        for (int i = 127; i >= 0; --i) {
            const bool carry = (r.hi >> 63) != 0;

            r.hi = (r.hi << 1) | (r.lo >> 63);
            r.lo <<= 1;

            const bool bit = i >= 64
                ? ((a.hi >> (i - 64)) & 1)
                : ((a.lo >> i) & 1);

            r.lo |= static_cast<ull>(bit);

            if (carry || compare_unsigned(r, b) >= 0) {
                r -= b;

                if (i >= 64) {
                    q.hi |= ull(1) << (i - 64);
                } else {
                    q.lo |= ull(1) << i;
                }
            }
        }
    }

public:
    Int128() = default;

    template <
        typename T,
        typename = std::enable_if_t<
            std::is_integral_v<T> && (sizeof(T) <= 8)
        >
    >
    Int128(T x) {
        lo = static_cast<ull>(x);

        if constexpr (std::is_signed_v<T>) {
            hi = x < 0 ? ~ull(0) : 0;
        }
    }

    // 加減算

    Int128& operator+=(const Int128& x) {
        const ull old = lo;
        lo += x.lo;
        hi += x.hi + (lo < old);
        return *this;
    }

    Int128& operator-=(const Int128& x) {
        const ull old = lo;
        lo -= x.lo;
        hi -= x.hi + (old < x.lo);
        return *this;
    }

    friend Int128 operator+(Int128 a, const Int128& b) {
        return a += b;
    }

    friend Int128 operator-(Int128 a, const Int128& b) {
        return a -= b;
    }

    Int128 operator+() const {
        return *this;
    }

    Int128 operator-() const {
        return negate(*this);
    }

    // 乗算

    Int128& operator*=(const Int128& x) {
        auto [h, l] = multiply_64(lo, x.lo);
        hi = h + hi * x.lo + lo * x.hi;
        lo = l;
        return *this;
    }

    friend Int128 operator*(Int128 a, const Int128& b) {
        return a *= b;
    }

    // 除算・剰余

    Int128& operator/=(const Int128& x) {
        const bool neg = negative(*this) != negative(x);

        Int128 q, r;
        divmod_unsigned(abs_unsigned(*this), abs_unsigned(x), q, r);

        *this = neg ? -q : q;
        return *this;
    }

    Int128& operator%=(const Int128& x) {
        const bool neg = negative(*this);

        Int128 q, r;
        divmod_unsigned(abs_unsigned(*this), abs_unsigned(x), q, r);

        *this = neg ? -r : r;
        return *this;
    }

    friend Int128 operator/(Int128 a, const Int128& b) {
        return a /= b;
    }

    friend Int128 operator%(Int128 a, const Int128& b) {
        return a %= b;
    }

    // 比較

    friend bool operator==(const Int128& a, const Int128& b) {
        return a.hi == b.hi && a.lo == b.lo;
    }

    friend bool operator!=(const Int128& a, const Int128& b) {
        return !(a == b);
    }

    friend bool operator<(const Int128& a, const Int128& b) {
        if (negative(a) != negative(b)) return negative(a);
        return compare_unsigned(a, b) < 0;
    }

    friend bool operator>(const Int128& a, const Int128& b) {
        return b < a;
    }

    friend bool operator<=(const Int128& a, const Int128& b) {
        return !(b < a);
    }

    friend bool operator>=(const Int128& a, const Int128& b) {
        return !(a < b);
    }

    // 複合代入

    Int128& operator++() {
        return *this += 1;
    }

    Int128 operator++(int) {
        Int128 res = *this;
        ++*this;
        return res;
    }

    Int128& operator--() {
        return *this -= 1;
    }

    Int128 operator--(int) {
        Int128 res = *this;
        --*this;
        return res;
    }

    // 変換

    explicit operator bool() const {
        return hi != 0 || lo != 0;
    }

    explicit operator long long() const {
        return static_cast<long long>(lo);
    }

    explicit operator unsigned long long() const {
        return lo;
    }

    // 文字列変換

    std::string to_string() const {
        if (*this == 0) return "0";

        const bool neg = negative(*this);
        Int128 x = abs_unsigned(*this);

        std::string s;

        while (x != 0) {
            Int128 q, r;
            divmod_unsigned(x, Int128(10), q, r);
            s.push_back(static_cast<char>('0' + r.lo));
            x = q;
        }

        if (neg) s.push_back('-');

        std::reverse(s.begin(), s.end());
        return s;
    }

    friend std::ostream& operator<<(std::ostream& os, const Int128& x) {
        return os << x.to_string();
    }

    friend std::istream& operator>>(std::istream& is, Int128& x) {
        std::string s;
        is >> s;

        if (!is) return is;

        bool neg = false;
        std::size_t i = 0;

        if (s[i] == '+' || s[i] == '-') {
            neg = s[i] == '-';
            ++i;
        }

        Int128 res = 0;

        for (; i < s.size(); ++i) {
            if (s[i] < '0' || s[i] > '9') {
                is.setstate(std::ios::failbit);
                return is;
            }
            res = res * 10 + (s[i] - '0');
        }

        x = neg ? -res : res;
        return is;
    }
};

/**
 * @brief 最大公約数
 */
inline Int128 gcd(Int128 a, Int128 b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0) {
        Int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/**
 * @brief 最小公倍数
 */
inline Int128 lcm(Int128 a, Int128 b) {
    if (a == 0 || b == 0) return 0;
    return (a / gcd(a, b)) * b;
}

} // namespace base
} // namespace kwm_t

/**
 * @brief ローカル環境用(MSC)int128
 */
#ifdef _MSC_VER
// using int128 = long long;
using int128 = kwm_t::base::Int128;
#else
using int128 = __int128;
std::ostream& operator<<(std::ostream& dest, int128 value) {
	if (value == 0) {
		return dest << 0;
	}
	if (value < 0) {
		dest << '-';
		value = -value;
	}
	char buffer[128];
	char* p = end(buffer);
	while (value > 0) {
		*--p = '0' + (value % 10);
		value /= 10;
	}
	return dest.write(p, end(buffer) - p);
}
#endif

#endif // KWM_T_BASE_INTEGR_HPP