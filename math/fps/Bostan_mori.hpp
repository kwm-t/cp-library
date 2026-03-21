#ifndef BOSTAN_MORI_HPP
#define BOSTAN_MORI_HPP

#include <vector>
#include <atcoder/all>
template<class T>
struct Bostan_mori {
	std::vector<T> p, q;
	#include <vector>(std::vector<T> &_p, std::vector<T> &_q) : p(_p), q(_q) {}
	void rev(std::vector<T> &f) const {
		int d = f.size();
		for (int i = 0; i < d; ++i) if (i & 1) f[i] = -f[i];
	}
	void even(std::vector<T> &f) const {
		int d = (f.size() + 1) >> 1;
		for (int i = 0; i < d; ++i) f[i] = f[i << 1];
		f.resize(d);
	}
	void odd(std::vector<T> &f) const {
		int d = f.size() >> 1;
		for (int i = 0; i < d; ++i) f[i] = f[i << 1 | 1];
		f.resize(d);
	}
	T operator[] (long long n) const {
		std::vector<T> _p(p), _q(q), _q_rev(q);
		rev(_q_rev);
		for (; n; n >>= 1) {
			_p = atcoder::convolution(move(_p), _q_rev);
			if (n & 1) odd(_p);
			else     even(_p);
			_q = atcoder::convolution(move(_q), move(_q_rev));
			even(_q);
			_q_rev = _q; rev(_q_rev);
		}
		return _p[0] / _q[0];
	}
};

#endif