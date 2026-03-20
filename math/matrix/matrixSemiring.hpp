#ifndef KWM_T_MATH_MATRIX_SEMIRING_HPP
#define KWM_T_MATH_MATRIX_SEMIRING_HPP

// #include
#include <array>
#include <cassert>

/**
 * @brief 半環（Semiring）上の固定サイズ行列（高速版）
 *
 * Semiring を struct として渡すことで完全にインライン化される。
 *
 * 典型用途:
 *   - 最短路（min-plus）
 *   - 最長路（max-plus / tropical）
 *   - 到達可能性（bool）
 *   - DP遷移
 *
 * 計算量:
 *   - 乗算: O(N^3)
 *   - 累乗: O(N^3 log K)
 *
 * @tparam S
 *   - 半環構造（add, mul, zero, one を持つ）
 * @tparam N
 *   - 行列サイズ（固定）
 *
 * 制約 / 注意:
 *   - S は以下を持つこと:
 *       using T;
 *       static T add(T, T);
 *       static T mul(T, T);
 *       static T zero();
 *       static T one();
 *
 * 使用例:
 *   struct MaxPlus {
 *       using T = long long;
 *       static T add(T a, T b) { return std::max(a, b); }
 *       static T mul(T a, T b) { return a + b; }
 *       static T zero() { return -(1LL<<60); }
 *       static T one() { return 0; }
 *   };
 *
 *   using M = MatrixSemiring<MaxPlus, 2>;
 *
 * verified:
 *   https://atcoder.jp/contests/awc0021/submissions/74245316
 */
namespace kwm_t::math::matrix {

	template<class S, int N>
	struct MatrixSemiring : std::array<std::array<typename S::T, N>, N> {
		using T = typename S::T;
		using M = MatrixSemiring;

		// ---- identity ----
		static M identity() {
			M r{};
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					r[i][j] = S::zero();
				}
			}
			for (int i = 0; i < N; i++) {
				r[i][i] = S::one();
			}
			return r;
		}

		void make_identity() { *this = identity(); }

		// ---- +, += ----
		M& operator+=(const M& rhs) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					(*this)[i][j] = S::add((*this)[i][j], rhs[i][j]);
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
					temp[i][k] = S::zero();
				}
			}

			for (int i = 0; i < N; i++) {
				for (int k = 0; k < N; k++) {
					for (int j = 0; j < N; j++) {
						temp[i][k] = S::add(temp[i][k],
							S::mul((*this)[i][j], rhs[j][k]));
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

} // namespace kwm_t::math::matrix

#endif // KWM_T_MATH_MATRIX_SEMIRING_HPP