template<typename T, typename U>
U Ternarysearch(T l, T r, const std::function<U(T)> &f) {
	// 下に凸
	while (2 < abs(r - l)) {
		T ml = (l * 2 + r) / 3;
		T mr = (l + r * 2) / 3;
		if (f(ml) < f(mr)) r = mr;
		else l = ml;
	}
	T ret = l;
	U value = f(l);
	for (T i = l; i <= r; ++i) value = std::min(value, f(i));
	return value;
}