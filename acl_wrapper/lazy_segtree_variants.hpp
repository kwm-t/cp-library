#ifndef KWM_T_ACLWRAPPER_LAZY_SEGTREE_VARIANTS_HPP
#define KWM_T_ACLWRAPPER_LAZY_SEGTREE_VARIANTS_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include <optional>
#include <atcoder/lazysegtree>

/**
 * @brief LazySegTree の典型パターン集
 *
 * 提供:
 *   - Range Add / Min, Max, Sum
 *   - Range Update / Min, Max, Sum
 *   - Arithmetic Progression Add / Sum
 *   - Min/Max with Count
 *   - Affine Transform (template)
 *
 * 注意:
 *   - INF は long long 前提
 *   - size は init() で必ず 1 を入れること
 * verified:
 *   - https://atcoder.jp/contests/practice2/submissions/74233369
 */

namespace kwm_t::aclwrapper {

// ================= Range Add / Min =================
namespace RangeAddRangeMin {
using S = long long;
using F = long long;
static constexpr long long INF = (long long)4e18;

S op(S a, S b) { return std::min(a, b); }
S e() { return INF; }
S mapping(F f, S x) { return f + x; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

// ================= Range Add / Max =================
namespace RangeAddRangeMax {
using S = long long;
using F = long long;
static constexpr long long INF = (long long)4e18;

S op(S a, S b) { return std::max(a, b); }
S e() { return -INF; }
S mapping(F f, S x) { return f + x; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

// ================= Range Add / Sum =================
namespace RangeAddRangeSum {
struct S {
	long long value;
	int size;
};
using F = long long;

S op(S a, S b) { return { a.value + b.value, a.size + b.size }; }
S e() { return { 0, 0 }; }
S mapping(F f, S x) { return { x.value + f * x.size, x.size }; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
std::vector<S> init(int n, long long val = 0) { return std::vector<S>(n, { val, 1 }); }
}

// ================= Range Update / Min =================
namespace RangeUpdateRangeMin {
using S = long long;
using F = long long;
static constexpr long long INF = (long long)4e18;
static constexpr long long ID = (long long)4e18;

S op(S a, S b) { return std::min(a, b); }
S e() { return INF; }
S mapping(F f, S x) { return (f == ID ? x : f); }
F composition(F f, F g) { return (f == ID ? g : f); }
F id() { return ID; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

// ================= Range Update / Max =================
namespace RangeUpdateRangeMax {
using S = long long;
using F = long long;
static constexpr long long INF = (long long)4e18;
static constexpr long long ID = (long long)4e18;

S op(S a, S b) { return std::max(a, b); }
S e() { return -INF; }
S mapping(F f, S x) { return (f == ID ? x : f); }
F composition(F f, F g) { return (f == ID ? g : f); }
F id() { return ID; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

// ================= Range Update / Sum =================
namespace RangeUpdateRangeSum {
struct S {
	long long value;
	int size;
};
using F = long long;
static constexpr long long ID = (long long)4e18;

S op(S a, S b) { return { a.value + b.value, a.size + b.size }; }
S e() { return { 0, 0 }; }
S mapping(F f, S x) {
	if (f == ID) return x;
	x.value = f * x.size;
	return x;
}
F composition(F f, F g) { return (f == ID ? g : f); }
F id() { return ID; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
std::vector<S> init(int n, long long val = 0) { return std::vector<S>(n, { val, 1 }); }
}

// ================= Arithmetic Progression =================
namespace AddArithmeticProgression {
struct S { long long val_sum, index_sum, len; };
struct F { long long a, b; };

S op(S x, S y) { return { x.val_sum + y.val_sum, x.index_sum + y.index_sum, x.len + y.len }; }
S e() { return { 0, 0, 0 }; }
S mapping(F f, S x) { return { x.val_sum + f.a * x.index_sum + f.b * x.len, x.index_sum, x.len }; }
F composition(F f, F g) { return { f.a + g.a, f.b + g.b }; }
F id() { return { 0, 0 }; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

// ================= Max + Count =================
namespace RangeAddRangeMaxCount {
using S = std::pair<long long, int>;
using F = long long;
static constexpr long long INF = (long long)4e18;

S op(S a, S b) {
	if (a.first > b.first) return a;
	if (a.first < b.first) return b;
	return { a.first, a.second + b.second };
}
S e() { return { -INF, 0 }; }
S mapping(F f, S x) { return { x.first + f, x.second }; }
F composition(F f, F g) { return f + g; }
F id() { return 0; };

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
std::vector<S> init(int n, long long val = 0) { return std::vector<S>(n, { val, 1 }); }
}

// ================= Min + Count =================
namespace RangeAddRangeMinCount {
using S = std::pair<long long, int>;
using F = long long;
static constexpr long long INF = (long long)4e18;

S op(S a, S b) {
	if (a.first < b.first) return a;
	if (a.first > b.first) return b;
	return { a.first, a.second + b.second };
}
S e() { return { INF, 0 }; }
S mapping(F f, S x) { return { x.first + f, x.second }; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
std::vector<S> init(int n, long long val = 0) { return std::vector<S>(n, { val, 1 }); }
}

// ================= Affine (Template) =================
template<class mint>
struct RangeAffineRangeSum {
	struct S { mint sum; int size; };
	struct F { mint b, c; };
	static S op(S l, S r) { return { l.sum + r.sum, l.size + r.size }; }
	static S e() { return { mint(0), 0 }; }
	static S mapping(F f, S x) { return { f.b * x.sum + f.c * x.size, x.size }; }
	static F composition(F f, F g) { return { f.b * g.b, f.b * g.c + f.c }; }
	static F id() { return { mint(1), mint(0) }; }

	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
	static std::vector<S> init(int n) { return std::vector<S>(n, { mint(0), 1 }); }
};

} // namespace kwm_t::aclwrapper

#endif // KWM_T_ACLWRAPPER_LAZY_SEGTREE_VARIANTS_HPP