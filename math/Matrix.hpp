template<typename T>
struct Matrix {
	int row, column;
	std::vector<std::vector<T>>mat;
	Matrix(int r = 0, int c = 0, T val = 0) :row(r), column(c) {
		mat = std::vector<std::vector<T>>(row, std::vector<T>(column, val));
	}
	std::vector<T>& operator[](const int i) {
		return mat[i];
	}
	Matrix& operator+=(const Matrix & other) {
		assert(row == other.row && column == other.column);
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) {
				mat[i][j] += other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator-=(const Matrix & other) {
		assert(row == other.row && column == other.column);
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) {
				mat[i][j] -= other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator*=(const Matrix & other) {
		assert(column == other.row);
		Matrix ret(row, other.column);
		for (int i = 0; i < row; ++i) {
			for (int k = 0; k < column; ++k) {
				for (int j = 0; j < other.column; ++j) {
					ret.mat[i][j] += mat[i][k] * other.mat[k][j];
				}
			}
		}
		return *this = ret;
	}
	Matrix& operator*=(T k) {
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) {
				mat[i][j] *= k;
			}
		}
		return *this;
	}
	Matrix operator+(const Matrix other)const { return Matrix(*this) += other; }
	Matrix operator-(const Matrix other)const { return Matrix(*this) -= other; }
	Matrix operator*(const Matrix other)const { return Matrix(*this) *= other; }
	Matrix operator*(const T k)const { return Matrix(*this) *= k; }
	std::vector<T> operator*(const std::vector<T> other) const {
		assert(column == (int)other.size());
		std::vector<T> ret(row);
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) {
				ret[i] += mat[i][j] * other[j];
			}
		}
		return ret;
	}
	Matrix pow(long long n) const {
		assert(row == column);
		if (n == 0) {
			Matrix e(row, column);
			for (int i = 0; i < row; ++i) e.mat[i][i] = 1;
			return e;
		}
		Matrix ret = pow(n >> 1);
		ret *= ret;
		if (n & 1) ret *= *this;
		return ret;
	}
	Matrix debug() {
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) std::cout << mat[i][j] << " ";
			std::cout << std::endl;
		}
	}
};