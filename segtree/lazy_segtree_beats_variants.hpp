/**
 * @file lazy_segtree_beats_variants.hpp
 * @brief Lazy Segment Tree Beats の各種バリエーション
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/segtree/lazy_segtree_beats_variants.hpp
 */
#ifndef KWM_T_SEGTREE_BEATS_VARIANTS_HPP
#define KWM_T_SEGTREE_BEATS_VARIANTS_HPP

#include <vector>
#include <algorithm>
#include "lazy_segtree_beats.hpp"

/**
 * @brief Segment Tree Beats 用の典型パターン
 *
 * 提供:
 *   - Range Chmin / Sum
 *   - Range Chmax / Sum
 *
 * 計算量:
 *   - amortized O(log N)
 *
 * 注意:
 *   - mapping は失敗時に {x, true} を返すこと
 *   - 値は long long 前提
 *
 * verified:
 *   -
 */

namespace kwm_t::segtree::beats {

// ================= Range Chmin / Sum =================
namespace RangeChminRangeSum {

struct S {
	long long sum;
	long long max1, max2;
	int maxc;
	int size;
};

using F = long long;

S op(S a, S b) {
	S res;
	res.sum = a.sum + b.sum;
	res.size = a.size + b.size;

	if (a.max1 > b.max1) {
		res.max1 = a.max1;
		res.maxc = a.maxc;
		res.max2 = std::max(a.max2, b.max1);
	}
	else if (a.max1 < b.max1) {
		res.max1 = b.max1;
		res.maxc = b.maxc;
		res.max2 = std::max(a.max1, b.max2);
	}
	else {
		res.max1 = a.max1;
		res.maxc = a.maxc + b.maxc;
		res.max2 = std::max(a.max2, b.max2);
	}
	return res;
}

S e() {
	return { 0, -(long long)4e18, -(long long)4e18, 0, 0 };
}

std::pair<S, bool> mapping(F f, S x) {
	if (x.max1 <= f) return { x, false };
	if (x.max2 < f) {
		long long diff = x.max1 - f;
		x.sum -= diff * x.maxc;
		x.max1 = f;
		return { x, false };
	}
	return { x, true }; // fail
}

F composition(F f, F g) {
	return { std::min(f, g) };
}

F id() {
	return { (long long)4e18 };
}

using segtree = lazy_segtree<S, op, e, F, mapping, composition, id>;

template <class T>
segtree make_segtree(const std::vector<T>& v) {
	int n = v.size();
	std::vector<S> init(n);
	for (int i = 0; i < n; i++) {
		init[i] = { v[i], v[i], -(long long)4e18, 1, 1 };
	}
	return segtree(init);
}
}

// ================= Range Chmax / Sum =================
namespace RangeChmaxRangeSum {

struct S {
	long long sum;
	long long min1, min2;
	int minc;
	int size;
};

using F = long long;

S op(S a, S b) {
	S res;
	res.sum = a.sum + b.sum;
	res.size = a.size + b.size;

	if (a.min1 < b.min1) {
		res.min1 = a.min1;
		res.minc = a.minc;
		res.min2 = std::min(a.min2, b.min1);
	}
	else if (a.min1 > b.min1) {
		res.min1 = b.min1;
		res.minc = b.minc;
		res.min2 = std::min(a.min1, b.min2);
	}
	else {
		res.min1 = a.min1;
		res.minc = a.minc + b.minc;
		res.min2 = std::min(a.min2, b.min2);
	}
	return res;
}

S e() {
	return { 0, (long long)4e18, (long long)4e18, 0, 0 };
}

std::pair<S, bool> mapping(F f, S x) {
	if (x.min1 >= f) return { x, false };
	if (x.min2 > f) {
		long long diff = f - x.min1;
		x.sum += diff * x.minc;
		x.min1 = f;
		return { x, false };
	}
	return { x, true };
}

F composition(F f, F g) {
	return { std::max(f, g) };
}

F id() {
	return -(long long)4e18;
}

using segtree = lazy_segtree<S, op, e, F, mapping, composition, id>;

template <class T>
segtree make_segtree(const std::vector<T>& v) {
	int n = v.size();
	std::vector<S> init(n);
	for (int i = 0; i < n; i++) {
		init[i] = { v[i], v[i], (long long)4e18, 1, 1 };
	}
	return segtree(init);
}

}

} // namespace kwm_t::segtree::beats

#endif // KWM_T_SEGTREE_BEATS_VARIANTS_HPP