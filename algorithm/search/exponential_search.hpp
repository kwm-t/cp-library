#ifndef KWM_T_ALGORITHM_SEARCH_EXPONENTIALSEARCH_HPP
#define KWM_T_ALGORITHM_SEARCH_EXPONENTIALSEARCH_HPP

#include <cstdlib>
#include "binarysearch.hpp"

/**
 * @brief 指数探索 (整数)
 *
 * @details
 *  単調性を持つ述語 f(x) に対して、f(x)=true となる最小の x を探索する。
 *  上限が不明な場合に有効で、倍々に探索範囲を広げてから二分探索を行う。
 *
 * Typical Use:
 *  - 上限が不明な二分探索
 *  - 無限範囲での境界探索
 *
 * Complexity:
 *  Time: O(log answer)
 *
 * Template Parameters:
 *  T : 整数型 (int / long long など)
 *  F : bool f(T) を返す関数オブジェクト
 *
 * Parameters:
 *  f : 判定関数 (ある点を境に false → true に変化する)
 *
 * Return:
 *  f(x) == true を満たす最小の x
 *
 * Requirements:
 *  - ある x を境に単調 (false → true)
 *  - 十分大きい x で f(x) == true になること
 *
 * Notes:
 *  - 内部で binarySearch を使用
 *
 * Example:
 *  ```cpp
 *  auto f = [&](long long x){
 *      return x * x >= 100;
 *  };
 *
 *  long long ans = exponentialSearch<long long>(f);
 *  ```
 *
 * Verified:
 *
 */
namespace kwm_t::algorithm::search {

template <class T, class F>
T exponentialSearch(const F &f) {
	T r = 1;

	// 範囲拡張
	while (!f(r)) r <<= 1;

	// binary search
	return binarySearch<T>(r, r >> 1, f);
}

} // namespace kwm_t::algorithm::search

#endif // KWM_T_ALGORITHM_SEARCH_EXPONENTIALSEARCH_HPP