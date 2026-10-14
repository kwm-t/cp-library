/**
 * @file golden_section_search.hpp
 * @brief 黄金分割探索
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/search/golden_section_search.hpp
 */
#ifndef KWM_T_ALGORITHM_SEARCH_GOLDENSECTIONSEARCH_HPP
#define KWM_T_ALGORITHM_SEARCH_GOLDENSECTIONSEARCH_HPP

#include <functional>
#include <utility>

/**
 * @brief 黄金分割探索 (整数)
 *
 * @details
 *  単峰関数 (unimodal function) に対して最小値または最大値を探索する。
 *  三分探索と異なり、評価回数を削減できる（キャッシュ再利用）。
 *
 *  内部的には Fibonacci に基づく探索を行う。
 *
 * Typical Use:
 *  ・評価関数が重い場合の最適化
 *  ・ラグランジュ緩和の λ 最適化
 *
 * Complexity:
 *  Time: O(log(r-l))
 *  （評価回数は三分探索より少ない）
 *
 * Template Parameters:
 *  T : 整数型 (int / long long など)
 *  F : 評価関数 f(T)
 *  Compare : 比較関数 (std::less / std::greater)
 *
 * Parameters:
 *  l : 探索区間左端
 *  r : 探索区間右端
 *  f : 評価関数
 *  comp : 比較関数
 *
 * Return:
 *  {最適な位置, その値}
 *
 * Requirements:
 *  f は単峰関数であること
 *
 * Notes:
 *  ・三分探索より高速になるケースあり（特に f が重い場合）
 *  ・実装はややトリッキー
 *
 * Example:
 *  ```cpp
 *  auto f = [&](long long x){
 *      return (x - 10) * (x - 10);
 *  };
 *
 *  auto res = golden_section_search(0LL, 100LL, f);
 *  ```
 *
 * Verified:
 *
 */
namespace kwm_t::algorithm::search {

template <class T, class F, class Compare = std::less<>>
auto golden_section_search(T l, T r, F f, Compare comp = Compare()) {
	using U = decltype(f(l));

	// Fibonacci-like expansion
	T s = 1, t = 2;
	while (t < r - l + 2) {
		std::swap(s += t, t);
	}

	T base = l - 1;
	T m1 = base + t - s;
	T m2 = base + s;

	U v1 = f(m1), v2 = f(m2);

	while (m1 != m2) {
		std::swap(s, t -= s);

		if (r < m2 || comp(v1, v2)) {
			m2 = m1;
			v2 = v1;
			m1 = base + t - s;
			v1 = f(m1);
		}
		else {
			base = m1;
			m1 = m2;
			v1 = v2;
			m2 = base + s;
			v2 = (m2 <= r ? f(m2) : v1);
		}
	}

	return std::pair{ m1, v1 };
}

} // namespace kwm_t::algorithm::search


/**
 * @brief 黄金分割探索 (実数)
 *
 * @details
 *  実数区間に対する黄金分割探索。
 *  評価値を再利用しながら収束させる。
 *
 * Typical Use:
 *  ・実数最適化
 *  ・評価関数が重い場合
 *
 * Complexity:
 *  Time: O(counter)
 *
 * Template Parameters:
 *  T : 浮動小数点型 (double / long double)
 *  F : 評価関数
 *  Compare : 比較関数
 *
 * Parameters:
 *  l : 左端
 *  r : 右端
 *  f : 評価関数
 *  counter : 反復回数
 *  comp : 比較関数
 *
 * Return:
 *  {近似位置, その値}
 *
 * Requirements:
 *  f は単峰関数
 *
 * Example:
 *  ```cpp
 *  auto f = [&](double x){
 *      return (x - 3.14) * (x - 3.14);
 *  };
 *
 *  auto res = golden_section_search_double(0.0, 10.0, f);
 *  ```
 *
 */
namespace kwm_t::algorithm::search {

template <class T, class F, class Compare = std::less<>>
auto golden_section_search_double(T l, T r, F f, int counter = 100, Compare comp = Compare()) {
	using U = decltype(f(l));

	const T phi = (1 + std::sqrt((T)5)) / 2;

	T m1 = r - (r - l) / phi;
	T m2 = l + (r - l) / phi;

	U v1 = f(m1), v2 = f(m2);

	while (counter--) {
		if (comp(v1, v2)) {
			r = m2;
			m2 = m1;
			v2 = v1;
			m1 = r - (r - l) / phi;
			v1 = f(m1);
		}
		else {
			l = m1;
			m1 = m2;
			v1 = v2;
			m2 = l + (r - l) / phi;
			v2 = f(m2);
		}
	}

	return std::pair{ l, f(l) };
}

} // namespace kwm_t::algorithm::search

#endif // KWM_T_ALGORITHM_SEARCH_GOLDENSECTIONSEARCH_HPP