/**
 * @file segtree_variants.hpp
 * @brief Segment Tree の各種バリエーション
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/acl_wrapper/segtree_variants.hpp
 */
#ifndef KWM_T_ACLWRAPPER_SEGTREE_VARIANTS_HPP
#define KWM_T_ACLWRAPPER_SEGTREE_VARIANTS_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include <optional>
#include <atcoder/segtree>

/**
 * @brief SegTree の典型パターン集
 *
 * 提供:
 */

namespace kwm_t::aclwrapper {

// ================= Maximum Subarray Sum =================
namespace MaximumSubarraySum {

struct S {
	long long sum;   // 区間和
	long long pref;  // 最大 prefix sum
	long long suff;  // 最大 suffix sum
	long long ans;   // 最大部分列和
};

S op(S l, S r) {
	return {
		l.sum + r.sum,
		std::max(l.pref, l.sum + r.pref),
		std::max(r.suff, r.sum + l.suff),
		std::max({l.ans, r.ans, l.suff + r.pref})
	};
}

S e() {
	static constexpr long long INF = (long long)4e18;
	return {
		0,
		-INF,
		-INF,
		-INF
	};
}

}; // namespace kwm_t::aclwrapper

}

#endif // KWM_T_ACLWRAPPER_SEGTREE_VARIANTS_HPP