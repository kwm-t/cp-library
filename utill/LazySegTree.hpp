namespace RangeAddRangeMin{
	// 区間加算・区間最小値取得
	using S = long long;
	using F = long long;
	const S INF = 8e18;
	S op(S a, S b) { return std::min(a, b); }
	S e() { return INF; }
	S mapping(F f, S x) { return f + x; }
	F composition(F f, F g) { return f + g; }
	F id() { return 0; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
};
namespace RangeAddRangeMax {
	// 区間加算・区間最大値取得
	using S = long long;
	using F = long long;
	const S INF = 8e18;
	S op(S a, S b) { return std::max(a, b); }
	S e() { return -INF; }
	S mapping(F f, S x) { return f + x; }
	F composition(F f, F g) { return f + g; }
	F id() { return 0; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}
namespace RangeAddRangeSum {
	// 区間加算・区間和取得
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
	std::vector<S>init(int size) { return std::vector<S>(size, { 0,1 }); }
}
namespace RangeUpdateRangeMin {
	// 区間変更・区間最小値取得
	using S = long long;
	using F = long long;
	const S INF = 8e18;
	const F ID = 8e18;
	S op(S a, S b) { return std::min(a, b); }
	S e() { return INF; }
	S mapping(F f, S x) { return (f == ID ? x : f); }
	F composition(F f, F g) { return (f == ID ? g : f); }
	F id() { return ID; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}
namespace RangeUpdateRangeMax {
	// 区間変更・区間最大値取得
	using S = long long;
	using F = long long;
	const S INF = 8e18;
	const F ID = 8e18;
	S op(S a, S b) { return std::max(a, b); }
	S e() { return -INF; }
	S mapping(F f, S x) { return (f == ID ? x : f); }
	F composition(F f, F g) { return (f == ID ? g : f); }
	F id() { return ID; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}
namespace RangeUpdateRangeSum {
	// 区間変更・区間和取得
	struct S {
		long long value;
		int size;
	};
	using F = optional<long long>;
	const F ID = nullopt;
	S op(S a, S b) { return { a.value + b.value, a.size + b.size }; }
	S e() { return { 0, 0 }; }
	S mapping(F f, S x) {
		if (f != ID) x.value = *f * x.size;
		return x;
	}
	F composition(F f, F g) { return (f == ID ? g : f); }
	F id() { return ID; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
	std::vector<S>init(int size) { return std::vector<S>(size, { 0,1 }); }
}
namespace AddArithmeticProgression {
	// 区間等差数列加算・区間和取得
	// init
	// seg.set(i,S{a[i],i,1});
	// update
	// [l,r)にai+bを加算
	// seg.apply(l,r,F{a,b});
	struct S { long long val_sum, index_sum, len; };
	struct F { long long a, b; };
	S op(S x, S y) { return S{ x.val_sum + y.val_sum,x.index_sum + y.index_sum,x.len + y.len }; }
	S e() { return S{ 0,0,0 }; }
	S mapping(F f, S x) { return S{ x.val_sum + f.a*x.index_sum + f.b*x.len,x.index_sum,x.len }; }
	F composition(F f, F g) { return F{ f.a + g.a, f.b + g.b }; }
	F id() { return F{ 0,0 }; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}
namespace RangeAddRangeMax {
	// 区間加算・区間最大値取得(個数付き)
	// {0,1}で初期化
	using S = std::pair<long long, int>;
	using F = long long;
	S op(S a, S b) {
		if (a.first > b.first)return a;
		else if (a.first < b.first)return b;
		return { a.first,a.second + b.second };
	}
	S e() { return { -8e18,0 }; }
	S mapping(F f, S x) { return { x.first + f,x.second }; }
	F composition(F f, F g) { return f + g; }
	F id() { return 0; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
	std::vector<S>init(int size) { return std::vector<S>(size, { 0,1 }); }
}
namespace RangeAddRangeMin {
	// 区間加算・区間最小値(個数付き)
	// {0,1}で初期化
	using S = std::pair<long long, int>;
	using F = long long;
	S op(S a, S b) {
		if (a.first < b.first)return a;
		else if (a.first > b.first)return b;
		return { a.first,a.second + b.second };
	}
	S e() { return { 8e18,0 }; }
	S mapping(F f, S x) { return { x.first + f,x.second }; }
	F composition(F f, F g) { return f + g; }
	F id() { return 0; }
	using lazy_segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
	std::vector<S>init(int size) { return std::vector<S>(size, { 0,1 }); }
}
