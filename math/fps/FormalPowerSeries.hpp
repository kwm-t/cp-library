#include <algorithm>
#include <iostream>
#include <vector>
// https://opt-cp.com/fps-implementation/
// verified by:
// https://judge.yosupo.jp/problem/convolution_mod
// https://judge.yosupo.jp/problem/inv_of_formal_power_series
// https://judge.yosupo.jp/problem/log_of_formal_power_series
// https://judge.yosupo.jp/problem/exp_of_formal_power_series
// https://judge.yosupo.jp/problem/pow_of_formal_power_series
// https://judge.yosupo.jp/problem/polynomial_taylor_shift
// https://judge.yosupo.jp/problem/bernoulong longi_number
// https://judge.yosupo.jp/problem/sharp_p_subset_sum


//using mint = modint998244353;
template<typename T> struct Factorial {
	int MAX;
	std::vector<T> fac, finv;
	Factorial(int m = 0) : MAX(m), fac(m + 1, 1), finv(m + 1, 1) {
		for (int i = 2; i < MAX + 1; ++i) fac[i] = fac[i - 1] * i;
		finv[MAX] /= fac[MAX];
		for (int i = MAX; i >= 3; --i)finv[i - 1] = finv[i] * i;
	}
	T binom(int n, int k) {
		if (k < 0 || n < k) return 0;
		return fac[n] * finv[k] * finv[n - k];
	}
	T perm(int n, int k) {
		if (k < 0 || n < k) return 0;
		return fac[n] * finv[n - k];
	}
};
Factorial<mint> fc;

std::istream &operator>>(std::istream &is, modint998244353 &a) { long long v; is >> v; a = v; return is; }
std::ostream &operator<<(std::ostream &os, const modint998244353 &a) { return os << a.val(); }
std::istream &operator>>(std::istream &is, modint1000000007 &a) { long long v; is >> v; a = v; return is; }
std::ostream &operator<<(std::ostream &os, const modint1000000007 &a) { return os << a.val(); }
template<int m> std::istream &operator>>(std::istream &is, static_modint<m> &a) { long long v; is >> v; a = v; return is; }
template<int m> std::istream &operator>>(std::istream &is, dynamic_modint<m> &a) { long long v; is >> v; a = v; return is; }
template<int m> std::ostream &operator<<(std::ostream &os, const static_modint<m> &a) { return os << a.val(); }
template<int m> std::ostream &operator<<(std::ostream &os, const dynamic_modint<m> &a) { return os << a.val(); }
template<class T> std::istream &operator>>(std::istream &is, std::vector<T> &v) { for (auto &e : v) is >> e; return is; }
template<class T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) { for (auto &e : v) os << e << ' '; return os; }
template<class T>
struct FormalPowerSeries : std::vector<T> {
	FormalPowerSeries(const std::vector<T>& vec) : std::vector<T>(vec) {}
	
	using std::vector<T>::vector;
	using std::vector<T>::operator=;
	using F = FormalPowerSeries;

	F operator-() const {
		F res(*this);
		for (auto &e : res) e = -e;
		return res;
	}
	F &operator*=(const T &g) {
		for (auto &e : *this) e *= g;
		return *this;
	}
	F &operator/=(const T &g) {
		assert(g != T(0));
		*this *= g.inv();
		return *this;
	}
	F &operator+=(const F &g) {
		int n = this->size(), m = g.size();
		for (int i = 0; i < std::min(n, m); ++i) (*this)[i] += g[i];
		return *this;
	}
	F &operator-=(const F &g) {
		int n = this->size(), m = g.size();
		for (int i = 0; i < std::min(n, m); ++i)(*this)[i] -= g[i];
		return *this;
	}
	F &operator<<=(const int d) {
		int n = this->size();
		if (d >= n) *this = F(n);
		this->insert(this->begin(), d, 0);
		this->resize(n);
		return *this;
	}
	F &operator>>=(const int d) {
		int n = this->size();
		this->erase(this->begin(), this->begin() + min(n, d));
		this->resize(n);
		return *this;
	}

