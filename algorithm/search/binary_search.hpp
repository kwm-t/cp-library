/**
 * @file binarysearch.hpp
 * @brief 二分探索
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/search/binary_search.hpp
 */

#ifndef KWM_T_ALGORITHM_SEARCH_BINARY_SEARCH_HPP
#define KWM_T_ALGORITHM_SEARCH_BINARY_SEARCH_HPP

#include <cstdlib>

namespace kwm_t::algorithm::search {
/**
 * @brief 二分探索 (整数)
 *
 * @details
 *  単調性を持つ述語 f(x) に対して、f(x)=true となる境界を探索する。
 *  初期状態で ok は true、ng は false を満たしている必要がある。
 *
 * Typical Use:
 *  最大/最小の条件を満たす整数を探す
 *
 * Complexity:
 *  Time: O(log |ok-ng|)
 *
 * Template Parameters:
 *  T : 整数型 (int / long long など)
 *  F : bool f(T) を返す関数オブジェクト
 *
 * Parameters:
 *  ok : 条件を満たす値
 *  ng : 条件を満たさない値
 *  f  : 判定関数
 *
 * Return:
 *  条件を満たす境界値
 *
 * Requirements:
 *  f(ok) == true
 *  f(ng) == false
 *
 * Notes:
 *  ok と ng の大小関係は問わない
 *
 * Example:
 *  ```cpp
 *  auto f = [&](long long x){
 *      return x * x <= 100;
 *  };
 *
 *  long long ans = binarySearch(0LL, 11LL, f);
 *  ```
 *
 * Verified:
 *
 */

template <class T, class F>
T binarySearch(T ok, T ng, const F &f) {
	while (std::abs(ok - ng) > 1) {
		T mid = (ok + ng) / 2;
		(f(mid) ? ok : ng) = mid;
	}
	return ok;
}

/**
 * @brief 二分探索 (実数)
 *
 * @details
 *  実数値に対する二分探索。
 *  回数固定で探索を行う。
 *
 * Typical Use:
 *  実数解の近似探索
 *
 * Complexity:
 *  Time: O(counter)
 *
 * Template Parameters:
 *  T : 浮動小数点型 (double / long double)
 *  F : bool f(T) を返す関数オブジェクト
 *
 * Parameters:
 *  ok      : 条件を満たす値
 *  ng      : 条件を満たさない値
 *  f       : 判定関数
 *  counter : 反復回数
 *
 * Return:
 *  条件を満たす近似値
 *
 * Requirements:
 *  f(ok) == true
 *  f(ng) == false
 *
 * Example:
 *  ```cpp
 *  auto f = [&](double x){
 *      return x * x <= 2.0;
 *  };
 *
 *  double ans = binarySearchDouble(0.0, 2.0, f);
 *  ```
 *
 * Verified:
 *
 */

template <class T, class F>
T binarySearchDouble(T ok, T ng, const F &f, int counter = 100) {
	while (counter--) {
		T mid = (ok + ng) / 2;
		(f(mid) ? ok : ng) = mid;
	}
	return ok;
}

} // namespace kwm_t::algorithm::search

#endif // KWM_T_ALGORITHM_SEARCH_BINARY_SEARCH_HPP