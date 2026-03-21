#ifndef KWM_T_ACLWRAPPER_SLOPE_HELPER_HPP
#define KWM_T_ACLWRAPPER_SLOPE_HELPER_HPP

#include <vector>
#include <utility>

/**
 * @brief min_cost_slope の結果を配列に展開するヘルパー
 *
 * slope: (flow, cost) の折れ線（ACL mcf_graph::slope の返り値）
 * 戻り値: ret[i] = 流量 i のときの最小コスト
 *
 * 典型用途:
 *   - 各流量に対するコストをDPや全探索で使いたいとき
 *   - ラグランジュ緩和（cost - λ * flow）への変換
 *
 * 計算量:
 *   O(F) (F = 最大流量)
 *
 * @tparam Cap   流量型 (int, long long など)
 * @tparam Cost  コスト型 (int, long long など)
 *
 * @param slope  (flow, cost) の折れ線
 *
 * 制約 / 注意:
 *   - slope[0] == (0, 0)
 *   - flow は単調増加
 *   - 同一直線上に3点はない（ACL仕様）
 *   - 傾きは整数であることが保証されている
 *
 * 使用例:
 *   auto v = graph.slope(s, t);
 *   auto cost = slope_helper(v);
 *
 * verified:
 *   https://atcoder.jp/contests/abc407/submissions/74186930
 *   https://atcoder.jp/contests/abc407/submissions/74187074
 */
namespace kwm_t::aclwrapper {

template <class Cap, class Cost>
std::vector<Cost> slope_helper(const std::vector<std::pair<Cap, Cost>>& slope) {
	std::vector<Cost> ret;
	if (slope.empty()) return ret;

	ret.reserve(slope.back().first + 1);
	ret.push_back(Cost(0));

	for (int i = 1; i < (int)slope.size(); ++i) {
		Cap k = slope[i].first;
		Cost v = slope[i].second;

		Cap pk = slope[i - 1].first;
		Cost pv = slope[i - 1].second;

		Cost slope_val = (v - pv) / (k - pk);

		for (Cap j = pk + 1; j <= k; ++j) {
			ret.push_back(pv + slope_val * (j - pk));
		}
	}

	return ret;
}

/**
 * @brief cost[i] - big * i を返す版（ラグランジュ緩和用）
 */
template <class Cap, class Cost>
std::vector<Cost> slope_helper(
	const std::vector<std::pair<Cap, Cost>>& slope,
	Cost big
) {
	auto ret = slope_helper<Cap, Cost>(slope);
	for (int i = 0; i < (int)ret.size(); ++i) {
		ret[i] -= big * i;
	}
	return ret;
}

/**
 * @brief min_x (g(x) - big * x) を高速に求める
 *
 * slope: (flow, cost) の折れ線（ACL slope）
 *
 * 計算量:
 *   O(K) (K = 折れ線の点数)
 */
template <class Cap, class Cost>
Cost slope_min_value(
	const std::vector<std::pair<Cap, Cost>>& slope,
	Cost big
) {
	Cost ans = std::numeric_limits<Cost>::max();

	for (const auto&[x, gx] : slope) {
		Cost val = gx - big * x;
		if (val < ans) ans = val;
	}

	return ans;
}

} // namespace kwm_t::aclwrapper

#endif // KWM_T_ACLWRAPPER_SLOPE_HELPER_HPP