#ifndef KWM_T_MATH_POLLARD_RHO_HPP
#define KWM_T_MATH_POLLARD_RHO_HPP

#include <vector>
#include <random>
#include <numeric>
#include <cstdint>
#include "base/integer.hpp"
#include "miller_rabin.hpp"

namespace kwm_t::math {

/**
 * @brief Pollard Rho による素因数分解（64bit対応）
 *
 * 計算量:
 *   平均 O(n^{1/4})
 *
 * 制約:
 *   - n < 2^64
 *
 * verified:
 *   https://judge.yosupo.jp/submission/367301
 */
struct PollardRho {
private:
	std::mt19937_64 rng;

	static int128 modmul(int128 a, int128 b, int128 mod) {
		return (a * b) % mod;
	}

	static long long f(long long x, long long c, long long mod) {
		return (modmul(x, x, mod) + c) % mod;
	}

	long long pollard(long long n) {
		if (n % 2 == 0) return 2;

		std::uniform_int_distribution<long long> dist(2, n - 2);

		while (true) {
			long long x = dist(rng);
			long long y = x;
			long long c = dist(rng);
			long long d = 1;

			while (d == 1) {
				x = f(x, c, n);
				y = f(f(y, c, n), c, n);

				long long diff = x > y ? x - y : y - x;
				d = std::gcd(diff, n);
			}

			if (d != n) return d;
		}
	}

	void factor_rec(long long n, std::vector<long long>& res) {
		if (n == 1) return;
		if (MillerRabin::is_prime(n)) {
			res.push_back(n);
			return;
		}

		long long d = pollard(n);
		factor_rec(d, res);
		factor_rec(n / d, res);
	}

public:
	PollardRho() : rng(std::random_device{}()) {}

	/**
	 * @brief 素因数分解(非ソート)
	 */
	std::vector<long long> factor(long long n) {
		std::vector<long long> res;
		factor_rec(n, res);
		return res;
	}
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_POLLARD_RHO_HPP