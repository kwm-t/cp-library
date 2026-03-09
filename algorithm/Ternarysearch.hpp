// 整数三分探索
template <class T, class F, class Compare = std::less<>>
auto ternary_search(T l, T r, F f, Compare comp = Compare()) {
	using U = decltype(f(l));
	while (r - l > 2) {
		T ml = l + (r - l) / 3;
		T mr = r - (r - l) / 3;

		if (comp(f(ml), f(mr))) r = mr;
		else l = ml;
	}
	T best = l;
	U val = f(l);
	for (T i = l + 1; i <= r; ++i) {
		U v = f(i);
		if (comp(v, val)) {
			best = i;
			val = v;
		}
	}
	return std::pair{ best, val };
}


// 実数三分探索
template <class T, class F, class Compare = std::less<>>
auto ternary_search_double(T l, T r, F f, int counter = 100, Compare comp = Compare()) {
	using U = decltype(f(l));
	while (counter--) {
		T ml = l + (r - l) / 3;
		T mr = r - (r - l) / 3;
		
		if (comp(f(ml), f(mr))) r = mr;
		else l = ml;
	}
	return std::pair{ l, f(l) };
}

// min
// auto res = ternary_search(0LL, 100LL, f);
// max
// auto res = ternary_search(0LL, 100LL, f, std::greater<>());