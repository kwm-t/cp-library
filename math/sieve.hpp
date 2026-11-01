/**
 * @file sieve.hpp
 * @brief 篩による素数列挙
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/sieve.hpp
 */
#ifndef KWM_T_MATH_SIEVE_HPP
#define KWM_T_MATH_SIEVE_HPP

#include <vector>
#include <algorithm>
#include <cmath>

namespace kwm_t::math {

/**
 * @brief エラトステネスの篩（bool版）
 * is_prime[i] を返す
 */
inline std::vector<bool> sieve(int n) {
	std::vector<bool> is_prime(n + 1, true);

	if (n >= 0) is_prime[0] = false;
	if (n >= 1) is_prime[1] = false;

	for (int p = 2; p * p <= n; ++p) {
		if (!is_prime[p]) continue;
		for (int i = p * p; i <= n; i += p) {
			is_prime[i] = false;
		}
	}
	return is_prime;
}

/**
 * @brief 素数列挙
 * sieve を利用して primes を返す
 */
inline std::vector<int> primes(int n) {
	auto is_prime = sieve(n);
	std::vector<int> res;

	for (int i = 2; i <= n; ++i) {
		if (is_prime[i]) res.push_back(i);
	}
	return res;
}

/**
 * @brief 区間篩
 * [l, r) に含まれる素数を列挙する
 */
inline std::vector<long long> segmented_sieve(long long l, long long r) {
	std::vector<long long> res;
	if (r <= 2 || l >= r) return res;

	l = std::max(l, 2LL);

	long long n = static_cast<long long>(std::sqrt(r - 1));
	while (n < (r - 1) / n) ++n;
	auto is_prime = sieve(static_cast<int>(n));

	std::vector<bool> ok(r - l, true);

	for (long long p = 2; p <= n; ++p) {
		if (!is_prime[p]) continue;

		long long start = std::max(p * p, ((l + p - 1) / p) * p);
		for (long long i = start; i < r; i += p) {
			ok[i - l] = false;
		}
	}

	for (long long i = 0; i < r - l; ++i) {
		if (ok[i]) res.push_back(l + i);
	}
	return res;
}
}
#endif