#include <set>
#include <iostream> 

template<typename T>
struct RangeSet {
	// ReadMe:閉区間なことに注意
	// member
	std::set<std::pair<T, T>>st;
	T TINF;
	// constructor
	RangeSet(T lim) {
		TINF = lim + 1;
		st.emplace(-TINF, -TINF);
		st.emplace(TINF, TINF);
	}
	// cover check
	bool IsCovered(T l, T r) {
		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));
		return ((ite->first <= l) && (r <= ite->second));
	}
	bool IsCovered(T x) { return IsCovered(x, x); }
	// get by covered
	std::pair<T, T> ByCovered(T l, T r) {
		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));
		if ((ite->first <= l) && (r <= ite->second)) return *ite;
		return { -TINF, -TINF };
	}
	std::pair<T, T> ByCovered(T  x) { return ByCovered(x, x); }
	// getFront
	std::pair<T, T>getFront() {
		auto itr = st.begin();
		itr++;
		return *itr;
	}
	// getBack
	std::pair<T, T>getBack() {
		auto itr = st.end();
		itr--;
		itr--;
		return *itr;
	}
	// insert
	void Insert(T l, T r,
		const std::function<void(int, int)> &insert = [](int, int) {},
		const std::function<void(int, int)> &erase = [](int, int) {}) {

		auto eraseRange = [&](auto &ite) {
			erase(ite->first, ite->second);
			ite = st.erase(ite);
		};
		auto insertRange = [&](T l, T r) {
			insert(l, r);
			st.emplace(l, r);
		};

		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));
		// no need merge
		if (IsCovered(l, r))return;

		// first merge pos
		if ((ite->first <= l) && (l <= (ite->second + 1))) {
			l = ite->first;
			eraseRange(ite);
		}
		else ite = next(ite);
		// merge
		while (r > ite->second) {
			eraseRange(ite);
		}
		if (((ite->first - 1) <= r) && (r <= ite->second)) {
			r = ite->second;
			eraseRange(ite);
		}
		insertRange(l, r);
	}
	void Insert(T x,
		const std::function<void(int, int)> &insert = [](int, int) {},
		const std::function<void(int, int)> &erase = [](int, int) {}
	) {
		Insert(x, x, insert, erase);
	}
	// erase
	void Erase(T l, T r,
		const std::function<void(int, int)> &insert = [](int, int) {},
		const std::function<void(int, int)> &erase = [](int, int) {}) {

		auto eraseRange = [&](auto &ite) {
			erase(ite->first, ite->second);
			T len = ite->second - ite->first + 1;
			ite = st.erase(ite);
			return len;
		};
		auto insertRange = [&](T l, T r) {
			insert(l, r);
			st.emplace(l, r);
		};

		auto get = ByCovered(l, r);
		if ((-TINF) != get.first) {
			erase(get.first, get.second);
			st.erase(get);
			if (get.first != l) insertRange(get.first, l - 1);
			if (get.second != r) insertRange(r + 1, get.second);
			return;
		}

		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));

		// first erase
		if ((ite->first <= l) && (l <= ite->second)) {
			if (ite->first != l)  insertRange(ite->first, l - 1);
			eraseRange(ite);
		}
		else ite = next(ite);

		// erase
		while (ite->second <= r) eraseRange(ite);

		// last
		if ((ite->first <= r) && (r <= ite->second)) {
			if (ite->second != r)  insertRange(r + 1, ite->second);
			eraseRange(ite);
		}
	}
	void Erase(T x,
		const std::function<void(int, int)> &insert = [](int, int) {},
		const std::function<void(int, int)> &erase = [](int, int) {}) {
		Erase(x, x, insert, erase);
	}
	// range count
	int size() { return st.size() - 2; }
	// mex
	T Mex(T x = 0) {
		if (!IsCovered(x)) return x;
		auto ite = prev(st.lower_bound({ x + 1, x + 1 }));
		return ite->second + 1;
	}
	// debug
	void Debug() {
		for (auto[l, r] : st)std::cout << l << " " << r << std::endl;
	}
};
// 区間の変更があるとコールバックが呼ばれるのでセグ木で良い感じにしたい場合とかはいい感じにする
// https://atcoder.jp/contests/abc430/submissions/70631649