#ifndef KWM_T_MATH_MODINT_BINOM_SUM_SQRT_HPP
#define KWM_T_MATH_MODINT_BINOM_SUM_SQRT_HPP

#include <vector>
#include <cmath>
#include <algorithm>

//#include "kwm_t/math/modint/binom.hpp"

namespace kwm_t::math::modint {

/**
 * @brief 二項係数の累積和（平方分割）
 *
 * prefix(m) = Σ_{k=0}^{m} C(n,k)
 *
 * n 固定でクエリが多い場合に有効
 *
 * 計算量:
 *   前計算: O(n√n)
 *   クエリ: O(√n)
 *
 * 制約 / 注意:
 *   - Binom<mint> が構築済みであること
 */
template <class Mint>
class BinomPrefixSumSqrt {
public:
	BinomPrefixSumSqrt(int n, const Binom<Mint>& C)
		: n(n), C(C) {
		build();
	}

	/**
	 * @brief Σ_{k=0}^{m} C(n,k)
	 */
	Mint prefix(int m) const {
		if (m < 0) return Mint(0);
		if (m >= n) return Mint(2).pow(n); // 2^n

		Mint ret = table[n / sqrtn][m];

		int x = (n / sqrtn) * sqrtn;
		while (x < n) {
			ret = ret * 2 - C(x, m);
			++x;
		}
		return ret;
	}

	/**
	 * @brief Σ_{k=l}^{r} C(n,k)
	 */
	Mint range(int l, int r) const {
		if (l > r) return Mint(0);
		return prefix(r) - prefix(l - 1);
	}

private:
	int n;
	int sqrtn;
	std::vector<std::vector<Mint>> table;
	const Binom<Mint>& C;

	void build() {
		sqrtn = std::max(1, (int)std::sqrt(n));

		int sz = n / sqrtn + 1;
		table.assign(sz, std::vector<Mint>(n + 1, Mint(0)));

		for (int i = 0; i < sz; ++i) {
			int x = i * sqrtn;
			for (int j = 0; j <= n; ++j) {
				if (j > 0) table[i][j] = table[i][j - 1];
				table[i][j] += C(x, j);
			}
		}
	}
};

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_BINOM_SUM_SQRT_HPP