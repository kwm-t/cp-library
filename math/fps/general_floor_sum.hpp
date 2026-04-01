#ifndef KWM_T_MATH_FPS_GENERAL_FLOOR_SUM_HPP
#define KWM_T_MATH_FPS_GENERAL_FLOOR_SUM_HPP

// #include
#include <vector>
#include "factorial.hpp"          // Factorial
#include "faulhaber.hpp"

/**
 * @brief General Floor Sum
 *
 * f_{p,q}(n,m,a,b) = Σ_{i=0}^{n-1} i^p * floor((a*i+b)/m)^q
 *
 * Faulhaber と再帰を用いて多項式時間で計算する
 *
 * 典型用途:
 *   ・床関数付き和の高速計算
 *   ・AtCoderのfloor sum拡張
 *   ・多項式×floor の総和
 *
 * 計算量:
 *   O(pq^2 log n) 程度（pqsum を P とすると O(P^3) に近い）
 *
 * @tparam mint modint型
 *
 * @param pqsum p+q の最大値
 * 参考:
 *   https://qiita.com/sounansya/items/51b39e0d7bf5cc194081
 *   
 * 制約 / 注意:
 *   ・Faulhaber係数が必要
 *   ・mint は pow, +, -, * が使えること
 *   ・intで収まらない時はmodを2つとって復元してください
 * 使用例:
 *   GeneralFloorSum<mint> gfs(5);
 *   auto res = gfs.solve(n, m, a, b);
 *
 * verified:
 *   https://atcoder.jp/contests/abc402/submissions/74587888
 */
namespace kwm_t::math::fps {

template <typename mint>
class GeneralFloorSum {
public:
	GeneralFloorSum(int pqsum) : pqsum(pqsum) {
		init();
	}

	std::vector<std::vector<mint>> solve(long long n, long long m, long long a, long long b) {
		long long a0 = a / m, b0 = b / m;
		long long a1 = a % m, b1 = b % m;

		auto sdp = solve_core(n, m, a1, b1);
		return apply_affine(a0, b0, sdp);
	}

private:
	int pqsum;
	std::vector<std::vector<mint>> faul;
	Factorial<mint> fc;

	void init() {
		fc = Factorial<mint>(pqsum + 10);
		faul.resize(pqsum + 1);
		for (int i = 0; i <= pqsum; ++i) {
			faul[i] = faulhaber(i, i == 0 ? 0 : -1, fc);
		}
	}

	// S_k(n) = Σ i^k (i=0..n-1)
	mint power_sum(int k, long long n) {
		if (n == 0) return 0;
		if (k == 0) return n;

		mint res = 0, powN = 1;
		for (auto c : faul[k]) {
			res += c * powN;
			powN *= n;
		}
		return res;
	}

	std::vector<std::vector<mint>> solve_core(long long n, long long m, long long a, long long b) {
		std::vector<std::vector<mint>> dp(pqsum + 1);

		if (a == 0) {
			for (int p = 0; p <= pqsum; ++p) {
				dp[p].resize(pqsum - p + 1);
				dp[p][0] = power_sum(p, n);
			}
			return dp;
		}

		long long k = (a * (n - 1) + b) / m;
		auto rec = solve(k, a, m, m - b + a - 1);

		for (int p = 0; p <= pqsum; ++p) {
			dp[p].resize(pqsum - p + 1);
			dp[p][0] = power_sum(p, n);

			for (int q = 1; p + q <= pqsum; ++q) {
				dp[p][q] = calc_recursive_term(p, q, n, k, rec);
			}
		}
		return dp;
	}

	mint calc_recursive_term(int p, int q, long long n, long long k,
		const std::vector<std::vector<mint>>& rec) {
		mint res = mint(k).pow(q) * power_sum(p, n);

		for (int i = 0; i < q; ++i) {
			mint tmp = 0;
			for (int j = 0; j <= p + 1; ++j) {
				tmp += faul[p][j] * rec[i][j];
			}
			res -= tmp * fc.binom(q, i);
		}
		return res;
	}

	std::vector<std::vector<mint>> apply_affine(
		long long a0, long long b0,
		const std::vector<std::vector<mint>>& sdp) {

		std::vector<mint> apow(pqsum + 1, 1), bpow(pqsum + 1, 1);
		for (int i = 1; i <= pqsum; ++i) {
			apow[i] = apow[i - 1] * a0;
			bpow[i] = bpow[i - 1] * b0;
		}

		std::vector<std::vector<mint>> dp(pqsum + 1);

		for (int p = 0; p <= pqsum; ++p) {
			dp[p].resize(pqsum - p + 1);
			dp[p][0] = sdp[p][0];

			for (int q = 1; p + q <= pqsum; ++q) {
				dp[p][q] = expand_affine_term(p, q, sdp, apow, bpow);
			}
		}
		return dp;
	}

	mint expand_affine_term(
		int p, int q,
		const std::vector<std::vector<mint>>& sdp,
		const std::vector<mint>& apow,
		const std::vector<mint>& bpow) {

		mint res = 0;

		for (int x = 0; x <= q; ++x) {
			for (int y = 0; x + y <= q; ++y) {
				mint term = fc.binom(q, x) * apow[x];
				term *= fc.binom(q - x, y) * bpow[y];
				term *= sdp[p + x][q - x - y];
				res += term;
			}
		}
		return res;
	}
};

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_GENERAL_FLOOR_SUM_HPP