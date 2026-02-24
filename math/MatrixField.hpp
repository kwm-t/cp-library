template<class T>
struct MatrixField {
	int H, W;
	std::vector<std::vector<T>> a;

	MatrixField() : H(0), W(0) {}
	MatrixField(int n, T init = T()) : H(n), W(n), a(n, std::vector<T>(n, init)) {}
	MatrixField(int h, int w, T init = T()) : H(h), W(w), a(h, std::vector<T>(w, init)) {}

	std::vector<T>& operator[](int i) { return a[i]; }
	const std::vector<T>& operator[](int i) const { return a[i]; }

	// ---- identity (square only) ----
	static MatrixField identity(int n) {
		MatrixField r(n, n, T(0));
		for (int i = 0; i < n; i++) r[i][i] = T(1);
		return r;
	}

	void make_identity() {
		assert(H == W);
		*this = identity(H);
	}

	// ---- +, += ----
	MatrixField& operator+=(const MatrixField& rhs) {
		assert(H == rhs.H && W == rhs.W);
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
				a[i][j] += rhs[i][j];
		return *this;
	}
	friend MatrixField operator+(MatrixField lhs, const MatrixField& rhs) {
		lhs += rhs;
		return lhs;
	}

	// ---- -, -= ----
	MatrixField& operator-=(const MatrixField& rhs) {
		assert(H == rhs.H && W == rhs.W);
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
				a[i][j] -= rhs[i][j];
		return *this;
	}
	friend MatrixField operator-(MatrixField lhs, const MatrixField& rhs) {
		lhs -= rhs;
		return lhs;
	}

	// ---- * (H×W) * (W×X) -> (H×X) ----
	MatrixField operator*(const MatrixField& rhs) const {
		assert(W == rhs.H);
		MatrixField res(H, rhs.W, T(0));
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
				for (int k = 0; k < rhs.W; k++)
					res[i][k] += a[i][j] * rhs[j][k];
		return res;
	}

	// ---- *= (square only) ----
	MatrixField& operator*=(const MatrixField& rhs) {
		assert(H == W && rhs.H == rhs.W && H == rhs.H);
		*this = (*this) * rhs;
		return *this;
	}

	// ---- pow (square only) ----
	template<class I>
	void inplace_pow(I k) {
		assert(H == W && k >= 0);
		MatrixField base = *this;
		*this = identity(H);
		while (k) {
			if (k & 1) *this = (*this) * base;
			base = base * base;
			k >>= 1;
		}
	}

	template<class I>
	MatrixField pow(I k) const {
		MatrixField r = *this;
		r.inplace_pow(k);
		return r;
	}

	// ---- inverse (square only) ----
	void inplace_inv() {
		assert(H == W);
		MatrixField a = *this;
		make_identity();

		for (int i = 0; i < H; ++i) {
			int pivot = i;
			while (pivot < H && a[pivot][i] == T(0)) pivot++;
			assert(pivot < H);

			if (pivot != i) {
				std::swap(a[pivot], a[i]);
				std::swap((*this)[pivot], (*this)[i]);
			}

			T inv = T(1) / a[i][i];
			for (int j = 0; j < W; ++j) {
				a[i][j] *= inv;
				(*this)[i][j] *= inv;
			}

			for (int j = 0; j < H; ++j) if (j != i) {
				T x = a[j][i];
				for (int k = 0; k < W; ++k) {
					a[j][k] -= x * a[i][k];
					(*this)[j][k] -= x * (*this)[i][k];
				}
			}
		}
	}

	MatrixField inv() const {
		MatrixField r = *this;
		r.inplace_inv();
		return r;
	}

	// ---- det (square only) ----
	T det() const {
		assert(H == W);
		MatrixField a = *this;
		T res = T(1);

		for (int i = 0; i < H; ++i) {
			int pivot = i;
			while (pivot < H && a[pivot][i] == T(0)) pivot++;
			if (pivot == H) return T(0);

			if (pivot != i) {
				std::swap(a[pivot], a[i]);
				res = -res;
			}

			res *= a[i][i];
			T inv = T(1) / a[i][i];
			for (int j = i; j < W; ++j) a[i][j] *= inv;

			for (int j = i + 1; j < H; ++j) {
				T c = a[j][i];
				for (int k = i; k < W; ++k)
					a[j][k] -= c * a[i][k];
			}
		}
		return res;
	}

	// ---- transpose ----
	MatrixField transpose() const {
		MatrixField res(W, H, T(0));
		for (int i = 0; i < H; ++i)
			for (int j = 0; j < W; ++j)
				res[j][i] = a[i][j];
		return res;
	}
};