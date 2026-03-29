#ifndef KWM_T_MATH_MATHUTIL_HPP
#define KWM_T_MATH_MATHUTIL_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::math {

// 約数の個数
inline long long divisor_count(long long n) {
    std::vector<long long> exponents;
    for (long long i = 2; i * i <= n; i++) {
        int count = 0;
        while (n % i == 0) {
            count++;
            n /= i;
        }
        if (count != 0) exponents.push_back(count);
    }
    if (n != 1) exponents.push_back(1);

    long long result = 1;
    for (long long e : exponents) {
        result *= (e + 1);
    }
    return result;
}

// 約数列挙
inline std::vector<long long> divisors(long long n) {
    std::vector<long long> result;
    for (long long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            result.push_back(i);
            if (i * i != n) result.push_back(n / i);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

// 素因数分解
inline std::vector<std::pair<long long, int>> prime_factorize(long long n) {
    std::vector<std::pair<long long, int>> result;
    for (long long i = 2; i * i <= n; i++) {
        int count = 0;
        while (n % i == 0) {
            count++;
            n /= i;
        }
        if (count != 0) result.emplace_back(i, count);
    }
    if (n != 1) result.emplace_back(n, 1);
    return result;
}

// オイラーのφ関数
inline long long euler_phi(long long n) {
    long long result = n;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            result -= result / i;
            while (n % i == 0) n /= i;
        }
    }
    if (n != 1) result -= result / n;
    return result;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_MATHUTIL_HPP