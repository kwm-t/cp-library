template<class T, T(*add)(T, T), T(*zero)(), T(*mul)(T, T), T(*one)(), int N>
struct MatrixSemiring : std::array<std::array<T, N>, N> {
	using M = MatrixSemiring;

	static M identity() {
		M r{};
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				r[i][j] = zero();
			}
		}
		for (int i = 0; i < N; i++) {
			r[i][i] = one();
		}
		return r;
	}

	void make_identity() { *this = identity(); }

	// ---- +, += ----
	M& operator+=(const M& rhs) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				(*this)[i][j] = add((*this)[i][j], rhs[i][j]);
			}
		}
		return *this;
	}
	friend M operator+(M lhs, const M& rhs) {
		lhs += rhs;
		return lhs;
	}
	// ---- *, *= ----
	M& operator*=(const M& rhs) {
		M temp{};
		for (int i = 0; i < N; i++) {
			for (int k = 0; k < N; k++) {
				temp[i][k] = zero();
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++) {
					temp[i][k] = add(temp[i][k], mul((*this)[i][j], rhs[j][k]));
				}
			}
		}

		*this = temp;
		return *this;
	}
	friend M operator*(M lhs, const M& rhs) {
		lhs *= rhs;
		return lhs;
	}

	// ---- pow ----
	template<class I>
	void inplace_pow(I k) {
		assert(k >= 0);
		M base = *this;
		*this = identity();
		while (k) {
			if (k & 1) *this *= base;
			base *= base;
			k >>= 1;
		}
	}

	template<class I>
	M pow(I k) const {
		M r = *this;
		r.inplace_pow(k);
		return r;
	}
};

//// tropical semiring 
long long add(long long x, long long y) { return std::max(x, y); }
long long zero() { return -1002003004005006007; }
long long mul(long long x, long long y) { return x + y; }
long long one() { return 0; }
using M = MatrixSemiring<long long, add, zero, mul, one, 2>;
// M mat
// mat[0] ={0,1,2}....;