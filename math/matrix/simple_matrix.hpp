#ifndef KWM_T_MATH_MATRIX_SIMPLE_MATRIX_HPP
#define KWM_T_MATH_MATRIX_SIMPLE_MATRIX_HPP

// #include
#include <vector>
#include <cassert>

/**
 * @brief シンプルな行列演算（vector版・軽量）
 *
 * 最低限の行列演算（乗算・累乗）を提供する。
 * とりあえず使う用の軽量実装。
 *
 * 典型用途:
 *   - 行列累乗（DP遷移）
 *   - 小規模な行列計算
 *
 * 計算量:
 *   - 乗算: O(N^3)
 *   - 累乗: O(N^3 log K)
 *
 * @tparam T
 *   - 要素型（+, *, += が定義されていること）
 *
 * 制約 / 注意:
 *   - 正方行列のみ想定（matrixPow）
 *   - サイズ不一致は assert
 *   - 単位元は T(1) を使用
 *
 * 使用例:
 *   vector<vector<long long>> A(n, vector<long long>(n));
 *   auto B = matrixPow(10, A);
 *
 * verified:
 *   - 未検証
 */
namespace kwm_t::math::matrix {

	template<typename T>
	std::vector<std::vector<T>> matrixMul(
		const std::vector<std::vector<T>>& A,
		const std::vector<std::vector<T>>& B
	) {
		assert(!A.empty() && !B.empty());
		assert(A[0].size() == B.size());

		int n = (int)A.size();
		int m = (int)B[0].size();
		int p = (int)A[0].size();

		std::vector<std::vector<T>> C(n, std::vector<T>(m, T(0)));

		for (int i = 0; i < n; ++i) {
			for (int k = 0; k < p; ++k) {
				for (int j = 0; j < m; ++j) {
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
		return C;
	}

	template<typename T>
	std::vector<std::vector<T>> matrixPow(
		long long n,
		const std::vector<std::vector<T>>& mat
	) {
		assert(!mat.empty());
		assert(mat.size() == mat[0].size());

		int size = (int)mat.size();

		std::vector<std::vector<T>> res(size, std::vector<T>(size, T(0)));
		for (int i = 0; i < size; ++i) {
			res[i][i] = T(1);
		}

		auto base = mat;

		while (n > 0) {
			if (n & 1) res = matrixMul(res, base);
			base = matrixMul(base, base);
			n >>= 1;
		}

		return res;
	}

} // namespace kwm_t::math::matrix

#endif // KWM_T_MATH_MATRIX_SIMPLE_MATRIX_HPP