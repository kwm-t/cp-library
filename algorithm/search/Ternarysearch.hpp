#ifndef ALGORITHM_SEARCH_TERNARYSEARCH_HPP
#define ALGORITHM_SEARCH_TERNARYSEARCH_HPP

#include <functional>
#include <utility>

/**
 * @brief 三分探索 (整数)
 *
 * @details
 *  単峰関数 (unimodal function) に対して最小値または最大値を探索する。
 *  Compare により最小値 / 最大値のどちらも求めることができる。
 *
 * Typical Use:
 *  凸関数 / 凹関数の最小値・最大値探索
 *
 * Complexity:
 *  Time: O(log(r-l)) + O(1)
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
 *  区間が小さくなった後は全探索する
 *
 * Example:
 *  ```cpp
 *  auto f = [&](long long x){
 *      return (x - 10) * (x - 10);
 *  };
 *
 *  // 最小値
 *  auto res = ternary_search(0LL, 100LL, f);
 *
 *  // 最大値
 *  auto res2 = ternary_search(0LL, 100LL, f, std::greater<>());
 *  ```
 *
 * Verified:
 *
 */
namespace algorithm::search {

	template <class T, class F, class Compare = std::less<>>
	auto ternary_search(T l, T r, F f, Compare comp = Compare()) {
		using U = decltype(f(l));

		while (r - l > 2) {
			T ml = l + (r - l) / 3;
			T mr = r - (r - l) / 3;

			if (comp(f(ml), f(mr))) r = mr;
			else l = ml;
		}

		T best = l;
		U val = f(l);

		for (T i = l + 1; i <= r; ++i) {
			U v = f(i);
			if (comp(v, val)) {
				best = i;
				val = v;
			}
		}

		return std::pair{ best, val };
	}
}

/**
 * @brief 三分探索 (実数)
 *
 * @details
 *  実数区間に対する三分探索。
 *  counter 回反復することで近似解を求める。
 *
 * Typical Use:
 *  実数値の最小値 / 最大値探索
 *
 * Complexity:
 *  Time: O(counter)
 *
 * Template Parameters:
 *  T : 浮動小数点型 (double / long double)
 *  F : 評価関数 f(T)
 *  Compare : 比較関数
 *
 * Parameters:
 *  l : 探索区間左端
 *  r : 探索区間右端
 *  f : 評価関数
 *  counter : 反復回数
 *  comp : 比較関数
 *
 * Return:
 *  {近似位置, その値}
 *
 * Requirements:
 *  f は単峰関数であること
 *
 * Example:
 *  ```cpp
 *  auto f = [&](double x){
 *      return (x - 3.14) * (x - 3.14);
 *  };
 *
 *  auto res = ternary_search_double(0.0, 10.0, f);
 *  ```
 *
 * Verified:
 *
 */
namespace algorithm::search {
	template <class T, class F, class Compare = std::less<>>
	auto ternary_search_double(T l, T r, F f, int counter = 100, Compare comp = Compare()) {
		using U = decltype(f(l));

		while (counter--) {
			T ml = l + (r - l) / 3;
			T mr = r - (r - l) / 3;

			if (comp(f(ml), f(mr))) r = mr;
			else l = ml;
		}

		return std::pair{ l, f(l) };
	}

} // namespace algorithm::search

#endif // ALGORITHM_SEARCH_TERNARYSEARCH_HPP