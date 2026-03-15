#ifndef ALGORITHM_SORT_MERGESORT_HPP
#define ALGORITHM_SORT_MERGESORT_HPP

#include <vector>
#include <utility>

/**
 * @brief Merge Sort
 *
 * @details
 *  マージソートによる安定ソート。
 *  比較関数を指定してソートできる。
 *
 * Typical Use:
 *  - 安定ソート
 *  - カスタム比較関数
 *
 * Complexity:
 *  Time: O(N log N)
 *  Memory: O(N)
 *
 * Template Parameters:
 *  T : 要素型
 *  F : 比較関数 (bool f(T a, T b))
 *
 * Parameters:
 *  vec : ソート対象配列
 *  f   : 比較関数
 *
 * Return:
 *  ソートされた配列
 *
 * Example:
 *  ```cpp
 *  vector<int> a = {3,1,4,1,5};
 *
 *  a = algorithm::sort::mergeSort(a, [](int l, int r) {
 *      return l < r;
 *  });
 *  ```
 *
 * Verified:
 *
 */

namespace algorithm::sort {

	template<typename T, class F>
	std::vector<T> mergeSort(std::vector<T> vec, const F &f) {

		auto dfs = [&](auto &&self, size_t l, size_t r) -> void {

			if (l + 1 >= r) return;

			size_t m = (l + r) / 2;

			self(self, l, m);
			self(self, m, r);

			std::vector<T> d;
			d.reserve(r - l);

			size_t i0 = l;
			size_t i1 = m;

			while (i0 < m || i1 < r) {

				if (i0 == m) {
					d.push_back(std::move(vec[i1++]));
				}
				else if (i1 == r) {
					d.push_back(std::move(vec[i0++]));
				}
				else {
					if (f(vec[i0], vec[i1]))
						d.push_back(std::move(vec[i0++]));
					else
						d.push_back(std::move(vec[i1++]));
				}
			}

			for (size_t i = l; i < r; ++i) {
				vec[i] = std::move(d[i - l]);
			}
		};

		dfs(dfs, 0, vec.size());

		return vec;
	}

}

#endif // ALGORITHM_SORT_MERGESORT_HPP