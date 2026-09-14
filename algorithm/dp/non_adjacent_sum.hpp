#ifndef KWM_T_ALGORITHM_DP_NON_ADJACENT_SUM_HPP
#define KWM_T_ALGORITHM_DP_NON_ADJACENT_SUM_HPP

#include <cassert>
#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace kwm_t::algorithm::dp {

/**
 * @brief 隣接する要素を同時に選ばず、ちょうど k 個選んだときの最小和を求める。
 *
 * ret[i] は、v から隣接する要素を同時に選ばずにちょうど i 個選んだときの
 * 要素の総和の最小値。
 *
 * ret[0] = 0。
 *
 * 計算量: O(N log N)
 * 空間計算量: O(N)
 *
 * @tparam T 要素の型
 * @param v 要素列
 * @param k 選ぶ要素数
 * @param inf 番兵に用いる十分大きな値
 *
 * verify
 *  https://atcoder.jp/contests/awc0156/submissions/79265167
 */
template <typename T>
std::vector<T> min_non_adjacent_sum(const std::vector<T>& v, int k, T inf) {
	const int n = (int)v.size();

	assert(0 <= k);
	assert(k <= (n + 1) / 2);

	std::vector<T> data;
	data.reserve(n + 4);

	data.push_back(inf);
	data.push_back(inf);
	for (T x : v) data.push_back(x);
	data.push_back(inf);
	data.push_back(inf);

	const int size = (int)data.size();

	std::priority_queue<std::pair<T, int>,
		std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> pq;

	std::vector<int> left(size);
	std::vector<int> right(size);
	std::vector<bool> alive(size, true);

	for (int i = 0; i < size; ++i) {
		left[i] = i - 1;
		right[i] = i + 1;
		pq.emplace(data[i], i);
	}

	std::vector<T> ret;
	ret.reserve(k + 1);
	ret.push_back(T(0));

	T sum = T(0);
	while ((int)ret.size() <= k) {
		auto [value, index] = pq.top();
		pq.pop();
		if (!alive[index]) continue;

		sum += value;
		ret.push_back(sum);

		const int li = left[index];
		const int ri = right[index];
		data[index] = data[li] + data[ri] - data[index];
		pq.emplace(data[index], index);

		{
			alive[li] = false;
			const int l = left[li];
			const int r = right[li];
			left[r] = l;
			right[l] = r;
		}

		{
			alive[ri] = false;
			const int l = left[ri];
			const int r = right[ri];
			left[r] = l;
			right[l] = r;
		}
	}

	return ret;
}

/**
 * @brief 隣接する要素を同時に選ばず、ちょうど k 個選んだときの最大和を求める。
 *
 * ret[i] は、v から隣接する要素を同時に選ばずにちょうど i 個選んだときの
 * 要素の総和の最大値。
 *
 * ret[0] = 0。
 *
 * @tparam T 要素の型
 * @param v 要素列
 * @param k 選ぶ要素数
 * @param inf 番兵に用いる十分大きな値
 */
template <typename T>
std::vector<T> max_non_adjacent_sum(const std::vector<T>& v, int k, T inf) {
	std::vector<T> negated(v.size());
	for (int i = 0; i < (int)v.size(); ++i)negated[i] = -v[i];
	std::vector<T> ret = min_non_adjacent_sum(negated, k, inf);
	for (T& x : ret) x = -x;
	return ret;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_NON_ADJACENT_SUM_HPP