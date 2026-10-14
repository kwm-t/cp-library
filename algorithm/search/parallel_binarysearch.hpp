/**
 * @file parallel_binarysearch.hpp
 * @brief 並列二分探索
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/search/parallel_binarysearch.hpp
 */
#ifndef KWM_T_ALGORITHM_SEARCH_PARALLEL_BINARY_SEARCH_HPP
#define KWM_T_ALGORITHM_SEARCH_PARALLEL_BINARY_SEARCH_HPP

#include <vector>
#include <cstdlib>

/**
 * @brief Parallel Binary Search
 *
 * @details
 *  複数クエリに対して「条件を満たす最小の更新番号」を同時に二分探索する。
 *  更新を順に適用できるデータ構造と組み合わせて使用する。
 *
 * Typical Use:
 *  - 「何番目の操作で条件を満たすか」
 *  - 区間更新 + 点取得
 *  - Dynamic connectivity
 *
 * Complexity:
 *  Time: O((update_cost + query_cost) * log K)
 *
 * Template Parameters:
 *  Reset : データ構造を初期化する関数 (void())
 *  Add   : i番目の更新を適用する関数 (void(int))
 *  Check : クエリidが条件を満たすか判定する関数 (bool(int))
 *
 * Parameters:
 *  q : クエリ数
 *  k : 更新数
 *  reset : データ構造の初期化
 *  add : i番目更新を適用
 *  check : クエリ判定
 *
 * Return:
 *  各クエリが条件を満たす最小の更新 index
 *  (存在しない場合は k)
 *
 * Notes:
 *  reset() は各イテレーションで呼ばれる
 *
 * Example:
 *  ```cpp
 *  DualSegtree seg(n);
 *
 *  auto reset = [&]() {
 *      seg = DualSegtree(n);
 *  };
 *
 *  auto add = [&](int i) {
 *      seg.apply(l[i], r[i] + 1, h[i]);
 *  };
 *
 *  auto check = [&](int id) {
 *      return seg.get(x[id]) >= y[id];
 *  };
 *
 *  auto ans = parallel_binary_search(q, k, reset, add, check);
 *  ```
 *
 * Verified:
 * https://atcoder.jp/contests/past20-open/submissions/74142387
 */
namespace kwm_t::algorithm::search {

template<class Reset, class Add, class Check>
std::vector<int> parallel_binary_search(
	int q,
	int k,
	Reset reset,
	Add add,
	Check check
) {
	std::vector<int> ok(q, k);
	std::vector<int> ng(q, -1);

	while (true) {
		bool fin = true;

		std::vector<std::vector<int>> mid(k);

		for (int i = 0; i < q; ++i) {
			if (std::abs(ok[i] - ng[i]) > 1) {
				fin = false;
				int m = (ok[i] + ng[i]) / 2;
				mid[m].push_back(i);
			}
		}

		if (fin) break;

		reset();

		for (int i = 0; i < k; ++i) {
			add(i);

			for (int id : mid[i]) {
				if (check(id)) ok[id] = i;
				else ng[id] = i;
			}
		}
	}

	return ok;
}

} // namespace kwm_t::algorithm::search

#endif // KWM_T_ALGORITHM_SEARCH_PARALLEL_BINARY_SEARCH_HPP