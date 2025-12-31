template<class T, T(*add)(T, T), T(*zero)(), T(*mul)(T, T), T(*one)(), int N>
struct Matrix : std::array<std::array<T, N>, N> {
	using M = Matrix<T, add, zero, mul, one, N>;
	void make_identity() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				(*this)[i][j] = zero();
			}
		}
		for (int i = 0; i < N; i++) {
			(*this)[i][i] = one();
		}
	}
	M& operator+=(const M& rhs) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				(*this)[i][j] = add((*this)[i][j], rhs[i][j]);
			}
		}
		return *this;
	}
	M& operator*=(const M& rhs) {
		static M temp;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				temp[i][j] = zero();
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
	template <class I>
	void inplace_pow(I k) {
		assert(k >= 0);
		static M temp;
		temp = *this;
		make_identity();
		while (k) {
			if (k & 1) *this *= temp;
			k >>= 1;
			if (k) temp *= temp;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const M& A) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				os << A[i][j] << " \n"[j + 1 == N];
			}
		}
		return os;
	}
};

// tropical semiring
long long add(long long x, long long y) { return std::max(x, y); }
long long zero() { return -1002003004005006007; }
long long mul(long long x, long long y) { return x + y; }
long long one() { return 0; }
using M = Matrix<long long, add, zero, mul, one, 100>;