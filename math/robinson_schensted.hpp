/**
 * @file robinson_schensted.hpp
 * @brief Robinson-Schensted 対応
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/robinson_schensted.hpp
 */
#ifndef KWM_T_MATH_ROBINSON_SCHENSTED_HPP
#define KWM_T_MATH_ROBINSON_SCHENSTED_HPP

#include <algorithm>
#include <utility>
#include <vector>

namespace kwm_t::math {

/**
 * @brief Robinson-Schensted 対応により、配列を P-tableau と Q-tableau に変換する。
 *
 * P-tableau は値を保持する半標準 Young tableau、
 * Q-tableau は各要素が元の配列の何番目から挿入されたかを保持する。
 *
 * is_strong が true の場合は lower_bound を用いる。
 * is_strong が false の場合は upper_bound を用いる。
 *
 * 計算量:
 *   O(N * K * log N)
 *
 * @tparam T 要素の型。
 *
 * @param a 入力配列。
 * @param k tableau の最大行数。
 * @param is_strong 挿入時に lower_bound を使用するか。
 *
 * @return
 *   first  : P-tableau。
 *   second : Q-tableau。
 *
 * 制約 / 注意:
 *   - k <= 0 の場合は空の tableau を返す。
 *   - Q-tableau の値は 0-indexed の入力位置。
 *   - is_strong == true の場合は lower_bound、
 *     is_strong == false の場合は upper_bound を使用する。
 *
 * verified:
 *   - https://yukicoder.me/submissions/1190590
 *   - https://atcoder.jp/contests/abc468/submissions/77872609
 *   - 上記は嘘解法のためWAです
 */
template <class T>
std::pair<std::vector<std::vector<T>>, std::vector<std::vector<int>>>
robinson_schensted(
	const std::vector<T>& a,
	int k,
	bool is_strong = true
) {
	if (k <= 0) return {};

	std::vector<std::vector<T>> p;
	std::vector<std::vector<int>> q;

	p.reserve(k);
	q.reserve(k);

	for (int i = 0; i < static_cast<int>(a.size()); ++i) {
		T value = a[i];

		for (int row_index = 0; row_index < k; ++row_index) {
			if (row_index == static_cast<int>(p.size())) {
				p.emplace_back();
				q.emplace_back();
			}

			auto& p_row = p[row_index];

			auto it = is_strong
				? std::lower_bound(p_row.begin(), p_row.end(), value)
				: std::upper_bound(p_row.begin(), p_row.end(), value);

			if (it == p_row.end()) {
				p_row.push_back(value);
				q[row_index].push_back(i);
				break;
			}

			std::swap(value, *it);
		}
	}

	return {std::move(p), std::move(q)};
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_ROBINSON_SCHENSTED_HPP