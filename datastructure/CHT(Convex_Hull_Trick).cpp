#include <deque>
template< typename T, bool isMin >
struct ConvexHullTrickAddMonotone {
	//https://ei1333.github.io/luzhiled/snippets/structure/convex-hull-trick-add-monotone.html
	std::deque<std::pair< T, T >> H;
	ConvexHullTrickAddMonotone() = default;
	bool empty() const { return H.empty(); }
	void clear() { H.clear(); }
	inline int sgn(T x) { return x == 0 ? 0 : (x < 0 ? -1 : 1); }
	using D = long double;
	inline bool check(const std::pair< T, T > &a, const std::pair< T, T > &b, const std::pair< T, T > &c) {
		if (b.second == a.second || c.second == b.second)
			return sgn(b.first - a.first) * sgn(c.second - b.second) >= sgn(c.first - b.first) * sgn(b.second - a.second);

		//return (b.F-a.F)*(c.S-b.S) >= (b.S-a.S)*(c.F-b.F);
		return
			D(b.first - a.first) * sgn(c.second - b.second) / D(abs(b.second - a.second)) >=
			D(c.first - b.first) * sgn(b.second - a.second) / D(abs(c.second - b.second));
	}

	void add(T a, T b) {
		if (!isMin) a *= -1, b *= -1;
		std::pair< T, T > line(a, b);
		if (empty()) {
			H.emplace_front(line);
			return;
		}
		if (H.front().first <= a) {
			if (H.front().first == a) {
				if (H.front().second <= b) return;
				H.pop_front();
			}
			while (H.size() >= 2 && check(line, H.front(), H[1])) H.pop_front();
			H.emplace_front(line);
		}
		else {
			assert(a <= H.back().first);
			if (H.back().first == a) {
				if (H.back().second <= b) return;
				H.pop_back();
			}
			while (H.size() >= 2 && check(H[H.size() - 2], H.back(), line)) H.pop_back();
			H.emplace_back(line);
		}
	}

	inline T get_y(const std::pair< T, T > &a, const T &x) {
		return a.first * x + a.second;
	}

	T query(T x) {
		assert(!empty());
		int l = -1, r = H.size() - 1;
		while (l + 1 < r) {
			int m = (l + r) >> 1;
			if (get_y(H[m], x) >= get_y(H[m + 1], x)) l = m;
			else r = m;
		}
		if (isMin) return get_y(H[r], x);
		return -get_y(H[r], x);
	}

	T query_monotone_inc(T x) {
		assert(!empty());
		while (H.size() >= 2 && get_y(H.front(), x) >= get_y(H[1], x)) H.pop_front();
		if (isMin) return get_y(H.front(), x);
		return -get_y(H.front(), x);
	}

	T query_monotone_dec(T x) {
		assert(!empty());
		while (H.size() >= 2 && get_y(H.back(), x) >= get_y(H[H.size() - 2], x)) H.pop_back();
		if (isMin) return get_y(H.back(), x);
		return -get_y(H.back(), x);
	}
};