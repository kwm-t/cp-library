template <typename T, int SIZE = 1>
struct Matrix {
	array<array<T, SIZE>, SIZE> v;
	Matrix(int i = 0) {
		if (1 == i) { for (int i = 0; i < SIZE; ++i) { v[i][i] = 1; } }
	}
	Matrix(const array<array<T, SIZE>, SIZE> &s) { v = s; }
	std::array<T, SIZE>& operator[](const int i) {
		return v[i];
	}
	Matrix operator*=(const Matrix & m) {
		Matrix s(0);
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				for (int k = 0; k < SIZE; ++k) {
					s.v[i][j] += v[i][k] * m.v[k][j];
				}
			}
		}
		swap(s, *this);
		return *this;
	}
	Matrix operator*(const Matrix other)const { return Matrix(*this) *= other; }
};