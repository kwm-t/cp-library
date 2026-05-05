#ifndef KWM_T_ALGORITHM_DP_SUBSET_SUM_FAST_HPP
#define KWM_T_ALGORITHM_DP_SUBSET_SUM_FAST_HPP

#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>

namespace kwm_t::algorithm::dp {

/**
 * @brief 高速 subset sum (多重集合対応・bitset圧縮)
 *
 * p の部分集合で作れる和をすべて列挙する
 *
 * @tparam MAX_LEN bitset の最大サイズ
 *
 * @param p 非負整数列
 * @param sum 合計値（-1なら自動計算）
 *
 * @return std::vector<bool>
 *   res[s] = 和 s が作れるか
 *
 * @note
 * - 計算量: O(S log S / word_size) 程度
 * - 値ごとに個数圧縮（binary splitting風）している
 * - p の要素は 0 以上である必要あり
 *
 * @warning
 * - sum <= MAX_LEN を満たす必要あり
 *
 * verified:
 * https://atcoder.jp/contests/ndpc/submissions/75531614
 */
template <int MAX_LEN = (1 << 22)>
std::vector<bool> subset_sum_fast(std::vector<int> p, int sum = -1) {
	if (sum == -1) {
		sum = 0;
		for (int x : p) {
			assert(x >= 0);
			sum += x;
		}
	}

	assert(sum <= MAX_LEN);

	// sum が大きすぎる場合は bitset サイズを拡張して再帰
	if (sum >= MAX_LEN) {
		return subset_sum_fast<std::min(MAX_LEN * 2, MAX_LEN)>(p, sum);
	}

	for (int x : p) assert(x >= 0);

	std::bitset<MAX_LEN> dp;
	std::vector<int> cnt(sum + 1);

	for (int x : p) cnt[x]++;

	dp[0] = 1;

	for (int i = 1; i <= sum; i++) {
		if (cnt[i] == 0) continue;

		// 1個使う
		dp |= (dp << i);
		cnt[i]--;

		// 偶数個ならもう1回
		if (cnt[i] % 2 == 1) {
			dp |= (dp << i);
			cnt[i]--;
		}

		// まとめて次へ（多重集合圧縮）
		if (i * 2 <= sum && cnt[i] > 0) {
			cnt[i * 2] += cnt[i] / 2;
		}
	}

	std::vector<bool> res(sum + 1);
	for (int i = 0; i <= sum; i++) {
		res[i] = dp[i];
	}

	return res;
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_SUBSET_SUM_FAST_HPP