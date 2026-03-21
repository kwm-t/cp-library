#ifndef KWM_T_ALGORITHM_DP_SUBSET_PARTITION_HPP
#define KWM_T_ALGORITHM_DP_SUBSET_PARTITION_HPP

#include <vector>

/**
 * @brief 部分集合分割 DP (subset partition DP) を列挙するヘルパー
 *
 * 各 mask に対して、その部分集合 sub を列挙し
 * merge(mask, sub, dp) を呼び出す。
 *
 * 典型用途:
 *   dp[mask] = min(dp[mask], dp[sub] + dp[mask ^ sub]);
 *
 * 計算量:
 *   O(3^n)
 *
 * @tparam T      dp の要素型
 * @tparam Merge  void merge(int mask, int sub, std::vector<T>& dp)
 *
 * @param dp           サイズ 2^n の DP 配列
 * @param merge        各 (mask, sub) に対して呼ばれる処理
 * @param allow_full   true の場合 sub = mask を許す
 * @param allow_empty  true の場合 sub = 0 を許す
 *
 * 列挙される sub は mask の部分集合のみ。
 * 使用例
 * https://atcoder.jp/contests/awc0023/submissions/74011888
 * dpテーブルの初期化は呼び出し元で行うこと
 */
namespace kwm_t::algorithm::dp {
template<class T, class Merge>
void subset_partition_dp(
	std::vector<T>& dp,
	Merge merge,
	bool allow_full = true,
	bool allow_empty = false
) {
	int N = dp.size();
	for (int mask = 1; mask < N; mask++) {
		int sub = allow_full ? mask : (mask - 1) & mask;
		for (;; sub = (sub - 1) & mask) {
			if (sub == 0 && !allow_empty) break;
			merge(mask, sub, dp);
			if (sub == 0) break;
		}
	}
}
}

#endif // KWM_T_ALGORITHM_DP_SUBSET_PARTITION_HPP