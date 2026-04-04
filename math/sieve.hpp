#ifndef KWM_T_MATH_SIEVE_HPP
#define KWM_T_MATH_SIEVE_HPP

#include <vector>

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
}

#endif