#ifndef KWM_T_STRING_BIGINT_STRING_HPP
#define KWM_T_STRING_BIGINT_STRING_HPP

#include <string>
#include <algorithm>
#include <type_traits>

/**
 * @brief 文字列による大整数演算（非負整数）
 *
 * 10進数文字列に対して整数型Tを使った演算を行う
 *
 * 対応:
 *   - 加算（T）
 *   - 乗算（T）
 *   - 除算（T）
 *   - mod（T）
 *
 * 制約 / 注意:
 *   - s は非負整数（先頭0なし or "0"）
 *   - T は整数型
 *
 * 計算量:
 *   O(|s|)
 */
namespace kwm_t::string {

// -------------------------
// s + d
// -------------------------
template <typename T>
inline std::string add_int(const std::string& s, T d) {
    static_assert(std::is_integral_v<T>, "T must be integral");

    T carry = d;
    std::string res;
    res.reserve(s.size() + 20);

    for (int i = (int)s.size() - 1; i >= 0; --i) {
        carry += (s[i] - '0');
        res += char('0' + (carry % 10));
        carry /= 10;
    }

    while (carry) {
        res += char('0' + (carry % 10));
        carry /= 10;
    }

    std::reverse(res.begin(), res.end());
    return res;
}

// -------------------------
// s * n
// -------------------------
template <typename T>
inline std::string mul_int(const std::string& s, T n) {
    static_assert(std::is_integral_v<T>, "T must be integral");

    if (n == 0) return "0";

    std::string res;
    res.reserve(s.size() + 20);

    T carry = 0;

    for (int i = (int)s.size() - 1; i >= 0; --i) {
        carry += (s[i] - '0') * n;
        res += char('0' + (carry % 10));
        carry /= 10;
    }

    while (carry) {
        res += char('0' + (carry % 10));
        carry /= 10;
    }

    std::reverse(res.begin(), res.end());
    return res;
}

// -------------------------
// s / d
// -------------------------
template <typename T>
inline std::string div_int(const std::string& s, T d) {
    static_assert(std::is_integral_v<T>, "T must be integral");

    T cur = 0;
    std::string res;
    res.reserve(s.size());

    for (int i = 0; i < (int)s.size(); ++i) {
        cur = cur * 10 + (s[i] - '0');
        T q = cur / d;
        cur %= d;

        if (q == 0 && res.empty()) continue;
        res += char('0' + q);
    }

    if (res.empty()) return "0";
    return res;
}

// -------------------------
// s % m
// -------------------------
template <typename T>
inline T mod_int(const std::string& s, T m) {
    static_assert(std::is_integral_v<T>, "T must be integral");

    T res = 0;

    for (char c : s) {
        res = (res * 10 + (c - '0')) % m;
    }

    return res;
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_BIGINT_STRING_HPP