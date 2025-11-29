namespace myranges {
	template<class It>
	It safe_next(It it, long long n, It bound) {
		while (n-- > 0 && it != bound) ++it;
		return it;
	}
#if __cpp_lib_ranges
	using std::ranges::next;
#else
	template<class It>
	It next(It it, long long n, It bound) { return safe_next(it, n, bound); }
#endif
}

template <class RandomAccessIterator, class Compare> constexpr void exponential_merge_sort(
	RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	auto len = (last - first);
	if (len <= 1) { return; }
	auto mid = first + len / 2;
	exponential_merge_sort(first, mid, comp);
	exponential_merge_sort(mid, last, comp);
	auto a = vector(first, mid);
	auto b = vector(mid, last);
	auto ai = a.begin();
	auto bi = b.begin();
	bool side = false;
	while (ai != a.end() || bi != b.end()) {
		if (ai == a.end()) {
			while (bi != b.end()) {
				(*first) = (*bi);
				first++; bi++;
			}
			break;
		}
		else if (bi == b.end()) {
			while (ai != a.end()) {
				(*first) = (*ai);
				first++; ai++;
			}
			break;
		}
		side = (!side);
		if (side) {
			// kick a.begin();
			long long d = 1;
			while (true) {
				auto it = myranges::next(bi, d - 1, b.end());
				if (it == b.end()) { break; }
				if (comp((*it), (*ai))) { d <<= 1; }
				else { break; }
			}
			d >>= 1;
			long long c = d;
			while (d >= 2) {
				d >>= 1;
				auto it = myranges::next(bi, c + d - 1, b.end());
				if (it == b.end()) { continue; }
				if (comp((*it), (*ai))) { c += d; }
			}
			while (c--) {
				(*first) = (*bi);
				first++; bi++;
			}
			(*first) = (*ai);
			first++; ai++;
		}
		else {
			// kick b.begin();
			long long d = 1;
			while (true) {
				auto it = myranges::next(ai, d - 1, a.end());
				if (it == a.end()) { break; }
				if (comp((*it), (*bi))) { d <<= 1; }
				else { break; }
			}
			d >>= 1;
			long long c = d;
			while (d >= 2) {
				d >>= 1;
				auto it = myranges::next(ai, c + d - 1, a.end());
				if (it == a.end()) { continue; }
				if (comp((*it), (*bi))) { c += d; }
			}
			while (c--) {
				(*first) = (*ai);
				first++; ai++;
			}
			(*first) = (*bi);
			first++; bi++;
		}
	}
}