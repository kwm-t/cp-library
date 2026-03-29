#ifndef KWM_T_MATH_SEIVE_OF_ERASTOSTHENES_HPP
#define KWM_T_MATH_SEIVE_OF_ERASTOSTHENES_HPP

#include <vector>

namespace kwm_t::math {

// エラトステネスの篩：n 以下の素数列挙
inline std::vector<long long> sieve_of_eratosthenes(long long n = 1000000) {
    std::vector<bool> is_prime(n + 1, true);
    std::vector<long long> primes;

    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (long long i = 2; i <= n; i++) {
        if (!is_prime[i]) continue;
        primes.push_back(i);

        // i*i から始める（高速化 & オーバーフロー対策）
        if (i * i > n) continue;
        for (long long j = i * i; j <= n; j += i) {
            is_prime[j] = false;
        }
    }

    return primes;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_SEIVE_OF_ERASTOSTHENES_HPP