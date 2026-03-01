namespace sorted_vec {
	// x と等しい要素の個数
	template <class T>
	std::size_t count_equal(const std::vector<T>& v, const T& x) {
		auto l = std::lower_bound(v.begin(), v.end(), x);
		auto r = std::upper_bound(v.begin(), v.end(), x);
		return std::distance(l, r);
	}

	// x 未満の個数
	template <class T>
	std::size_t count_less(const std::vector<T>& v, const T& x) {
		return std::distance(v.begin(), std::lower_bound(v.begin(), v.end(), x));
	}

	// x 以下の個数
	template <class T>
	std::size_t count_leq(const std::vector<T>& v, const T& x) {
		return std::distance(v.begin(), std::upper_bound(v.begin(), v.end(), x));
	}

	// x 以上の個数
	template <class T>
	std::size_t count_geq(const std::vector<T>& v, const T& x) {
		return std::distance(std::lower_bound(v.begin(), v.end(), x), v.end());
	}

	// x より大きい個数
	template <class T>
	std::size_t count_greater(const std::vector<T>& v, const T& x) {
		return std::distance(std::upper_bound(v.begin(), v.end(), x), v.end());
	}

	// [l, r) に含まれる個数
	template <class T>
	std::size_t count_range(const std::vector<T>& v, const T& l, const T& r) {
		auto L = std::lower_bound(v.begin(), v.end(), l);
		auto R = std::lower_bound(v.begin(), v.end(), r);
		return std::distance(L, R);
	}
}
