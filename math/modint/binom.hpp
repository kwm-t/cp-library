#ifndef KWM_T_MATH_MODINT_BINOM_HPP
#define KWM_T_MATH_MODINT_BINOM_HPP

#include <vector>
#include <cassert>

namespace kwm_t::math::modint {
/**
 * @brief 二項係数・順列・多項係数（modint用）
 *
 * 典型用途:
 *   nCk, nPk, 多項係数
 *
 * 計算量:
 *   前計算 O(N)
 *   クエリ O(1)
 *
 * @tparam Mint modint型 (例: atcoder::static_modint<MOD>)
 *
 * 制約 / 注意:
 *   - mod は素数
 *   - n < mod（階乗が0にならない範囲）
 *
 * 使用例:
 *   Binom<mint> C(2000000);
 *   C.com(n, k);
 *
 * verified:
 */
template <class Mint>
struct Binom {
	std::vector<Mint> fact, ifact;

	Binom(int n = 2000006) {
		init(n);
	}

	void init(int n) {
		fact.resize(n + 1);
		ifact.resize(n + 1);

		fact[0] = 1;
		for (int i = 1; i <= n; ++i) {
			fact[i] = fact[i - 1] * i;
		}

		ifact[n] = fact[n].inv();
		for (int i = n; i >= 1; --i) {
			ifact[i - 1] = ifact[i] * i;
		}
	}

	// nCk
	Mint com(int n, int k) const {
		if (k < 0 || k > n) return 0;
		return fact[n] * ifact[k] * ifact[n - k];
	}

	Mint operator()(int n, int k) const {
		return com(n, k);
	}

	// nPk
	Mint perm(int n, int k) const {
		if (k < 0 || k > n) return 0;
		return fact[n] * ifact[n - k];
	}

	// nCk (nが大きくkが小さい場合)
	Mint com_sub(long long n, long long k) const {
		if (k < 0 || k > n) return 0;
		if (n - k < k) k = n - k;
		assert(k < (int)fact.size());

		Mint res = ifact[k];
		for (long long i = 0; i < k; ++i) {
			res *= (n - i);
		}
		return res;
	}

	// 多項係数
	template <class... Ints>
	Mint multinomial(int n, const Ints&... ms) const {
		Mint res = fact[n];
		int sum = 0;

		for (int m : {ms...}) {
			if (m < 0 || m > n) return 0;
			res *= ifact[m];
			sum += m;
		}

		if (sum > n) return 0;
		res *= ifact[n - sum];
		return res;
	}

	// 1/x
	Mint inv(int x) const {
		if (x < (int)fact.size()) {
			return fact[x - 1] * ifact[x];
		}
		return Mint(x).inv();
	}

	// nCk の逆数
	Mint inv_com(int n, int k) const {
		if (k < 0 || k > n) return 0;
		return ifact[n] * fact[k] * fact[n - k];
	}
};

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_BINOM_HPP