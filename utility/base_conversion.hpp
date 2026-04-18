#ifndef KWM_T_UTILITY_BASE_CONVERSION_HPP
#define KWM_T_UTILITY_BASE_CONVERSION_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <cassert>

namespace kwm_t::utility {

namespace internal {

inline int char_to_val(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'Z') return c - 'A' + 10;
    if ('a' <= c && c <= 'z') return c - 'a' + 10;
    return -1;
}

inline char val_to_char(int x) {
    if (x < 10) return char('0' + x);
    return char('A' + (x - 10));
}

} // namespace internal

/**
 * @brief p進数文字列 → 10進数 (整数)
 *
 * @details
 *   小さい値用（オーバーフロー注意）
 *
 * @tparam T 整数型
 */
template <class T>
constexpr T to_decimal(const std::string& s, int base) {
    static_assert(std::is_integral_v<T>, "T must be integral");
    assert(2 <= base && base <= 36);

    T ret = 0;
    for (char c : s) {
        int v = internal::char_to_val(c);
        assert(0 <= v && v < base);
        ret = ret * base + v;
    }
    return ret;
}

/**
 * @brief 10進数 → p進数文字列
 */
template <class T>
std::string from_decimal(T x, int base) {
    static_assert(std::is_integral_v<T>, "T must be integral");
    assert(2 <= base && base <= 36);

    if (x == 0) return "0";

    bool neg = (x < 0);
    if (neg) x = -x;

    std::string res;
    while (x > 0) {
        int r = x % base;
        res.push_back(internal::val_to_char(r));
        x /= base;
    }
    if (neg) res.push_back('-');

    std::reverse(res.begin(), res.end());
    return res;
}

/**
 * @brief p進数 → q進数 変換（大きい数対応）
 *
 * @details
 *   文字列ベースで安全に変換（桁数制限なし）
 */
inline std::string convert_base(std::string s, int from, int to) {
    assert(2 <= from && from <= 36);
    assert(2 <= to && to <= 36);

    if (s == "0" || from == to) return s;

    std::string res;

    while (!(s.size() == 1 && s[0] == '0')) {
        std::vector<int> digits;
        for (char c : s) {
            int v = internal::char_to_val(c);
            assert(0 <= v && v < from);
            digits.push_back(v);
        }

        std::vector<int> nxt;
        long long carry = 0;

        for (int d : digits) {
            carry = carry * from + d;
            int q = carry / to;
            carry %= to;
            if (!nxt.empty() || q != 0) nxt.push_back(q);
        }

        res.push_back(internal::val_to_char((int)carry));

        if (nxt.empty()) {
            s = "0";
        } else {
            s.clear();
            for (int x : nxt) s.push_back(internal::val_to_char(x));
        }
    }

    std::reverse(res.begin(), res.end());
    return res;
}

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_BASE_CONVERSION_HPP