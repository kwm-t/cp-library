#ifndef KWM_T_MATH_CRT_HPP
#define KWM_T_MATH_CRT_HPP

#include <tuple>
#include <utility>
#include <vector>

namespace kwm_t::math {

template<class T>
T safe_mod(T x, T m) {
	x %= m;
	if (x < 0) x += m;
	return x;
}

template<class T>
std::pair<T, T> inv_gcd(T a, T b) {
	a = safe_mod(a, b);
	if (a == 0) return { b, 0 };

	T s = b, t = a;
	T m0 = 0, m1 = 1;

	while (t != 0) {
		T u = s / t;

		s -= t * u;
		m0 -= m1 * u;

		std::swap(s, t);
		std::swap(m0, m1);
	}

	if (m0 < 0) m0 += b / s;

	return { s, m0 };
}

/**
 * @brief 中国剰余定理 (Chinese Remainder Theorem)
 *
 * x ≡ r[i] (mod m[i]) を満たす最小非負整数 x を求める。
 * ACLのコピーコード、__int128, boost::multiprecision::cpp_intに対応させるため
 *
 * 計算量:
 * O(N log M)
 *
 * @tparam T
 * 整数型
 * (long long, __int128, boost::multiprecision::cpp_int など)
 *
 * @param r
 * 余り
 *
 * @param m
 * 法 (正)
 *
 * @return
 * {x, lcm}
 *
 * 解が存在しない場合は {0,0} を返す。
 *
 * verified:
 * https://atcoder.jp/contests/awc0105/submissions/77149970
 */
template<class T>
std::pair<T, T> crt(
	const std::vector<T>& r,
	const std::vector<T>& m) {

	T r0 = 0;
	T m0 = 1;

	for (int i = 0; i < (int)r.size(); i++) {

		T r1 = safe_mod(r[i], m[i]);
		T m1 = m[i];

		if (m0 < m1) {
			std::swap(r0, r1);
			std::swap(m0, m1);
		}

		if (m0 % m1 == 0) {
			if (r0 % m1 != r1) return { 0,0 };
			continue;
		}

		T g, im;
		std::tie(g, im) = inv_gcd(m0, m1);

		if ((r1 - r0) % g != 0) return { 0,0 };

		T u1 = m1 / g;

		T x = ((r1 - r0) / g) % u1;
		x = (x * im) % u1;

		r0 += x * m0;
		m0 *= u1;

		if (r0 < 0) r0 += m0;
	}

	return { r0, m0 };
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_CRT_HPP