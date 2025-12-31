#include <set>
template<typename T>
struct Median {
	// 中央値管理
	std::multiset<T> x, y;
	Median() {}
	void add(T val) {
		x.insert(val);
		y.insert(*x.rbegin());
		x.erase(x.find(*x.rbegin()));
		x.insert(*y.begin());
		y.erase(y.begin());
		if (x.size() > y.size() + 1) {
			y.insert(*x.rbegin());
			x.erase(x.find(*x.rbegin()));
		}
	}
	// 存在は保証されている
	void del(T val) {
		if (x.end() != x.find(val)) {
			x.erase(x.find(val));
			if (x.size() < y.size()) {
				x.insert(*y.begin());
				y.erase(y.begin());
			}
		}
		else  if (y.end() != y.find(val)) {
			y.erase(y.find(val));
			if (x.size() > y.size() + 1) {
				y.insert(*x.rbegin());
				x.erase(x.find(*x.rbegin()));
			}
		}
		else {
			// error
		}
	}
	T get() {
		if (x.empty())return -1;//error
		return *x.rbegin();
	}
};