#ifndef KWM_T_MATH_FPS_FPS_HPP
#define KWM_T_MATH_FPS_FPS_HPP

#include <vector>
#include <algorithm>
#include "factorial.hpp"
#include "atcoder/convolution"

namespace kwm_t::math::fps {


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
			f.resize(2 * m), atcoder::internal::butterfly(f);
			g.resize(2 * m), atcoder::internal::butterfly(g);
			for (int i = 0; i < 2 * m; ++i) f[i] *= g[i];
			atcoder::internal::butterfly_inv(f);
			f.erase(f.begin(), f.begin() + m);
			f.resize(2 * m), atcoder::internal::butterfly(f);
			for (int i = 0; i < 2 * m; ++i) f[i] *= g[i];
			atcoder::internal::butterfly_inv(f);
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
			f_fft.resize(2 * m), atcoder::internal::butterfly(f_fft);

			// Step 2.a'
			// {
			F _g(m);
			for (int i = 0; i < m; ++i) _g[i] = f_fft[i] * g_fft[i];
			atcoder::internal::butterfly_inv(_g);
			_g.erase(_g.begin(), _g.begin() + m / 2);
			_g.resize(m), atcoder::internal::butterfly(_g);
			for (int i = 0; i < m; ++i) _g[i] *= g_fft[i];
			atcoder::internal::butterfly_inv(_g);
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
			r.resize(m); atcoder::internal::butterfly(r);
			for (int i = 0; i < m; ++i) r[i] *= f_fft[i];
			atcoder::internal::butterfly_inv(r);
			r /= -m;
			// Step 2.d'
			t += r;
			t.insert(t.begin(), t.back()); t.pop_back();
			// }

			// Step 2.e'
			if (2 * m < d) {
				t.resize(2 * m); atcoder::internal::butterfly(t);
				g_fft = g; g_fft.resize(2 * m); atcoder::internal::butterfly(g_fft);
				for (int i = 0; i < 2 * m; ++i) t[i] *= g_fft[i];
				atcoder::internal::butterfly_inv(t);
				t.resize(m);
				t /= 2 * m;
			}
			else { // この場合分けをしても数パーセントしか速くならない
				F g1(g.begin() + m / 2, g.end());
				F s1(t.begin() + m / 2, t.end());
				t.resize(m / 2);
				g1.resize(m), atcoder::internal::butterfly(g1);
				t.resize(m), atcoder::internal::butterfly(t);
				s1.resize(m), atcoder::internal::butterfly(s1);
				for (int i = 0; i < m; ++i) s1[i] = g_fft[i] * s1[i] + g1[i] * t[i];
				for (int i = 0; i < m; ++i) t[i] *= g_fft[i];
				atcoder::internal::butterfly_inv(t);
				atcoder::internal::butterfly_inv(s1);
				for (int i = 0; i < m / 2; ++i) t[i + m / 2] += s1[i];
				t /= m;
			}

			// Step 2.f'
			F v(this->begin() + m, this->begin() + std::min<int>(d, 2 * m)); v.resize(m);
			t.insert(t.begin(), m - 1, 0); t.push_back(0);
			t.integ_inplace();
			for (int i = 0; i < m; ++i) v[i] -= t[m + i];

			// Step 2.g'
			v.resize(2 * m); atcoder::internal::butterfly(v);
			for (int i = 0; i < 2 * m; ++i) v[i] *= f_fft[i];
			atcoder::internal::butterfly_inv(v);
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


} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_FPS_HPP