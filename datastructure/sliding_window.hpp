/**
 * @file sliding_window.hpp
 * @brief スライディングウィンドウ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/sliding_window.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_SLIDING_WINDOW_AGGREGATION_HPP
#define KWM_T_DATA_STRUCTURE_SLIDING_WINDOW_AGGREGATION_HPP

#include <deque>
#include <functional>
#include <cassert>

namespace kwm_t::data_structure {

/**
* @brief スライド最小値 / 最大値（Monotonic Queue）
*
* 典型用途:
*   - 区間最小値 / 最大値のスライドクエリ
*   - DP最適化（尺取り・deque最適化）
*
* 計算量:
*   - push: amortized O(1)
*   - query: O(1)
*
* @tparam T
*   比較可能な型
* @tparam Compare
*   比較関数（std::less で最小値、std::greater で最大値）
*
* 使用例:
*   SlidingWindow<int> sw;
*   sw.push(i, a[i]);
*   int mn = sw.get(l);
*
* verified:
*   - https://atcoder.jp/contests/awc0068/submissions/75770550
*/
template <class T, class Compare = std::less<T>>
struct SlidingWindow {
private:
	std::deque<std::pair<int, T>> dq;
	Compare comp;

public:
	SlidingWindow() = default;

	// (index, value)
	void push(int i, const T& x) {
		while (!dq.empty() && !comp(dq.back().second, x)) {
			dq.pop_back();
		}
		dq.emplace_back(i, x);
	}

	// 区間左端 l を指定
	T get(int l) {
		while (!dq.empty() && dq.front().first < l) {
			dq.pop_front();
		}
		assert(!dq.empty());
		return dq.front().second;
	}

	bool empty() const {
		return dq.empty();
	}

	void clear() {
		dq.clear();
	}
};

/**
 * @brief スライド区間 min/max をまとめて計算
 *
 * @param a
 *   入力配列
 * @param k
 *   窓幅
 * @return
 *   各長さ k の区間に対する min/max の配列
 *
 * 使用例:
 *   auto mn = sliding_window<int>(a, 3);
 *   auto mx = sliding_window<int, greater<int>>(a, 3);
 *
 * 計算量:
 *   O(N)
 * verify
 *   https://atcoder.jp/contests/awc0068/submissions/75953477
 */
template <class T, class Compare = std::less<T>>
std::vector<T> sliding_window(const std::vector<T>& a, int k) {
	assert(1 <= k && k <= (int)a.size());

	SlidingWindow<T, Compare> sw;
	std::vector<T> res;
	res.reserve(a.size() - k + 1);

	for (int i = 0; i < k - 1; i++) {
		sw.push(i, a[i]);
	}

	for (int i = k - 1; i < (int)a.size(); i++) {
		sw.push(i, a[i]);
		res.push_back(sw.get(i - k + 1));
	}

	return res;
}

} // namespace kwm_t::data_structure

#endif