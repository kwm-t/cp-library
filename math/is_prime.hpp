#ifndef KWM_T_MATH_IS_PRIME_HPP
#define KWM_T_MATH_IS_PRIME_HPP

#include <cstdint>
#include <vector>

#include "base/integer.hpp"

namespace kwm_t::math {

namespace detail {

/**
 * @brief a^e mod m を計算する。
 *
 * 計算量:
 *   O(log e)
 *
 * @param a 底。
 * @param e 指数。
 * @param m 法。
 *
 * 制約 / 注意:
 *   - m > 0 を仮定する。
 *   - __int128 をサポートするコンパイラが必要。
 */
inline long long mod_pow(int128 a, long long e, long long m) {
	int128 ret = 1;

	while (e > 0) {
		if (e & 1) {
			ret = ret * a % m;
		}
		a = a * a % m;
		e >>= 1;
	}

	return static_cast<long long>(ret);
}

} // namespace detail

/**
 * @brief Miller-Rabin 法により n が素数か判定する。
 *
 * 計算量:
 *   O(log n)
 *
 * 制約 / 注意:
 *   - n >= 0 を仮定する。
 *   - Miller-Rabin 法による素数判定。
 *   - 一般の n に対しては確率的アルゴリズムだが、
 *     基底 {2, 3, 5, 7, 11} を用いる本実装では、
 *     n <= 10^12 の範囲において誤りなく素数判定を行える。
 *   - __int128 をサポートするコンパイラが必要。
 *
 * 使用例:
 *   bool p1 = is_prime(2);          // true
 *   bool p2 = is_prime(1000000007); // true
 *   bool p3 = is_prime(1000000008); // false
 *
 * verified:
 *   -
 */
inline bool is_prime(long long n) {
	if (n <= 2) return n == 2;
	if (n % 2 == 0) return false;

	const std::vector<long long> a_list = { 2, 3, 5, 7, 11 };

	// n - 1 = d * 2^s (d は奇数)
	long long d = (n - 1) / ((n - 1) & (1 - n));

	for (long long a : a_list) {
		long long t = d;
		a = detail::mod_pow(a, t, n);
		if (a == 0 || a == 1) continue;
		while (a != n - 1) {
			t *= 2;
			if (t == n - 1) return false;
			a = static_cast<long long>(static_cast<int128>(a) * a % n);
			if (a == 1) return false;
		}
	}
	return true;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_IS_PRIME_HPP