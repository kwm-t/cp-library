#include <vector>
template<class T>
struct Formal_power_series : std::vector<T> {
	using std::vector<T>::vector;
	using std::vector<T>::operator=;
	using F = Formal_power_series;
	F operator-() const {
		F res(*this);
		for (auto &e : res) e = -e;
		return res;
	}
	F inv(int d = -1) const {
		int n = this->size();
		assert(n != 0 && (*this)[0] != 0);
		if (d == -1) d = n;
		assert(d >= 0);
		F res{ (*this)[0].inv() };
		for (int m = 1; m < d; m *= 2) {
			F f(this->begin(), this->begin() + min(n, 2 * m));
			F g(res);
			f.resize(2 * m), internal::butterfly(f);
			g.resize(2 * m), internal::butterfly(g);
			for (int i = 0; i < 2 * m; ++i) f[i] *= g[i];
			internal::butterfly_inv(f);
			f.erase(f.begin(), f.begin() + m);
			f.resize(2 * m), internal::butterfly(f);
			for (int i = 0; i < 2 * m; ++i) f[i] *= g[i];
			internal::butterfly_inv(f);
			T iz = T(2 * m).inv(); iz *= -iz;
			for (int i = 0; i < m; ++i) f[i] *= iz;
			res.insert(res.end(), f.begin(), f.begin() + m);
		}
		res.resize(d);
		return res;
	}
	F &divide_inplace(const F &g, int d = -1) {
		int n = this->size();
		if (d == -1) d = n;
		assert(d >= 0);
		*this = convolution(move(*this), g.inv(d));
		this->resize(d);
		return *this;
	}
	F divide(const F &g, const int d = -1) const { return F(*this).divide_inplace(g, d); }
};

template<class T>
struct Bostan_mori {
	std::vector<T> p, q;
	Bostan_mori(std::vector<T> &_p, std::vector<T> &_q) : p(_p), q(_q) {}
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
			_p = convolution(move(_p), _q_rev);
			if (n & 1) odd(_p);
			else     even(_p);
			_q = convolution(move(_q), move(_q_rev));
			even(_q);
			_q_rev = _q; rev(_q_rev);
		}
		return _p[0] / _q[0];
	}
};