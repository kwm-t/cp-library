/**
 * @file determinant.hpp
 * @brief 行列式
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/matrix/determinant.hpp
 */
#ifndef KWM_T_MATH_MATRIX_DETERMINANT_HPP
#define KWM_T_MATH_MATRIX_DETERMINANT_HPP

#include <vector>

/**
 * @brief 行列式（determinant）
 *
 * 正方行列 A の行列式を求める。
 *
 * 典型用途:
 *   - 行列の可逆判定
 *   - 線形独立判定
 *
 * 計算量:
 *   - O(n^3)
 *
 * @tparam T 体（+,-,*,/ が使える型）
 *
 * @param A 入力行列（コピーして使う）
 *
 * 制約 / 注意:
 *   - A は正方行列
 *   - 0除算が起きないように pivot を選ぶ
 *   - 行交換で符号が反転することに注意
 *
 * 使用例:
 *   auto det = determinant<mint>(A);
 *
 * verified:
 *   https://judge.yosupo.jp/submission/367304
 */
namespace kwm_t::math::matrix {

template <class T>
T determinant(std::vector<std::vector<T>> A) {
	int n = A.size();
	T det = T(1);

	for (int i = 0; i < n; ++i) {
		// pivot 探索
		int pivot = i;
		while (pivot < n && A[pivot][i] == T(0)) {
			++pivot;
		}

		// すべて0 → det = 0
		if (pivot == n) return T(0);

		// 行交換
		if (i != pivot) {
			std::swap(A[i], A[pivot]);
			det = -det;
		}

		// pivot を掛ける
		det *= A[i][i];

		// 下を消す
		T inv = T(1) / A[i][i];
		for (int j = i + 1; j < n; ++j) {
			T factor = A[j][i] * inv;
			for (int k = i; k < n; ++k) {
				A[j][k] -= factor * A[i][k];
			}
		}
	}

	return det;
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_MATRIX_DETERMINANT_HPP