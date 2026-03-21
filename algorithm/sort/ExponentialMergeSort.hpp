#ifndef KWM_T_ALGORITHM_SORT_EXPONENTIAL_MERGESORT_HPP
#define KWM_T_ALGORITHM_SORT_EXPONENTIAL_MERGESORT_HPP

#include <vector>
#include <iterator>

/**
 * @brief Exponential Merge Sort
 *
 * @details
 *  マージ時に指数探索を使う Merge Sort。
 *  片側に連続して要素がある場合に比較回数を削減する。
 *
 * Complexity:
 *  Time  : O(N log N)
 *  Memory: O(N)
 *
 * Template:
 *  RandomAccessIterator
 *  Compare
 *
 * Example:
 *  vector<int> a = {3,1,4,1,5};
 *  algorithm::sort::exponential_merge_sort(
 *      a.begin(), a.end(),
 *      [](int l,int r){ return l < r; }
 *  );
 *
 * Verified:
 *  https://atcoder.jp/contests/abc434/submissions/74143077
 */
namespace kwm_t::algorithm::sort {

namespace detail {

template<class It>
It safe_next(It it, long long n, It bound) {
	while (n-- > 0 && it != bound) ++it;
	return it;
}

#if __cpp_lib_ranges
using std::ranges::next;
#else
template<class It>
It next(It it, long long n, It bound) {
	return safe_next(it, n, bound);
}
#endif

} // namespace detail

template<class RandomAccessIterator, class Compare>
constexpr void exponential_merge_sort(
	RandomAccessIterator first,
	RandomAccessIterator last,
	Compare comp
) {

	auto len = last - first;
	if (len <= 1) return;

	auto mid = first + len / 2;

	exponential_merge_sort(first, mid, comp);
	exponential_merge_sort(mid, last, comp);

	using T = typename std::iterator_traits<RandomAccessIterator>::value_type;

	std::vector<T> a(first, mid);
	std::vector<T> b(mid, last);

	auto ai = a.begin();
	auto bi = b.begin();

	bool side = false;

	while (ai != a.end() || bi != b.end()) {

		if (ai == a.end()) {
			while (bi != b.end()) {
				*first++ = *bi++;
			}
			break;
		}
		else if (bi == b.end()) {
			while (ai != a.end()) {
				*first++ = *ai++;
			}
			break;
		}

		side = !side;

		if (side) {
			// kick a.begin()

			long long d = 1;

			while (true) {
				auto it = detail::next(bi, d - 1, b.end());
				if (it == b.end()) break;
				if (comp(*it, *ai)) d <<= 1;
				else break;
			}

			d >>= 1;
			long long c = d;

			while (d >= 2) {
				d >>= 1;
				auto it = detail::next(bi, c + d - 1, b.end());
				if (it == b.end()) continue;
				if (comp(*it, *ai)) c += d;
			}

			while (c--) {
				*first++ = *bi++;
			}

			*first++ = *ai++;
		}
		else {
			// kick b.begin()

			long long d = 1;

			while (true) {
				auto it = detail::next(ai, d - 1, a.end());
				if (it == a.end()) break;
				if (comp(*it, *bi)) d <<= 1;
				else break;
			}

			d >>= 1;
			long long c = d;

			while (d >= 2) {
				d >>= 1;
				auto it = detail::next(ai, c + d - 1, a.end());
				if (it == a.end()) continue;
				if (comp(*it, *bi)) c += d;
			}

			while (c--) {
				*first++ = *ai++;
			}

			*first++ = *bi++;
		}
	}
}

} // namespace kwm_t::algorithm::sort

#endif //KWM_T_ALGORITHM_SORT_EXPONENTIAL_MERGESORT_HPP