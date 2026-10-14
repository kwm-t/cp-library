/**
 * @file inversion_number.hpp
 * @brief 転倒数
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/inversion_number.hpp
 */
#ifndef KWM_T_ALGORITHM_INVERSION_NUMBER_HPP
#define KWM_T_ALGORITHM_INVERSION_NUMBER_HPP

#include <vector>
#include <algorithm>

#include <atcoder/segtree>

namespace kwm_t::algorithm {

namespace internal {

inline long long inversion_number_op(long long l, long long r) { return l + r; }
inline long long inversion_number_e() { return 0LL; }

} // namespace internal

/**
 * @brief 配列の転倒数を求める
 *
 * 配列 a の転倒数
 *   #{ (i, j) | i < j かつ a[i] > a[j] }
 * を返す。
 *
 * 順列でなくても使用可能。
 * 重複要素にも対応。
 *
 * 内部で座標圧縮を行う。
 *
 * 計算量:
 *   O(N log N)
 *
 * @tparam T
 *   比較可能 (< が定義されている) 型
 *
 * @param a
 *   入力配列
 *
 * @return long long
 *   転倒数
 *
 * 制約 / 注意:
 * - T は std::sort / std::lower_bound で比較可能であること
 * - 転倒数は long long に収まること
 *
 * 使用例:
 * @code
 * std::vector<int> a = {3, 1, 4, 1, 5};
 * long long inv = kwm_t::algorithm::inversion_number(a);
 * @endcode
 *
 * verified:
 * - https://atcoder.jp/contests/awc0064/submissions/75573019
 *
 */
template<class T>
long long inversion_number(const std::vector<T>& a) {
	std::vector<T> xs = a;

	std::sort(xs.begin(), xs.end());
	xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

	const int m = (int)xs.size();

	atcoder::segtree<long long, internal::inversion_number_op, internal::inversion_number_e> seg(m);

	long long res = 0;

	for (const T& x : a) {
		const int id = (int)(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin());
		res += seg.prod(id + 1, m);
		seg.set(id, seg.get(id) + 1);
	}

	return res;
}

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_INVERSION_NUMBER_HPP