#ifndef KWM_T_DATA_STRUCTURE_CONVEX_HULL_TRICK_ADD_MONOTONE_HPP
#define KWM_T_DATA_STRUCTURE_CONVEX_HULL_TRICK_ADD_MONOTONE_HPP

#include <deque>
#include <utility>
#include <cassert>
#include <cmath>

namespace kwm_t::data_structure {

/**
 * @brief Convex Hull Trick (Add Monotone)
 *
 * 典型用途:
 *   - 直線集合に対する最小値 / 最大値クエリ
 *   - 傾きが単調に追加される場合のDP最適化
 *
 * 計算量:
 *   - add: amortized O(1)
 *   - query: O(log N)
 *   - monotone query: O(1)
 * verified
 *   https://atcoder.jp/contests/abc228/submissions/75110284
 *   https://atcoder.jp/contests/abc289/submissions/75110314
 */
template <typename T, bool isMin>
struct ConvexHullTrickAddMonotone {
	std::deque<std::pair<T, T>> H;

	ConvexHullTrickAddMonotone() = default;

	bool empty() const { return H.empty(); }
	void clear() { H.clear(); }

	inline int sgn(T x) { return x == 0 ? 0 : (x < 0 ? -1 : 1); }

	using D = long double;

	inline bool check(const std::pair<T, T> &a,
		const std::pair<T, T> &b,
		const std::pair<T, T> &c) {
		if (b.second == a.second || c.second == b.second)
			return sgn(b.first - a.first) * sgn(c.second - b.second) >=
			sgn(c.first - b.first) * sgn(b.second - a.second);

		return
			D(b.first - a.first) * sgn(c.second - b.second) / D(std::abs(b.second - a.second)) >=
			D(c.first - b.first) * sgn(b.second - a.second) / D(std::abs(c.second - b.second));
	}

	void add(T a, T b) {
		if (!isMin) a *= -1, b *= -1;
		std::pair<T, T> line(a, b);

		if (empty()) {
			H.emplace_front(line);
			return;
		}

		if (H.front().first <= a) {
			if (H.front().first == a) {
				if (H.front().second <= b) return;
				H.pop_front();
			}
			while (H.size() >= 2 && check(line, H.front(), H[1])) H.pop_front();
			H.emplace_front(line);
		}
		else {
			assert(a <= H.back().first);
			if (H.back().first == a) {
				if (H.back().second <= b) return;
				H.pop_back();
			}
			while (H.size() >= 2 && check(H[H.size() - 2], H.back(), line)) H.pop_back();
			H.emplace_back(line);
		}
	}

	inline T get_y(const std::pair<T, T> &a, const T &x) {
		return a.first * x + a.second;
	}

	T query(T x) {
		assert(!empty());
		int l = -1, r = (int)H.size() - 1;
		while (l + 1 < r) {
			int m = (l + r) >> 1;
			if (get_y(H[m], x) >= get_y(H[m + 1], x)) l = m;
			else r = m;
		}
		if (isMin) return get_y(H[r], x);
		return -get_y(H[r], x);
	}

	T query_monotone_inc(T x) {
		assert(!empty());
		while (H.size() >= 2 && get_y(H.front(), x) >= get_y(H[1], x))
			H.pop_front();
		if (isMin) return get_y(H.front(), x);
		return -get_y(H.front(), x);
	}

	T query_monotone_dec(T x) {
		assert(!empty());
		while (H.size() >= 2 &&
			get_y(H.back(), x) >= get_y(H[H.size() - 2], x))
			H.pop_back();
		if (isMin) return get_y(H.back(), x);
		return -get_y(H.back(), x);
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_CONVEX_HULL_TRICK_ADD_MONOTONE_HPP