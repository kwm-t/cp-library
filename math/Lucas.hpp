/**
 * @file lucas.hpp
 * @brief Lucas の定理
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/lucas.hpp
 */

#ifndef KWM_T_MATH_LUCAS_HPP
#define KWM_T_MATH_LUCAS_HPP

#include <vector>

/**
 * @brief Lucasの定理による nCk mod p (pは素数)
 *
 * n, k が大きくても計算可能
 *
 * 典型用途:
 *   ・n, k <= 1e18 などの組み合わせ
 *   ・mod が小さい素数
 *
 * 計算量:
 *   前処理: O(p)
 *   クエリ: O(log_p n)
 *
 * @param p 素数
 *
 * 制約 / 注意:
 *   ・p は素数
 *   ・p が大きすぎると前処理が重い
 *
 * 使用例:
 *   Lucas lc(p);
 *   auto res = lc.comb(n, k);
 *
 * verified:
 *   ・基本ケース
 */
namespace kwm_t::math {

struct Lucas {
private:
	long long p;
	std::vector<long long> fact, ifact;

	long long mod_pow(long long a, long long e) const {
		long long r = 1;
		while (e) {
			if (e & 1) r = r * a % p;
			a = a * a % p;
			e >>= 1;
		}
		return r;
	}

	long long inv(long long x) const {
		return mod_pow(x, p - 2);
	}

	void build() {
		fact.resize(p);
		ifact.resize(p);

		fact[0] = 1;
		for (int i = 1; i < p; ++i) {
			fact[i] = fact[i - 1] * i % p;
		}

		ifact[p - 1] = inv(fact[p - 1]);
		for (int i = p - 1; i > 0; --i) {
			ifact[i - 1] = ifact[i] * i % p;
		}
	}

	// nCk mod p (n, k < p)
	long long small_comb(long long n, long long k) const {
		if (n < k) return 0;
		return fact[n] * ifact[k] % p * ifact[n - k] % p;
	}

public:
	explicit Lucas(long long p) : p(p) {
		build();
	}

	long long comb(long long n, long long k) const {
		if (n < k) return 0;

		long long res = 1;
		while (n > 0 || k > 0) {
			long long ni = n % p;
			long long ki = k % p;

			if (ni < ki) return 0;

			res = res * small_comb(ni, ki) % p;

			n /= p;
			k /= p;
		}
		return res;
	}
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_LUCAS_HPP