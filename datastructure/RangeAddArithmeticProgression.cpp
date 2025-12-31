#include <vector>
#include <iostream> 
template<typename T>
struct RangeAddArithmeticProgression {// 区間に等差数列をAddする
	RangeAddArithmeticProgression(int sz) :sz(sz) {
		v.resize(sz + 1);
		sub.resize(sz + 1);
	}
	void set(T x, T y, int l, int r) {
		//初項x等差yの数列を[l,r)にAddする
		if (r < l) {
			x += (l - r - 1)*y;
			y *= -1;
			l++, r++;
			std::swap(l, r);
		}
		if (sz < r)r = sz;
		if (l < 0)x += abs(l)*y, l = 0;
		v[l] += y, v[r] -= y;
		sub[r] -= y * (r - l);
		sub[l] += x - y, sub[r] -= x - y;
	}
	void build() {
		for (int i = 0; i < sz; ++i)v[i + 1] += v[i];
		for (int i = 0; i < sz + 1; ++i)v[i] += sub[i];
		for (int i = 0; i < sz; ++i)v[i + 1] += v[i];
	}
	void debug() {
		for (int i = 0; i < sz; ++i) std::cout << v[i] << " ";
		std::cout << std::endl;
	}
	int sz;
	std::vector<T>v, sub;
};