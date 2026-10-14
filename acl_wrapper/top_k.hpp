/**
 * @file top_k.hpp
 * @brief 上位 K 要素を管理するデータ構造
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/acl_wrapper/top_k.hpp
 */
#ifndef KWM_T_ACLWRAPPER_TOP_K_HPP
#define KWM_T_ACLWRAPPER_TOP_K_HPP

#include <algorithm>
#include <array>
#include <functional>

namespace kwm_t::aclwrapper {

/**
 * @brief Compare の順に上位 K 個の要素を保持するモノイド
 *
 * 各要素は Compare に従ってソートされた状態で保持される。
 * merge() では 2 つの TopK をマージし、先頭 K 個のみを残す。
 *
 * 区間の要素数が K 未満の場合は、実際に存在する要素のみを保持する。
 *
 * 典型用途:
 * - セグメント木で区間内の上位 K 個を求める
 * - 区間内の最大値・2 番目に大きい値などを求める
 *
 * 計算量:
 * - merge: O(K)
 * - 保持する要素数: O(K)
 *
 * @tparam T 要素の型
 * @tparam K 保持する要素数
 * @tparam Compare 要素の優先順位を決める比較関数
 *
 * @param x 1 要素からなる TopK を構築する。
 *
 * 制約 / 注意:
 * - K >= 1
 * - Compare は strict weak ordering を満たすこと。
 * - v[0, size) のみが有効な要素であり、v[size, K) は未使用。
 *
 * 使用例:
 * @code
 * using Top2 = kwm_t::aclwrapper::TopK<int, 2, std::greater<int>>;
 *
 * atcoder::segtree<Top2, Top2::op, Top2::e> seg(n);
 * @endcode
 *
 * verified:
 * https://atcoder.jp/contests/awc0171/submissions/79800572
 */
template <typename T, int K, typename Compare = std::less<T>>
struct TopK {
	static_assert(K >= 1);

	std::array<T, K> v;
	int size = 0;

	TopK() = default;

	TopK(const T& x) : v{x}, size(1) {}

	/**
	 * @brief 2 つの TopK をマージする。
	 *
	 * それぞれ Compare に従ってソート済みなので、
	 * 2-way merge によって先頭 K 個だけを取得する。
	 */
	TopK merge(const TopK& other) const {
		TopK res;

		Compare comp;
		int i = 0;
		int j = 0;

		while (res.size < K && (i < size || j < other.size)) {
			if (j == other.size ||
				(i < size && comp(v[i], other.v[j]))) {
				res.v[res.size++] = v[i++];
			}
			else {
				res.v[res.size++] = other.v[j++];
			}
		}

		return res;
	}

	/**
	 * @brief モノイドの二項演算。
	 */
	static TopK op(TopK l, TopK r) {
		return l.merge(r);
	}

	/**
	 * @brief モノイドの単位元。
	 */
	static TopK e() {
		return {};
	}
};

} // namespace kwm_t::aclwrapper

#endif // KWM_T_ACLWRAPPER_TOP_K_HPP