	// O(n log n)
	F inv(int d = -1) const {
		int n = this->size();
		assert(n != 0 && (*this)[0] != 0);
		if (d == -1) d = n;
		assert(d >= 0);
		F res{ (*this)[0].inv() };
		for (int m = 1; m < d; m *= 2) {
			F f(this->begin(), this->begin() + std::min(n, 2 * m));
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

	// fast: FMT-friendly modulus only
	// O(n log n)
	F &multiply_inplace(const F &g, int d = -1) {
		int n = this->size();
		if (d == -1) d = n;
		assert(d >= 0);
		*this = convolution(move(*this), g);
		this->resize(d);
		return *this;
	}
	F multiply(const F &g, const int d = -1) const { return F(*this).multiply_inplace(g, d); }
	// O(n log n)
	F &divide_inplace(const F &g, int d = -1) {
		int n = this->size();
		if (d == -1) d = n;
		assert(d >= 0);
		*this = convolution(move(*this), g.inv(d));
		this->resize(d);
		return *this;
	}
	F divide(const F &g, const int d = -1) const { return F(*this).divide_inplace(g, d); }

	// // naive
	// // O(n^2)
	// F &multiply_inplace(const F &g) {
	//   int n = this->size(), m = g.size();
	//   rrep(i, n) {
	//     (*this)[i] *= g[0];
	//     rep2(j, 1, min(i+1, m)) (*this)[i] += (*this)[i-j] * g[j];
	//   }
	//   return *this;
	// }
	// F multiply(const F &g) const { return F(*this).multiply_inplace(g); }
	// // O(n^2)
	// F &divide_inplace(const F &g) {
	//   assert(g[0] != T(0));
	//   T ig0 = g[0].inv();
	//   int n = this->size(), m = g.size();
	//   rep(i, n) {
	//     rep2(j, 1, min(i+1, m)) (*this)[i] -= (*this)[i-j] * g[j];
	//     (*this)[i] *= ig0;
	//   }
	//   return *this;
	// }
	// F divide(const F &g) const { return F(*this).divide_inplace(g); }

	// sparse
	// O(nk)
	F &multiply_inplace(std::vector<std::pair<int, T>> g) {
		int n = this->size();
		auto[d, c] = g.front();
		if (d == 0) g.erase(g.begin());
		else c = 0;

		for (int i = (n - 1); i >= 0; --i) {
			(*this)[i] *= c;
			for (auto &[j, b] : g) {
				if (j > i) break;
				(*this)[i] += (*this)[i - j] * b;
			}
		}
		return *this;
	}
	F multiply(const std::vector<std::pair<int, T>> &g) const { return F(*this).multiply_inplace(g); }
	// O(nk)
	F &divide_inplace(std::vector<std::pair<int, T>> g) {
		int n = this->size();
		auto[d, c] = g.front();
		assert(d == 0 && c != T(0));
		T ic = c.inv();
		g.erase(g.begin());
		for (int i = 0; i < n; ++i) {
			for (auto &[j, b] : g) {
				if (j > i) break;
				(*this)[i] -= (*this)[i - j] * b;
			}
			(*this)[i] *= ic;
		}
		return *this;
	}
	F divide(const std::vector<std::pair<int, T>> &g) const { return F(*this).divide_inplace(g); }

	// multiply and divide (1 + cz^d)
	// O(n)
	void multiply_inplace(const int d, const T c) {
		int n = this->size();
		if (c == T(1)) for (int i = (n - d - 1); i >= 0; --i) (*this)[i + d] += (*this)[i];
		else if (c == T(-1)) for (int i = (n - d - 1); i >= 0; --i) (*this)[i + d] -= (*this)[i];
		else for (int i = (n - d - 1); i >= 0; --i) (*this)[i + d] += (*this)[i] * c;
	}
	// O(n)
	void divide_inplace(const int d, const T c) {
		int n = this->size();
		if (c == T(1)) for (int i = 0; i < n - d; ++i) (*this)[i + d] -= (*this)[i];
		else if (c == T(-1)) for (int i = 0; i < n - d; ++i) (*this)[i + d] += (*this)[i];
		else for (int i = 0; i < n - d; ++i) (*this)[i + d] -= (*this)[i] * c;
	}

	// O(n)
	T eval(const T &a) const {
		T x(1), res(0);
		for (auto e : *this) res += e * x, x *= a;
		return res;
	}

	// O(n)
	F &integ_inplace() {
		int n = this->size();
		assert(n > 0);
		if (n == 1) return *this = F{ 0 };
		this->insert(this->begin(), 0);
		this->pop_back();
		std::vector<T> inv(n);
		inv[1] = 1;
		int p = T::mod();
		for (int i = 2; i < n; ++i) inv[i] = -inv[p%i] * (p / i);
		for (int i = 2; i < n; ++i) (*this)[i] *= inv[i];
		return *this;
	}
	F integ() const { return F(*this).integ_inplace(); }

	// O(n)
	F &deriv_inplace() {
		int n = this->size();
		assert(n > 0);
		for (int i = 2; i < n; ++i) (*this)[i] *= i;
		this->erase(this->begin());
		this->push_back(0);
		return *this;
	}
	F deriv() const { return F(*this).deriv_inplace(); }

	// O(n log n)
	F &log_inplace(int d = -1) {
		int n = this->size();
		assert(n > 0 && (*this)[0] == 1);
		if (d == -1) d = n;
		assert(d >= 0);
		if (d < n) this->resize(d);
		F f_inv = this->inv();
		this->deriv_inplace();
		this->multiply_inplace(f_inv);
		this->integ_inplace();
		return *this;
	}
	F log(const int d = -1) const { return F(*this).log_inplace(d); }

	// O(n log n)
	// https://arxiv.org/abs/1301.5804 (Figure 1, right)
	F &exp_inplace(int d = -1) {
		int n = this->size();
		assert(n > 0 && (*this)[0] == 0);
		if (d == -1) d = n;
		assert(d >= 0);
		F g{ 1 }, g_fft{ 1, 1 };
		(*this)[0] = 1;
		this->resize(d);
		F h_drv(this->deriv());
		for (int m = 2; m < d; m *= 2) {
			// prepare
			F f_fft(this->begin(), this->begin() + m);
			f_fft.resize(2 * m), internal::butterfly(f_fft);

			// Step 2.a'
			// {
			F _g(m);
			for (int i = 0; i < m; ++i) _g[i] = f_fft[i] * g_fft[i];
			internal::butterfly_inv(_g);
			_g.erase(_g.begin(), _g.begin() + m / 2);
			_g.resize(m), internal::butterfly(_g);
			for (int i = 0; i < m; ++i) _g[i] *= g_fft[i];
			internal::butterfly_inv(_g);
			_g.resize(m / 2);
			_g /= T(-m) * m;
			g.insert(g.end(), _g.begin(), _g.begin() + m / 2);
			// }

			// Step 2.b'--d'
			F t(this->begin(), this->begin() + m);
			t.deriv_inplace();
			// {
			  // Step 2.b'
			F r{ h_drv.begin(), h_drv.begin() + m - 1 };
			// Step 2.c'
			r.resize(m); internal::butterfly(r);
			for (int i = 0; i < m; ++i) r[i] *= f_fft[i];
			internal::butterfly_inv(r);
			r /= -m;
			// Step 2.d'
			t += r;
			t.insert(t.begin(), t.back()); t.pop_back();
			// }

			// Step 2.e'
			if (2 * m < d) {
				t.resize(2 * m); internal::butterfly(t);
				g_fft = g; g_fft.resize(2 * m); internal::butterfly(g_fft);
				for (int i = 0; i < 2 * m; ++i) t[i] *= g_fft[i];
				internal::butterfly_inv(t);
				t.resize(m);
				t /= 2 * m;
			}
			else { // この場合分けをしても数パーセントしか速くならない
				F g1(g.begin() + m / 2, g.end());
				F s1(t.begin() + m / 2, t.end());
				t.resize(m / 2);
				g1.resize(m), internal::butterfly(g1);
				t.resize(m), internal::butterfly(t);
				s1.resize(m), internal::butterfly(s1);
				for (int i = 0; i < m; ++i) s1[i] = g_fft[i] * s1[i] + g1[i] * t[i];
				for (int i = 0; i < m; ++i) t[i] *= g_fft[i];
				internal::butterfly_inv(t);
				internal::butterfly_inv(s1);
				for (int i = 0; i < m / 2; ++i) t[i + m / 2] += s1[i];
				t /= m;
			}

			// Step 2.f'
			F v(this->begin() + m, this->begin() + std::min<int>(d, 2 * m)); v.resize(m);
			t.insert(t.begin(), m - 1, 0); t.push_back(0);
			t.integ_inplace();
			for (int i = 0; i < m; ++i) v[i] -= t[m + i];

			// Step 2.g'
			v.resize(2 * m); internal::butterfly(v);
			for (int i = 0; i < 2 * m; ++i) v[i] *= f_fft[i];
			internal::butterfly_inv(v);
			v.resize(m);
			v /= 2 * m;

			// Step 2.h'
			for (int i = 0; i < std::min(d - m, m); ++i)(*this)[m + i] = v[i];
		}
		return *this;
	}
	F exp(const int d = -1) const { return F(*this).exp_inplace(d); }

	// O(n log n)
	F &pow_inplace(const long long k, int d = -1) {
		int n = this->size();
		if (d == -1) d = n;
		assert(d >= 0 && k >= 0);
		if (k == 0) {
			*this = F(d);
			if (d > 0) (*this)[0] = 1;
			return *this;
		}
		int l = 0;
		while (l < n && (*this)[l] == 0) ++l;
		if (l > (d - 1) / k || l == n) return *this = F(d);
		T c = (*this)[l];
		this->erase(this->begin(), this->begin() + l);
		*this /= c;
		this->log_inplace(d - l * k);
		*this *= k;
		this->exp_inplace();
		*this *= c.pow(k);
		this->insert(this->begin(), l*k, 0);
		return *this;
	}
	F pow(const long long k, const int d = -1) const { return F(*this).pow_inplace(k, d); }

	// O(n log n)
	F &shift_inplace(const T c) {
		int n = this->size();
		auto fc = Factorial<T>(n);
		for (int i = 0; i < n; ++i) (*this)[i] *= fc.fac[i];
		reverse(this->begin(), this->end());
		F g(n);
		T cp = 1;
		for (int i = 0; i < n; ++i) g[i] = cp * fc.finv[i], cp *= c;
		this->multiply_inplace(g, n);
		reverse(this->begin(), this->end());
		for (int i = 0; i < n; ++i) (*this)[i] *= fc.finv[i];
		return *this;
	}
	F shift(const T c) const { return F(*this).shift_inplace(c); }

	F operator*(const T &g) const { return F(*this) *= g; }
	F operator/(const T &g) const { return F(*this) /= g; }
	F operator+(const F &g) const { return F(*this) += g; }
	F operator-(const F &g) const { return F(*this) -= g; }
	F operator<<(const int d) const { return F(*this) <<= d; }
	F operator>>(const int d) const { return F(*this) >>= d; }
	F operator*(std::vector<std::pair<int, T>> g) const { return F(*this) *= g; }
	F operator/(std::vector<std::pair<int, T>> g) const { return F(*this) /= g; }
};























using fps = FormalPowerSeries<mint>;
using sfps = std::vector<std::pair<int, mint>>;

// https://judge.yosupo.jp/problem/inv_of_formal_power_series
// f(x)に対して 1/f(x)の先頭n項を求める
void yosupo_inv() {
	int n; std::cin >> n;
	fps a(n); std::cin >> a;
	std::cout << a.inv() << '\n';
}

// https://judge.yosupo.jp/problem/log_of_formal_power_series
// f(x)に対して log(f(x))の先頭n項を求める
void yosupo_log() {
	int n; std::cin >> n;
	fps a(n); std::cin >> a;
	std::cout << a.log_inplace() << '\n';
}

// https://judge.yosupo.jp/problem/exp_of_formal_power_series
// f(x)に対して exp(f(x))の先頭n項を求める
void yosupo_exp() {
	int n; std::cin >> n;
	fps a(n); std::cin >> a;
	std::cout << a.exp_inplace() << '\n';
}

// https://judge.yosupo.jp/problem/pow_of_formal_power_series
// f(x)に対してf(x)^mの先頭n項を求める
void yosupo_pow() {
	int n, m; std::cin >> n >> m;
	fps a(n); std::cin >> a;
	std::cout << a.pow_inplace(m) << '\n';
}

// https://judge.yosupo.jp/problem/polynomial_taylor_shift
// f(x)に対してf(x+c)の先頭n項を求める
void yosupo_shift() {
	int n, c; std::cin >> n >> c;
	fps a(n); std::cin >> a;
	std::cout << a.shift_inplace(c) << '\n';
}

// https://judge.yosupo.jp/problem/bernoulli_number
// ベルヌーイ数の先頭n項を求める
void yosupo_bernoulli() {
	int n; std::cin >> n;
	++n;
	fc = Factorial<mint>(n);
	fps f((fc.finv).begin() + 1, (fc.finv).end());
	f = f.inv();
	for (int i = 0; i < n; ++i) f[i] *= fc.fac[i];
	std::cout << f << '\n';
}

// https://judge.yosupo.jp/problem/sharp_p_subset_sum
// Π(1+x^a)の先頭t+1項を求める
void yosupo_count_subset_sum() {
	int n, t; std::cin >> n >> t;
	++t;
	std::vector<int> c(t);
	for (int i = 0; i < n; ++i) {
		int s; std::cin >> s;
		++c[s];
	}

	std::vector<mint> inv(t);
	inv[1] = 1;
	int p = mint::mod();
	for (int i = 2; i < t; ++i) inv[i] = -inv[p%i] * (p / i);

	fps f(t);
	for (int i = 0; i < t; ++i) {
		if (c[i] == 0) continue;
		for (int j = 1, d = i; d < t; ++j, d += i) {
			if (j & 1) f[d] += c[i] * inv[j];
			else f[d] -= c[i] * inv[j];
		}
	}
	f.exp_inplace();
	f.erase(f.begin());
	std::cout << f << '\n';
}