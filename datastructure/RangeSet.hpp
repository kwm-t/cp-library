#include <set>
#include <iostream> 

template<typename T>
struct RangeSet {
	// ReadMe:•Â‹æŠÔ‚È‚±‚Æ‚É’ˆÓ
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
	T Insert(T l, T r) {
		//insert‚Åmarge‚µ‚È‚¢ver
		//st.emplace(l, r);
		//return 0;
		T sumErase = T(0);
		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));
		// no need merge
		if (IsCovered(l, r))return sumErase;
		// first merge pos
		if ((ite->first <= l) && (l <= (ite->second + 1))) {
			l = ite->first;
			sumErase += ite->second - ite->first + 1;
			ite = st.erase(ite);
		}
		else ite = next(ite);
		// merge
		while (r > ite->second) {
			sumErase += ite->second - ite->first + 1;
			ite = st.erase(ite);
		}
		if (((ite->first - 1) <= r) && (r <= ite->second)) {
			r = ite->second;
			sumErase += ite->second - ite->first + 1;
			st.erase(ite);
		}
		st.emplace(l, r);
		return (r - l + 1) - sumErase;
	}
	T Insert(T x) { return Insert(x, x); }
	// erase
	T Erase(T l, T r) {
		T ret = T(0);
		auto get = ByCovered(l, r);
		if ((-TINF) != get.first) {
			st.erase(get);
			if (get.first != l)st.emplace(get.first, l - 1);
			if (get.second != r)st.emplace(r + 1, get.second);
			ret += r - l + 1;
			return ret;
		}
		auto ite = prev(st.lower_bound({ l + 1,l + 1 }));
		// first erase
		if ((ite->first <= l) && (l <= ite->second)) {
			ret += ite->second - l + 1;
			if (ite->first != l)st.emplace(ite->first, l - 1);
			ite = st.erase(ite);
		}
		else ite = next(ite);
		// erase
		while (ite->second <= r) {
			ret += ite->second - ite->first + 1;
			ite = st.erase(ite);
		}
		// last
		if ((ite->first <= r) && (r <= ite->second)) {
			ret += r - ite->first + 1;
			if (ite->second != r) st.emplace(r + 1, ite->second);
			st.erase(ite);
		}
		return ret;
	}
	T Erase(T x) { return Erase(x, x); }
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