#ifndef KWM_T_ALGORITHM_DP_BIT_DP_HPP
#define KWM_T_ALGORITHM_DP_BIT_DP_HPP

#include <vector>
#include <algorithm>

/**
 * @brief bitDP（部分集合DP）の遷移
 *
 * dp[mask][last] から未訪問の nxt へ遷移する
 *
 * 典型用途:
 *   - 巡回セールスマン問題（TSP）
 *   - 頂点部分集合DP
 *
 * 計算量:
 *   O(2^k * k^2)
 *
 * @tparam T コスト型
 * @tparam F cost関数 (T f(int last, int nxt))
 *
 * @param dp dpテーブル（事前初期化済み）
 * @param k  要素数
 * @param cost 遷移コスト関数
 * @param INF 無限大（到達不能判定用）
 *
 * 制約 / 注意:
 *   - dp[mask][i] は「mask に i が含まれる状態」のみ有効
 *   - 初期状態は外部で設定すること
 *
 * 使用例:
 *   vector dp(1<<k, vector<ll>(k, INF));
 *   dp[1<<i][i] = 初期値;
 *
 *   auto cost = [&](int i, int j) {
 *       return d[t[i]][t[j]];
 *   };
 *
 *   bit_dp_transition(dp, k, cost, INF);
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0029/submissions/74234236
 */
namespace kwm_t::algorithm::dp {

template <class T, class F>
void bit_dp_transition(
	std::vector<std::vector<T>> &dp,
	int k,
	F cost,
	T INF
) {
	for (int mask = 0; mask < (1 << k); ++mask) {
		for (int last = 0; last < k; ++last) {
			if (!(mask & (1 << last))) continue;
			if (dp[mask][last] == INF) continue;

			for (int nxt = 0; nxt < k; ++nxt) {
				if (mask & (1 << nxt)) continue;

				int nmask = mask | (1 << nxt);
				dp[nmask][nxt] = std::min(
					dp[nmask][nxt],
					dp[mask][last] + cost(last, nxt)
				);
			}
		}
	}
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_BIT_DP_HPP