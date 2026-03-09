#pragma once
#include <vector>
template<typename T>
struct CumulativeSum {
	std::vector<T> s;
	CumulativeSum() {}
	CumulativeSum(const vector<T>& a) {
		int n = (int)a.size();
		s.resize(n + 1);
		for (int i = 0; i < n; i++) s[i + 1] = s[i] + a[i];
	}
	// [l,r)
	T sum(int l, int r) const { return s[r] - s[l]; }
	// [0,r)
	T prefix(int r) const { return sum(0, r); }
	// [l,n)
	T suffix(int l) const { return  sum(l, s.size() - 1); }
};