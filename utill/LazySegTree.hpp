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
}
namespace RangeAddRangeSum {
	// 区間加算・区間最大値取得
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
}
namespace RangeUpdateRangeSum {
	// 区間変更・区間和取得
	struct S {
		long long value;
		int size;
	};
	using F = long long;
	const F ID = 8e18;
	S op(S a, S b) { return { a.value + b.value, a.size + b.size }; }
	S e() { return { 0, 0 }; }
	S mapping(F f, S x) {
		if (f != ID) x.value = f * x.size;
		return x;
	}
	F composition(F f, F g) { return (f == ID ? g : f); }
	F id() { return ID; }
}