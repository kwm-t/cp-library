/**
 * @file matrix_field.hpp
 * @brief 体上の行列演算
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/matrix/matrix_field.hpp
 */
#ifndef KWM_T_MATH_MATRIX_MATRIX_FIELD_HPP
#define KWM_T_MATH_MATRIX_MATRIX_FIELD_HPP

// #include
#include <vector>
#include <cassert>
#include <algorithm>

/**
 * @brief 体（Field）上の行列クラス（加減乗算・累乗・逆行列・行列式対応）
 *
 * 任意の型 T に対して行列演算を提供する。
 * T は以下を満たす必要がある：
 *  - +, -, *, / が定義されている
 *  - T(0), T(1) が存在する
 *
 * 典型用途:
 *   - 行列累乗による高速DP（遷移行列）
 *   - 逆行列計算（連立一次方程式）
 *   - 行列式計算
 *
 * 計算量:
 *   - 加減算: O(HW)
 *   - 乗算: O(H * W * X)
 *   - 累乗: O(N^3 log K)
 *   - 逆行列: O(N^3)
 *   - 行列式: O(N^3)
 *
 * @tparam T
 *   - 体（Field）上の型（例: double, modint など）
 *
 * @param
 *   - H: 行数
 *   - W: 列数
 *
 * 制約 / 注意:
 *   - 逆行列・行列式は正則性を仮定（非正則なら assert）
 *   - 浮動小数点では誤差に注意
 *   - modint 使用推奨（特に競プロ）
 *
 * 使用例:
 *   MatrixField<mint> A(n);
 *   auto B = A.pow(k);
 *   auto invA = A.inv();
 *
 * verified:
 *   - 未検証（必要なら記入）
 */
namespace kwm_t::math::matrix {

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

		// ---- * ----
		MatrixField operator*(const MatrixField& rhs) const {
			assert(W == rhs.H);
			MatrixField res(H, rhs.W, T(0));
			for (int i = 0; i < H; i++)
				for (int j = 0; j < W; j++)
					for (int k = 0; k < rhs.W; k++)
						res[i][k] += a[i][j] * rhs[j][k];
			return res;
		}

		// ---- *= ----
		MatrixField& operator*=(const MatrixField& rhs) {
			assert(H == W && rhs.H == rhs.W && H == rhs.H);
			*this = (*this) * rhs;
			return *this;
		}

		// ---- pow ----
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

		// ---- inverse ----
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

		// ---- det ----
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

} // namespace kwm_t::math::matrix

#endif // KWM_T_MATH_MATRIX_MATRIX_FIELD_HPP