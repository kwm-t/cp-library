// 狭義凸関数でしか使えないことに注意
template <class T, class U>
std::pair<T, U> ternarysearch(T l, T r, const std::function<U(T)> &f) {
	// 下に凸(最小値)
	while (2 < abs(r - l)) {
		T ml = (l * 2 + r) / 3;
		T mr = (l + r * 2) / 3;
		if (f(ml) < f(mr)) r = mr;
		else l = ml;
	}
	T ret = l;
	U value = f(l);
	for (T i = l + 1; i <= r; ++i) {
		auto tmp = f(i);
		if (tmp < value) {
			ret = i;
			value = tmp;
		}
	}
	return { ret,value };
}

template <class T, class U>
std::pair<T, U> ternarysearchDouble(T l, T r, const std::function<U(T)> &f, int counter = 100) {
	// 下に凸(最小値)
	while (counter--) {
		T ml = (l * 2 + r) / 3;
		T mr = (l + r * 2) / 3;
		if (f(ml) < f(mr)) r = mr;
		else l = ml;
	}
	T ret = l;
	U value = f(l);
	return { ret,value };
}
	