#ifndef KWM_T_MATH_EULER_PENTAGONAL_HPP
#define KWM_T_MATH_EULER_PENTAGONAL_HPP

#include <vector>

/**
 * @brief オイラーの五角数定理（生成関数係数）
 *
 * f(x^k) = Π_{i=1}^{∞} (1 - x^{i*k})
 * の展開における先頭 n+1 項の係数を返す
 *
 * すなわち
 *   f(x^k) = Σ_{i=-∞}^{∞} (-1)^i x^{k * i(3i-1)/2}
 *
 * 典型用途:
 *   - 分割数DP（partition function）
 *   - 生成関数の逆数計算
 *   - 多項式積の高速化（形式的冪級数）
 *
 * 計算量:
 *   O(√n)
 *
 * @tparam mint modint型など
 *
 * @param n 最大次数
 * @param k スケーリング（通常は1）
 * @return std::vector<mint> 係数列
 *
 * 制約 / 注意:
 *   - result[i] = [x^i] Π(1 - x^{j*k})
 *   - 無限積だが、次数 n までなら有限項でOK
 *   - i(3i-1)/2 は五角数
 *
 * 使用例:
 *   auto f = kwm_t::math::euler_pentagonal<mint>(10);
 *
 * verified:
 *   - https://yukicoder.me/submissions/1154654
 *   - https://atcoder.jp/contests/abc279/submissions/75200580
 */
namespace kwm_t::math {

template <class T>
std::vector<T> euler_pentagonal(int n, int k = 1) {
    std::vector<T> result(n + 1);

    for (long long i = 0;; ++i) {
        long long g1 = i * (3 * i - 1) / 2;
        long long g2 = i * (3 * i + 1) / 2;

        if (g1 * k > n && g2 * k > n) break;

        T sign = (i % 2 == 0 ? 1 : -1);

        if (g1 * k <= n) result[g1 * k] += sign;
        if (i != 0 && g2 * k <= n) result[g2 * k] += sign;
    }

    return result;
}

// 非零項のみをスパース形式で返す
template <typename T>
std::vector<std::pair<long long, T>> euler_pentagonal_sparse(long long n) {
	std::vector<std::pair<long long, T>> res;
	if (n <= 0) return res;

	res.emplace_back(0LL, T(1));

	for (long long k = 1;; ++k) {
		long long g1 = k * (3 * k - 1) / 2;
		if (g1 >= n) break;

		T sgn = (k & 1) ? T(-1) : T(1);
		res.emplace_back(g1, sgn);

		long long g2 = k * (3 * k + 1) / 2;
		if (g2 < n) res.emplace_back(g2, sgn);
	}

	return res;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_EULER_PENTAGONAL_HPP