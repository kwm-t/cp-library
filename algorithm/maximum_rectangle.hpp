/**
 * @file maximum_rectangle.hpp
 * @brief 最大長方形
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/maximum_rectangle.hpp
 */
#ifndef KWM_T_ALGORITHM_MAXIMUM_RECTANGLE_HPP
#define KWM_T_ALGORITHM_MAXIMUM_RECTANGLE_HPP

#include <vector>
#include <stack>
#include <algorithm>
#include <cassert>

namespace kwm_t::algorithm {

/**
 * @brief ヒストグラム最大長方形
 *
 * @details
 *   各高さ a[i] を底辺とする最大の長方形を考え、
 *   スタックを用いて O(N) で最大面積を求める。
 *
 * 典型用途:
 *   - ヒストグラム最大長方形問題
 *   - 2次元グリッドの最大長方形（各行をヒストグラム化）
 *
 * 計算量:
 *   - O(N)
 *
 * @param a
 *   各棒の高さ（非負を想定）
 *
 * @return
 *   最大長方形の面積
 *
 * 制約 / 注意:
 *   - a[i] >= 0 を想定
 *   - 面積は long long で返す
 *
 * 使用例:
 *   std::vector<int> a = {2,1,5,6,2,3};
 *   long long res = maximum_rectangle(a); // = 10
 *
 * verified:
 *   - AOJ DSL_3_C
 *   - AtCoder 典型問題
 */
template <class T>
long long maximum_rectangle(const std::vector<T>& a) {
	const int n = (int)a.size();
	std::vector<int> L(n), R(n);
	std::stack<int> st;

	// 左端
	for (int i = 0; i < n; i++) {
		while (!st.empty() && a[st.top()] >= a[i]) st.pop();
		L[i] = st.empty() ? 0 : st.top() + 1;
		st.push(i);
	}

	while (!st.empty()) st.pop();

	// 右端
	for (int i = n - 1; i >= 0; i--) {
		while (!st.empty() && a[st.top()] >= a[i]) st.pop();
		R[i] = st.empty() ? n : st.top();
		st.push(i);
	}

	long long res = 0;
	for (int i = 0; i < n; i++) {
		long long width = R[i] - L[i];
		long long area = (long long)a[i] * width;
		res = std::max(res, area);
	}

	return res;
}

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_MAXIMUM_RECTANGLE_HPP