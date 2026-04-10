#ifndef KWM_T_MATH_HOCKEY_STICK_HPP
#define KWM_T_MATH_HOCKEY_STICK_HPP

#include <cassert>

namespace kwm_t::math::modint {

/**
 * @brief Hockey Stick Identity（ホッケースティックの定理）
 *
 * Σ_{i=k}^{n} C(i, k) = C(n+1, k+1)
 *
 * Binom クラスを直接利用する版
 *
 * verified:
 */

 /**
  * @brief Σ_{i=k}^{n} C(i,k)
  */
template <class Binom>
auto hockey_stick(int n, int k, const Binom& C) {
	using Mint = decltype(C.com(0, 0));
	if (n < k) return Mint(0);
	return C.com(n + 1, k + 1);
}

/**
 * @brief Σ_{i=l}^{r} C(i,k)
 */
template <class Binom>
auto hockey_stick(int l, int r, int k, const Binom& C) {
	using Mint = decltype(C.com(0, 0));

	if (l > r) return Mint(0);
	if (r < k) return Mint(0);

	Mint res = hockey_stick(r, k, C);
	if (l - 1 >= k) {
		res -= hockey_stick(l - 1, k, C);
	}
	return res;
}

/**
 * @brief Σ_{i=k}^{n} C(i,k) * i
 *
 * 恒等式:
 *   Σ i*C(i,k) = (k+1) * C(n+2, k+2) - C(n+1, k+1)
 */
template <class Binom>
auto hockey_stick_weighted(int n, int k, const Binom& C) {
	using Mint = decltype(C.com(0, 0));
	if (n < k) return Mint(0);

	return Mint(k + 1) * C.com(n + 2, k + 2)
		- C.com(n + 1, k + 1);
}

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_HOCKEY_STICK_HPP